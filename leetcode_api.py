"""Shared LeetCode GraphQL helpers used by the sync and stats scripts."""
import os
import sys
import time

import requests

BASE_URL = "https://leetcode.com"
GRAPHQL_URL = "https://leetcode.com/graphql/"

USER_AGENT = (
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0 Safari/537.36"
)

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


def make_session():
    """Build a requests.Session authenticated from the environment."""
    session, csrf = get_credentials()
    obj = requests.Session()
    obj.headers.update(build_headers(session, csrf))
    return obj


def get_extension(lang):
    return LANG_TO_EXTENSION.get(lang, "txt")


def slug_of(file_name):
    """Extract the <slug> from a "<qid>-<slug>.<ext>" file name."""
    stem = file_name.rsplit(".", 1)[0]
    return stem.split("-", 1)[1] if "-" in stem else stem


def graphql_post(session_obj, query, variables, max_retries=5):
    """POST a GraphQL query with exponential backoff on failure.

    Returns (response, json_or_None). json is None for 403 (locked/Premium).
    """
    payload = {"query": query, "variables": variables}
    for retry in range(max_retries + 1):
        try:
            response = session_obj.post(GRAPHQL_URL, json=payload, timeout=30)
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
