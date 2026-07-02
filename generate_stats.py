"""Build a metadata cache from the backed-up solutions and render STATS.md.

Separation of concerns: fetch_submissions.py owns *syncing code*; this script
owns *reporting*. It scans submissions/, fills in any missing problem metadata
(difficulty / topics) from LeetCode, caches it in data/problems.json, and
renders a stats dashboard grouped by difficulty and topic.
"""
import json
import os
import re
import subprocess
import time
from collections import Counter, defaultdict
from datetime import datetime, timedelta, timezone

from leetcode_api import graphql_post, make_session, slug_of

SUBMISSIONS_DIR = "submissions"
CACHE_PATH = os.path.join("data", "problems.json")
STATS_PATH = "STATS.md"
README_PATH = "README.md"

# README section that generate_stats.py owns and rewrites in place.
README_START = "<!-- STATS:START -->"
README_END = "<!-- STATS:END -->"

DIFFICULTY_ORDER = ["Easy", "Medium", "Hard"]

QUESTION_QUERY = """
query getQuestion($titleSlug: String!) {
  question(titleSlug: $titleSlug) {
    questionFrontendId
    title
    difficulty
    topicTags {
      name
    }
  }
}
"""

USERNAME_QUERY = "query { userStatus { username } }"

PROFILE_QUERY = """
query userProfile($username: String!) {
  matchedUser(username: $username) {
    username
    profile {
      realName
      userAvatar
      ranking
    }
    submitStatsGlobal {
      acSubmissionNum { difficulty count }
      totalSubmissionNum { difficulty count }
    }
  }
  allQuestionsCount { difficulty count }
}
"""


def load_cache():
    if os.path.exists(CACHE_PATH):
        with open(CACHE_PATH, encoding="utf-8") as f:
            return json.load(f)
    return {}


def save_cache(cache):
    os.makedirs(os.path.dirname(CACHE_PATH), exist_ok=True)
    with open(CACHE_PATH, "w", encoding="utf-8") as f:
        json.dump(cache, f, ensure_ascii=False, indent=2, sort_keys=True)
        f.write("\n")


def solved_timestamp(file_name):
    """Solve date = the (backdated) commit date of the file, else its mtime."""
    path = os.path.join(SUBMISSIONS_DIR, file_name)
    try:
        out = subprocess.run(
            ["git", "log", "-1", "--format=%ct", "--", path],
            capture_output=True,
            text=True,
            check=True,
        ).stdout.strip()
        if out:
            return int(out)
    except Exception:
        pass
    return int(os.path.getmtime(path))


def fetch_question(session_obj, slug):
    _, data = graphql_post(session_obj, QUESTION_QUERY, {"titleSlug": slug})
    if data is None:
        return None
    return (data or {}).get("data", {}).get("question")


def build_cache(session_obj):
    """Ensure every solution file has an entry in the metadata cache."""
    cache = load_cache()
    files = [
        f
        for f in os.listdir(SUBMISSIONS_DIR)
        if os.path.isfile(os.path.join(SUBMISSIONS_DIR, f))
    ] if os.path.isdir(SUBMISSIONS_DIR) else []

    present_slugs = set()
    for file_name in sorted(files):
        slug = slug_of(file_name)
        present_slugs.add(slug)
        qid = file_name.split("-", 1)[0]
        ext = file_name.rsplit(".", 1)[-1]

        entry = cache.get(slug, {})
        # Fetch metadata only if we don't already have the difficulty.
        if not entry.get("difficulty"):
            q = fetch_question(session_obj, slug)
            time.sleep(1)
            if q:
                entry["title"] = q.get("title", slug)
                entry["difficulty"] = q.get("difficulty", "Unknown")
                entry["topics"] = [t["name"] for t in (q.get("topicTags") or [])]
                qid = q.get("questionFrontendId", qid)
                print(f"  fetched metadata for {slug} ({entry['difficulty']})")
            else:
                entry.setdefault("title", slug)
                entry.setdefault("difficulty", "Unknown")
                entry.setdefault("topics", [])

        entry["qid"] = qid
        entry["ext"] = ext
        entry["file"] = file_name
        entry["timestamp"] = solved_timestamp(file_name)
        cache[slug] = entry

    # Drop entries whose file no longer exists.
    for slug in list(cache):
        if slug not in present_slugs:
            del cache[slug]

    return cache


