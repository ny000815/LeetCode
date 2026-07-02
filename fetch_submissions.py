import os
import sys
import time
import subprocess

from leetcode_api import get_extension, graphql_post, make_session, slug_of

SUBMISSIONS_DIR = "submissions"

# Commit messages start with this; used to read the last sync time from git history.
COMMIT_PREFIX = "Sync LeetCode submission"

# When set (DRY_RUN=1), no files are written and no commits are made.
DRY_RUN = os.environ.get("DRY_RUN") == "1"


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


def get_last_sync_timestamp():
    """Read the last synced submission time from git history.

    We backdate each commit to its submission time, so the most recent sync
    commit's date is the newest submission we have already synced. The git log
    itself is the state store — no external database or state file is needed.
    """
    try:
        out = subprocess.run(
            ["git", "log", "-1", f"--grep={COMMIT_PREFIX}", "--format=%ct"],
            capture_output=True,
            text=True,
            check=True,
        ).stdout.strip()
        return int(out) if out else 0
    except Exception:
        return 0


def fetch_new_submissions(session_obj, last_ts):
    """Page through submissionList (newest first), stopping once we reach a
    submission at or before last_ts. Returns Accepted submissions newer than
    last_ts."""
    collected = []
    offset = 0
    limit = 20
    first = True
    stop = False
    while not stop:
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
        for sub in batch:
            if int(sub["timestamp"]) <= last_ts:
                stop = True
                break
            if sub["statusDisplay"] == "Accepted":
                collected.append(sub)

        print(f"  offset={offset}: {len(batch)} rows ({len(collected)} new AC so far)")

        if stop or not sub_list["hasNext"]:
            break
        offset += limit

    return collected


def get_details(session_obj, sub_id):
    """Return (code, question_frontend_id) or None if locked/unavailable."""
    _, data = graphql_post(
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
        ["git", "commit", "-m", f"{COMMIT_PREFIX} - {title}"],
        check=True,
        env=env,
    )


def main():
    session_obj = make_session()

    os.makedirs(SUBMISSIONS_DIR, exist_ok=True)

    last_ts = get_last_sync_timestamp()
    if last_ts:
        print(f"Last sync timestamp (from git history): {last_ts}")
    else:
        print("No previous sync found; performing a full backup.")

    new_subs = fetch_new_submissions(session_obj, last_ts)

    # Keep only the latest Accepted submission per problem (by titleSlug).
    latest_ac = {}
    for sub in new_subs:
        slug = sub["titleSlug"]
        ts = int(sub["timestamp"])
        if slug not in latest_ac or ts > int(latest_ac[slug]["timestamp"]):
            latest_ac[slug] = sub

    # Oldest first so backdated commits are laid down in chronological order.
    todo = sorted(latest_ac.values(), key=lambda s: int(s["timestamp"]))
    print(f"{len(todo)} problems to sync (new or newly re-accepted).")

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
            if slug_of(old) == slug and old != file_name:
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


if __name__ == "__main__":
    main()
