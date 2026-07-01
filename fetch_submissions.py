import os
import sys
import time
import subprocess

import requests

SUBMISSIONS_DIR = "submissions"
BASE_URL = "https://leetcode.com"
GRAPHQL_URL = "https://leetcode.com/graphql/"

# When set (DRY_RUN=1), no files are written and no commits are made.
DRY_RUN = os.environ.get("DRY_RUN") == "1"

# LeetCode language slug -> file extension (from joshcai/leetcode-sync).
LANG_TO_EXTENSION = {
    "bash": "sh",
    "c": "c",
    "cpp": "cpp",
    "csharp": "cs",
    "dart": "dart",
    "elixir": "ex",
    "erlang": "erl",
    "golang": "go",
    "java": "java",
    "javascript": "js",
    "kotlin": "kt",
    "mssql": "sql",
    "mysql": "sql",
    "oraclesql": "sql",
    "php": "php",
    "python": "py",
    "python3": "py",
    "pythondata": "py",
    "postgresql": "sql",
    "racket": "rkt",
    "ruby": "rb",
    "rust": "rs",
    "scala": "scala",
    "swift": "swift",
    "typescript": "ts",
}

USER_AGENT = (
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0 Safari/537.36"
)


def get_credentials():
    session = os.environ.get("LEETCODE_SESSION")
    csrf = os.environ.get("LEETCODE_CSRF_TOKEN")
    if not session or not csrf:
        print(
            "ERROR: LEETCODE_SESSION and LEETCODE_CSRF_TOKEN environment "
            "variables must both be set."
        )
        sys.exit(1)
    return session, csrf


def build_headers(session, csrf):
    return {
        "content-type": "application/json",
        "origin": BASE_URL,
        "referer": BASE_URL,
        "user-agent": USER_AGENT,
        "x-csrftoken": csrf,
        "cookie": f"csrftoken={csrf}; LEETCODE_SESSION={session};",
    }


def get_extension(lang):
    return LANG_TO_EXTENSION.get(lang, "txt")


def graphql_post(session_obj, query, variables, max_retries=5):
    """POST a GraphQL query with exponential backoff on failure."""
    payload = {"query": query, "variables": variables}
    for retry in range(max_retries + 1):
        try:
            response = session_obj.post(GRAPHQL_URL, json=payload, timeout=30)
            # Locked problems (no LeetCode Premium) return 403.
            if response.status_code == 403:
                return response, None
            response.raise_for_status()
            return response, response.json()
        except Exception as e:
            if retry >= max_retries:
                raise
            wait = 3 ** retry
            print(f"  WARN: request failed ({e}); retrying in {wait}s...")
            time.sleep(wait)


SUBMISSION_LIST_QUERY = """
query ($offset: Int!, $limit: Int!) {
  submissionList(offset: $offset, limit: $limit) {
    hasNext
    submissions {
      id
      lang
      timestamp
      statusDisplay
      title
      titleSlug
    }
  }
}
"""

SUBMISSION_DETAILS_QUERY = """
query submissionDetails($submissionId: Int!) {
  submissionDetails(submissionId: $submissionId) {
    code
    question {
      questionFrontendId
    }
  }
}
"""


def fetch_all_submissions(session_obj):
    """Page through submissionList until hasNext is false."""
    all_subs = []
    offset = 0
    limit = 20
    first = True
    while True:
        if not first:
            time.sleep(1)
        first = False

        _, data = graphql_post(
            session_obj, SUBMISSION_LIST_QUERY, {"offset": offset, "limit": limit}
        )
        sub_list = (data or {}).get("data", {}).get("submissionList")
        if sub_list is None:
            print(
                "ERROR: submissionList came back empty. Your LEETCODE_SESSION "
                "cookie is probably expired or invalid."
            )
            sys.exit(1)

        batch = sub_list["submissions"]
        all_subs.extend(batch)
        print(f"  offset={offset}: {len(batch)} rows (total {len(all_subs)})")

        if not sub_list["hasNext"]:
            break
        offset += limit

    return all_subs