def _get_username(session_obj):
    env_name = os.environ.get("LEETCODE_USERNAME")
    if env_name:
        return env_name
    try:
        _, data = graphql_post(session_obj, USERNAME_QUERY, {})
        return ((data or {}).get("data", {}).get("userStatus") or {}).get("username")
    except Exception:
        return None


def fetch_profile(session_obj):
    """Fetch profile-level stats (ranking, per-difficulty solved/total).

    Returns a dict, or None on any failure so the rest of the dashboard still
    renders (graceful degradation)."""
    username = _get_username(session_obj)
    if not username:
        print("  WARN: could not determine LeetCode username; skipping profile.")
        return None
    try:
        _, data = graphql_post(session_obj, PROFILE_QUERY, {"username": username})
    except Exception as e:
        print(f"  WARN: profile fetch failed ({e}); skipping profile.")
        return None

    root = (data or {}).get("data", {})
    user = root.get("matchedUser")
    if not user:
        print(f"  WARN: no matchedUser for '{username}'; skipping profile.")
        return None

    def by_diff(nums):
        return {n["difficulty"]: n["count"] for n in (nums or [])}

    stats = user.get("submitStatsGlobal") or {}
    ac = by_diff(stats.get("acSubmissionNum"))
    tot_sub = by_diff(stats.get("totalSubmissionNum"))
    all_counts = by_diff(root.get("allQuestionsCount"))
    profile = user.get("profile") or {}

    ac_all = ac.get("All", 0)
    sub_all = tot_sub.get("All", 0)
    acceptance = (100.0 * ac_all / sub_all) if sub_all else None

    return {
        "username": user.get("username", username),
        "realName": profile.get("realName"),
        "avatar": profile.get("userAvatar"),
        "ranking": profile.get("ranking"),
        "solved": ac,       # {Easy, Medium, Hard, All}
        "total": all_counts,  # {Easy, Medium, Hard, All}
        "acceptanceRate": acceptance,
    }


def _solve_dates(cache):
    """Set of distinct UTC solve dates (as date objects)."""
    dates = set()
    for p in cache.values():
        ts = int(p.get("timestamp", 0) or 0)
        if ts:
            dates.add(datetime.fromtimestamp(ts, timezone.utc).date())
    return dates


def compute_streaks(cache):
    """Return (current_streak, longest_streak) in days from solve dates.

    Multiple solves on the same day count once. The current streak counts
    consecutive days ending today or yesterday (UTC)."""
    dates = sorted(_solve_dates(cache))
    if not dates:
        return 0, 0

    longest = run = 1
    for prev, cur in zip(dates, dates[1:]):
        run = run + 1 if (cur - prev) == timedelta(days=1) else 1
        longest = max(longest, run)

    today = datetime.now(timezone.utc).date()
    date_set = set(dates)
    current = 0
    if today in date_set or (today - timedelta(days=1)) in date_set:
        cursor = today if today in date_set else today - timedelta(days=1)
        while cursor in date_set:
            current += 1
            cursor -= timedelta(days=1)
    return current, longest


def compute_monthly_pace(cache):
    """Return [(YYYY-MM, count), ...] sorted ascending by month."""
    counts = defaultdict(int)
    for p in cache.values():
        ts = int(p.get("timestamp", 0) or 0)
        if ts:
            month = datetime.fromtimestamp(ts, timezone.utc).strftime("%Y-%m")
            counts[month] += 1
    return sorted(counts.items())


def recent_solves(cache, n=10):
    """Return the n most recently solved problems (newest first)."""
    return sorted(
        cache.values(), key=lambda p: int(p.get("timestamp", 0) or 0), reverse=True
    )[:n]


def _bar(count, total, width=20):
    if total == 0:
        return ""
    filled = round(width * count / total)
    return "█" * filled + "░" * (width - filled)


def _profile_card(profile, total_solved):
    """Render a compact profile-card header. Returns a list of md lines."""
    lines = []
    if not profile:
        lines.append(f"**Total solved: {total_solved}**")
        lines.append("")
        return lines

    name = profile.get("username") or "LeetCode"
    ranking = profile.get("ranking")
    avatar = profile.get("avatar")
    header = f"### 👤 {name}"
    if ranking:
        header += f" &nbsp;·&nbsp; 🏆 Global ranking **#{ranking:,}**"
    lines.append(header)
    lines.append("")

    # Per-difficulty coverage table: solved / total (pct)
    solved = profile.get("solved", {})
    totals = profile.get("total", {})
    header_cells = ["**Solved**"]
    value_cells = [f"**{total_solved}**"]
    for diff in DIFFICULTY_ORDER:
        s = solved.get(diff, 0)
        t = totals.get(diff, 0)
        pct = f"{100.0 * s / t:.1f}%" if t else "—"
        header_cells.append(diff)
        value_cells.append(f"{s}/{t} ({pct})")
    lines.append("| " + " | ".join(header_cells) + " |")
    lines.append("| " + " | ".join(["---"] * len(header_cells)) + " |")
    lines.append("| " + " | ".join(value_cells) + " |")
    lines.append("")
    if profile.get("acceptanceRate") is not None:
        lines.append(f"_Acceptance rate: {profile['acceptanceRate']:.1f}%_")
        lines.append("")
    if avatar:
        # Small avatar, right after the card, kept unobtrusive.
        lines.append(f'<img src="{avatar}" width="64" align="right" alt="avatar" />')
        lines.append("")
    return lines


def render_readme_summary(cache, profile):
    """Compact block embedded between the README STATS markers."""
    problems = list(cache.values())
    total = len(problems)
    lang_counts = Counter(p.get("ext", "?") for p in problems)
    topic_counts = Counter(t for p in problems for t in p.get("topics", []))
    current, longest = compute_streaks(cache)
    now = datetime.now(timezone.utc).strftime("%Y-%m-%d")

    lines = ["## 📊 LeetCode Stats", ""]
    lines += _profile_card(profile, total)
    lines.append(
        f"🔥 **Current streak:** {current} day(s) &nbsp;·&nbsp; "
        f"**Longest:** {longest} day(s)"
    )
    lines.append("")

    top_langs = ", ".join(f"`{e}` {c}" for e, c in lang_counts.most_common(3))
    if top_langs:
        lines.append(f"**Top languages:** {top_langs}")
        lines.append("")
    top_topics = " · ".join(f"{t} ({c})" for t, c in topic_counts.most_common(8))
    if top_topics:
        lines.append(f"**Top topics:** {top_topics}")
        lines.append("")

    lines.append(f"➡️ Full dashboard: **[STATS.md](STATS.md)** _(updated {now})_")
    return "\n".join(lines).strip()


def inject_into_readme(summary):
    """Replace the content between the README markers with `summary`."""
    if not os.path.exists(README_PATH):
        print(f"  WARN: {README_PATH} not found; skipping README embed.")
        return
    with open(README_PATH, encoding="utf-8") as f:
        content = f.read()
    if README_START not in content or README_END not in content:
        print(
            f"  WARN: markers {README_START}/{README_END} not found in "
            f"{README_PATH}; skipping README embed."
        )
        return
    block = f"{README_START}\n{summary}\n{README_END}"
    new_content = re.sub(
        re.escape(README_START) + r".*?" + re.escape(README_END),
        lambda _: block,
        content,
        flags=re.DOTALL,
    )
    if new_content != content:
        with open(README_PATH, "w", encoding="utf-8") as f:
            f.write(new_content)
        print(f"Updated {README_PATH} stats section.")