def get_details(session_obj, sub_id):
    """Return (code, question_frontend_id) or None if locked/unavailable."""
    response, data = graphql_post(
        session_obj, SUBMISSION_DETAILS_QUERY, {"submissionId": int(sub_id)}
    )
    if data is None:  # 403 locked problem
        print(f"  WARN: submission {sub_id} is locked (Premium); skipping.")
        return None

    details = (data or {}).get("data", {}).get("submissionDetails")
    if not details or details.get("code") is None:
        print(f"  WARN: no code returned for submission {sub_id}; skipping.")
        return None

    code = details["code"]
    question = details.get("question") or {}
    qid = question.get("questionFrontendId") or "0000"
    return code, qid


def git_commit(file_path, title, epoch):
    """Commit a single file with author/committer date set to the solve time."""
    env = dict(os.environ)
    iso = f"{epoch} +0000"  # git accepts "<unix ts> <tz>"
    env["GIT_AUTHOR_DATE"] = iso
    env["GIT_COMMITTER_DATE"] = iso
    subprocess.run(["git", "add", file_path], check=True)
    subprocess.run(
        ["git", "commit", "-m", f"Sync LeetCode submission - {title}"],
        check=True,
        env=env,
    )


def main():
    session, csrf = get_credentials()
    session_obj = requests.Session()
    session_obj.headers.update(build_headers(session, csrf))

    os.makedirs(SUBMISSIONS_DIR, exist_ok=True)

    submissions = fetch_all_submissions(session_obj)
    if not submissions:
        print("ERROR: API returned 0 submissions. Aborting (likely blocked).")
        sys.exit(1)

    print(f"Fetched {len(submissions)} submissions from API.")

    # Keep only the latest Accepted submission per problem (by titleSlug).
    latest_ac = {}
    for sub in submissions:
        if sub["statusDisplay"] != "Accepted":
            continue
        slug = sub["titleSlug"]
        ts = int(sub["timestamp"])
        if slug not in latest_ac or ts > int(latest_ac[slug]["timestamp"]):
            latest_ac[slug] = sub

    print(f"{len(latest_ac)} problems have at least one Accepted submission.")

    # Skip problems already backed up (any file matching "*-<slug>.*").
    existing = set(os.listdir(SUBMISSIONS_DIR)) if os.path.isdir(SUBMISSIONS_DIR) else set()

    todo = [sub for slug, sub in latest_ac.items() if not _slug_exists(existing, slug)]

    # Oldest first so backdated commits are laid down in chronological order.
    todo.sort(key=lambda s: int(s["timestamp"]))
    print(f"{len(todo)} new problems to fetch.")

    new_count = 0
    for sub in todo:
        slug = sub["titleSlug"]
        ext = get_extension(sub["lang"])

        details = get_details(session_obj, sub["id"])
        time.sleep(2)
        if details is None:
            continue
        code, qid = details

        file_name = f"{qid}-{slug}.{ext}"
        file_path = os.path.join(SUBMISSIONS_DIR, file_name)

        if DRY_RUN:
            print(f"  [dry-run] would write {file_path}")
            new_count += 1
            continue

        # Remove stale files for the same slug (different qid/extension).
        for old in os.listdir(SUBMISSIONS_DIR):
            if _slug_of(old) == slug and old != file_name:
                os.remove(os.path.join(SUBMISSIONS_DIR, old))

        print(f"Writing {file_name} (submission {sub['id']})...")
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(code if code.endswith("\n") else code + "\n")

        git_commit(file_path, sub["title"], int(sub["timestamp"]))
        new_count += 1

    if DRY_RUN:
        print(f"Done (dry-run). {new_count} files would be written.")
    else:
        print(f"Done. {new_count} files written and committed.")


def _slug_of(file_name):
    """Extract the <slug> from a "<qid>-<slug>.<ext>" file name."""
    stem = file_name.rsplit(".", 1)[0]
    return stem.split("-", 1)[1] if "-" in stem else stem


def _slug_exists(existing, slug):
    return any(_slug_of(f) == slug for f in existing)


if __name__ == "__main__":
    main()