def render_stats(cache, profile=None):
    """Pure function: turn the metadata cache into STATS.md markdown."""
    problems = sorted(cache.values(), key=lambda p: int(p.get("qid") or 0) or 0)
    total = len(problems)

    diff_counts = Counter(p.get("difficulty", "Unknown") for p in problems)
    lang_counts = Counter(p.get("ext", "?") for p in problems)
    topic_counts = Counter(
        topic for p in problems for topic in p.get("topics", [])
    )

    now = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
    current, longest = compute_streaks(cache)
    lines = []
    lines.append("# 📊 LeetCode Stats")
    lines.append("")
    lines.append(f"_Auto-generated by `generate_stats.py` — last updated {now}._")
    lines.append("")

    # Profile card (solved counts + per-difficulty coverage)
    lines += _profile_card(profile, total)

    # Streak
    lines.append(
        f"🔥 **Current streak:** {current} day(s) &nbsp;·&nbsp; "
        f"**Longest streak:** {longest} day(s)"
    )
    lines.append("")

    # By difficulty
    lines.append("## By difficulty")
    lines.append("")
    lines.append("| Difficulty | Solved | |")
    lines.append("| --- | ---: | --- |")
    for diff in DIFFICULTY_ORDER + [
        d for d in diff_counts if d not in DIFFICULTY_ORDER
    ]:
        if diff in diff_counts:
            c = diff_counts[diff]
            lines.append(f"| {diff} | {c} | `{_bar(c, total)}` |")
    lines.append("")

    # By language
    lines.append("## By language")
    lines.append("")
    lines.append("| Language (ext) | Solved |")
    lines.append("| --- | ---: |")
    for ext, c in lang_counts.most_common():
        lines.append(f"| `{ext}` | {c} |")
    lines.append("")

    # Top topics
    lines.append("## Top topics")
    lines.append("")
    lines.append("| Topic | Solved |")
    lines.append("| --- | ---: |")
    for topic, c in topic_counts.most_common(15):
        lines.append(f"| {topic} | {c} |")
    lines.append("")

    # Monthly pace
    pace = compute_monthly_pace(cache)
    if pace:
        peak = max(c for _, c in pace)
        lines.append("## Activity (solves per month)")
        lines.append("")
        lines.append("```")
        for month, c in pace:
            bar = "█" * round(20 * c / peak) if peak else ""
            lines.append(f"{month}  {bar} {c}")
        lines.append("```")
        lines.append("")

    # Recently solved
    recent = recent_solves(cache, 10)
    if recent:
        lines.append("## Recently solved")
        lines.append("")
        lines.append("| # | Problem | Difficulty | Solved |")
        lines.append("| ---: | --- | --- | --- |")
        for p in recent:
            date = datetime.fromtimestamp(
                int(p.get("timestamp", 0) or 0), timezone.utc
            ).strftime("%Y-%m-%d")
            link = f"[{p.get('title', p['file'])}]({SUBMISSIONS_DIR}/{p['file']})"
            lines.append(
                f"| {p.get('qid', '')} | {link} | {p.get('difficulty', '')} | {date} |"
            )
        lines.append("")

    # Full list grouped by difficulty
    lines.append("## Solved problems")
    lines.append("")
    for diff in DIFFICULTY_ORDER + [
        d for d in diff_counts if d not in DIFFICULTY_ORDER
    ]:
        group = [p for p in problems if p.get("difficulty") == diff]
        if not group:
            continue
        lines.append(f"### {diff} ({len(group)})")
        lines.append("")
        lines.append("| # | Problem | Topics | Lang | Solved |")
        lines.append("| ---: | --- | --- | --- | --- |")
        for p in group:
            date = datetime.fromtimestamp(
                int(p.get("timestamp", 0)), timezone.utc
            ).strftime("%Y-%m-%d")
            link = f"[{p.get('title', p['file'])}]({SUBMISSIONS_DIR}/{p['file']})"
            topics = ", ".join(p.get("topics", [])[:4])
            lines.append(
                f"| {p.get('qid', '')} | {link} | {topics} | `{p.get('ext', '')}` | {date} |"
            )
        lines.append("")

    return "\n".join(lines).rstrip() + "\n"


def main():
    session_obj = make_session()
    cache = build_cache(session_obj)
    profile = fetch_profile(session_obj)
    save_cache(cache)
    with open(STATS_PATH, "w", encoding="utf-8") as f:
        f.write(render_stats(cache, profile))
    inject_into_readme(render_readme_summary(cache, profile))
    print(f"Wrote {STATS_PATH} ({len(cache)} problems) and {CACHE_PATH}.")


if __name__ == "__main__":
    main()
