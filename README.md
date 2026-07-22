# LeetCode Submission Backup
<!-- STATS:START -->
<div align="center">

![LeetCode](https://img.shields.io/badge/LeetCode-zaki8-1a1a1a?style=for-the-badge&logo=leetcode&logoColor=white)

![Solved](https://img.shields.io/badge/Solved-86-1f6feb?style=for-the-badge) ![Easy](https://img.shields.io/badge/Easy-33%2F956-00b8a3?style=for-the-badge) ![Medium](https://img.shields.io/badge/Medium-51%2F2088-ffb800?style=for-the-badge) ![Hard](https://img.shields.io/badge/Hard-2%2F955-ef4743?style=for-the-badge)

![🔥 Streak](https://img.shields.io/badge/%F0%9F%94%A5%20Streak-3%20days-ff6b6b?style=for-the-badge) ![🏆 Rank](https://img.shields.io/badge/%F0%9F%8F%86%20Rank-%231%2C807%2C685-8957e5?style=for-the-badge) ![Acceptance](https://img.shields.io/badge/Acceptance-98.9%25-2ea043?style=for-the-badge)

![cpp](https://img.shields.io/badge/cpp-65-555555?style=flat-square) ![java](https://img.shields.io/badge/java-11-555555?style=flat-square) ![py](https://img.shields.io/badge/py-10-555555?style=flat-square)

![Array](https://img.shields.io/badge/Array-49-0d1117?style=flat-square) ![String](https://img.shields.io/badge/String-20-0d1117?style=flat-square) ![Hash Table](https://img.shields.io/badge/Hash%20Table-17-0d1117?style=flat-square) ![Two Pointers](https://img.shields.io/badge/Two%20Pointers-14-0d1117?style=flat-square) ![Linked List](https://img.shields.io/badge/Linked%20List-12-0d1117?style=flat-square) ![Breadth-First Search](https://img.shields.io/badge/Breadth--First%20Search-12-0d1117?style=flat-square)

</div>

<div align="center">➡️ <b><a href="STATS.md">Full dashboard →</a></b></div>
<!-- STATS:END -->

*([日本語版](#日本語版) / Japanese version below)*

A GitHub Actions workflow that automatically backs up your **accepted** LeetCode submissions to a Git repository on a daily schedule.

For each problem you have solved, it keeps the source code of your latest accepted submission, named by problem number and slug:

```
submissions/
├── 0001-two-sum.py
├── 0011-container-with-most-water.cpp
├── 0146-lru-cache.py
└── ...
```

The backup runs entirely on GitHub Actions, so once it is set up you do not need to run anything manually. Each solution is committed as a **separate commit whose date is set to when you actually solved the problem**, so the auto-commits land on your GitHub contribution graph on the day you solved each problem — see [Commit identity & contribution graph](#commit-identity--contribution-graph).



---

## How it works

1. It reads the **last sync time from git history**: each solution is committed with its date backdated to the submission time, so the most recent sync commit's date is the newest submission already backed up. The git log itself is the state store — no external database or state file.
2. It calls the LeetCode GraphQL API (`submissionList`) with your session cookie, paginating newest-first via `offset` / `hasNext`, and **stops as soon as it reaches a submission at or before the last sync time**. So each run only pulls what's new.
3. It keeps only the latest accepted submission per problem in that new batch.
4. For each, it fetches the source code via `submissionDetails` and writes it to `submissions/<problem_number>-<title_slug>.<ext>`.
5. Each file is committed individually with the commit date backdated to the submission time, and the workflow pushes them.

Because syncing is keyed on submission time (not filename), a **new accepted submission to an already-solved problem is picked up and updates the file** on the next run. The very first run has no prior sync commit, so it backs up your entire accepted history.

The API approach (GraphQL queries, headers, language map, locked-problem handling) is based on [joshcai/leetcode-sync](https://github.com/joshcai/leetcode-sync); the overall structure and workflow are based on the [AtCoder version](https://github.com/ny000815/AtCoder).

---

## Setup

1. **Obtain your LeetCode cookies.** Log in to [leetcode.com](https://leetcode.com), then:
   - Right-click the page → **Inspect**, and open the **Network** tab.
   - Refresh the page and click any request to `leetcode.com`.
   - Under **Request Headers**, find the `cookie:` line and copy the values of `csrftoken` and `LEETCODE_SESSION`.

2. **Fork or copy** this repository into your own GitHub account (it can be private or public).

3. **Add the cookies as [GitHub secrets](https://docs.github.com/en/actions/security-guides/encrypted-secrets)** in your repo (Settings → Secrets and variables → Actions):
![variables](https://i.postimg.cc/xdWKJgC0/variables.png)
   - `LEETCODE_SESSION` — the `LEETCODE_SESSION` cookie value
   - `LEETCODE_CSRF_TOKEN` — the `csrftoken` cookie value

4. **Give Actions write permission.**　Settings → Actions → General → **Workflow permissions** → select **Read and write permissions**. (The workflow also requests `contents: write`.)

5. **Set the commit identity.** See [the next section](#commit-identity--contribution-graph) — this controls whether the auto-commits appear on your contribution graph.

6. **Run it.** Trigger it manually from the Actions tab (**Run workflow**), or wait for the daily scheduled run.

The first run backs up your entire accepted history, so it may take a while. Subsequent runs only fetch newly solved problems and finish quickly.

> **Note:** the `LEETCODE_SESSION` cookie expires periodically. If the workflow starts failing with an authentication error, repeat step 1 and update the secrets.

---

## Commit identity & contribution graph

The commits are made with the identity configured in `.github/workflows/leetcode-backup.yml`:

```yaml
- name: Configure git identity
  run: |
    git config user.name  "your_github_username"
    git config user.email "your_github_username@users.noreply.github.com"
```

GitHub only adds a commit to your contribution graph when the commit's **author email matches a verified email on your GitHub account**.

| Email you set | Email exposed publicly? | Counts as contribution? |
| --- | --- | --- |
| `your_github_username@users.noreply.github.com` | No | **Yes** |
| `12345678+your_github_username@users.noreply.github.com` | No | **Yes** |
| Your real email | **Yes — gets scraped for spam** | Yes |
| `github-actions[bot]@users.noreply.github.com` | No | No (shows as a bot) |

**Recommended:** use your personal GitHub no-reply address (Settings → Emails). It keeps your real email private **and** still grows your grass. Because each commit is backdated to the submission time, the grass appears on the day you actually solved each problem.

---

## Scheduling and time zones

The daily run is controlled by the `cron` line in `.github/workflows/leetcode-backup.yml`:

```yaml
schedule:
  - cron: '0 5 * * *'   # 05:00 UTC every day
```

- **GitHub Actions cron is always in UTC**; it does not use your local time zone. `0 15 * * *` is 15:00 UTC (= 00:00 JST the next day).
- **Daylight Saving Time is not handled automatically.**
- Scheduled runs can be delayed a few minutes when runners are busy.

---

## Supported languages

The file extension is chosen from the submission language (based on LeetCode's language slugs):

`bash` `c` `cpp` `csharp` `dart` `elixir` `erlang` `golang` `java` `javascript` `kotlin` `mysql` `mssql` `oraclesql` `postgresql` `php` `python` `python3` `pythondata` `racket` `ruby` `rust` `scala` `swift` `typescript`

Anything not in the list is saved with a `.txt` extension. To add a language, edit the `LANG_TO_EXTENSION` dictionary in `fetch_submissions.py`.

---

## Notes & limitations

- **LeetCode Premium–locked problems are skipped** (the API returns 403 for their code/details).
- The API is undocumented and may change; if the backup stops working, check the request/response shape.
- If LeetCode ever blocks the plain `requests` client (Cloudflare), install and swap in [`cloudscraper`](https://pypi.org/project/cloudscraper/) in `fetch_submissions.py` (create the scraper and reuse it instead of `requests.Session()`).

---

## Local testing (dry run)

You can verify authentication without writing files or making commits:

```bash
export LEETCODE_SESSION="..."
export LEETCODE_CSRF_TOKEN="..."
export DRY_RUN=1
python fetch_submissions.py
```

It prints the accepted submissions and the file names it *would* create.

---

## Credits

- API approach based on [leetcode-sync](https://github.com/joshcai/leetcode-sync) by [@joshcai](https://github.com/joshcai).
- Structure based on the [AtCoder Submission Backup](https://github.com/ny000815/AtCoder).

---
---

# 日本語版

# LeetCode 提出コードバックアップ

自分の **AC（正解）した** LeetCode の提出コードを、毎日定期的に Git リポジトリへ自動バックアップする GitHub Actions ワークフローです。

解いた各問題について、最新の AC 提出のソースコードを「問題番号-スラグ」の名前で保存します。

```
submissions/
├── 0001-two-sum.py
├── 0011-container-with-most-water.cpp
├── 0146-lru-cache.py
└── ...
```

バックアップは GitHub Actions 上で完結するため、一度セットアップすれば手動で何かを実行する必要はありません。各提出は **コミット日時を実際に解いた日時に設定した個別コミット**として記録されるので、GitHub のコントリビューショングラフ（草）には解いた日に草が生えます。詳しくは [コミットの author 設定とコントリビューショングラフ](#コミットの-author-設定とコントリビューショングラフ-1) を参照してください。

---

## 仕組み

1. **前回同期時刻を git 履歴から読み取ります**。各解答はコミット日時を提出日時に遡らせて記録されるため、直近の同期コミットの日時＝すでにバックアップ済みの最新提出の時刻になります。git のログそのものが状態ストアで、外部 DB や状態ファイルは不要です。
2. LeetCode の GraphQL API（`submissionList`）をセッション Cookie 付きで新しい順にページングし、**前回同期時刻以前の提出に達した時点で取得を打ち切ります**。つまり毎回「新しい分だけ」を取得します。
3. その新規バッチの中で、各問題の最新 AC 提出だけを残します。
4. それぞれの提出について `submissionDetails` でソースコードを取得し、`submissions/<問題番号>-<title_slug>.<ext>` に書き出します。
5. 各ファイルを、コミット日時を提出日時に遡らせて個別にコミットし、ワークフローが push します。

同期の基準は提出日時（ファイル名ではない）なので、**既に解いた問題に新しい AC 提出をすると、次回実行時にそれを取り込んでファイルを更新します**。初回実行時は同期コミットが無いため、AC 履歴の全件をバックアップします。

API の実装方針（GraphQL クエリ・ヘッダ・言語マップ・ロック問題の扱い）は [joshcai/leetcode-sync](https://github.com/joshcai/leetcode-sync) を、全体構成とワークフローは [AtCoder 版](https://github.com/ny000815/AtCoder) を参考にしています。

---

## セットアップ

1. **LeetCode の Cookie を取得** します。[leetcode.com](https://leetcode.com) にログインし、
   - ページを右クリック →「検証（Inspect）」→ **Network** タブを開く。
   - ページを再読み込みし、`leetcode.com` へのリクエストをクリック。
   - **Request Headers** の `cookie:` 行から `csrftoken` と `LEETCODE_SESSION` の値をコピー。

2. このリポジトリを自分の GitHub アカウントに **フォーク（または複製）** します（private / public どちらでも可）。

3. 取得した値を **GitHub Secrets** に登録します（Settings → Secrets and variables → Actions）：
![variables](https://i.postimg.cc/xdWKJgC0/variables.png)
   - `LEETCODE_SESSION` … `LEETCODE_SESSION` Cookie の値
   - `LEETCODE_CSRF_TOKEN` … `csrftoken` Cookie の値

4. **Actions に書き込み権限を付与** します。Settings → Actions → General → **Workflow permissions** で **Read and write permissions** を選択。

5. **コミットの author を設定** します。[次のセクション](#コミットの-author-設定とコントリビューショングラフ-1)を参照。

6. **実行** します。Actions タブから手動実行（**Run workflow**）するか、毎日の定期実行を待ちます。

初回実行では AC 履歴の全件をバックアップするため時間がかかることがあります。2 回目以降は新しく解いた問題だけを取得するのですぐ終わります。

> **注意：** `LEETCODE_SESSION` Cookie は定期的に失効します。認証エラーで失敗し始めたら、手順 1 をやり直して Secrets を更新してください。

---

## コミットの author 設定とコントリビューショングラフ

コミットは `.github/workflows/leetcode-backup.yml` の `git config` で設定した identity で作成されます。

```yaml
- name: Configure git identity
  run: |
    git config user.name  "your_github_username"
    git config user.email "your_github_username@users.noreply.github.com"
```

GitHub は、コミットの **author メールが自分のアカウントに登録済み（verified）のメールと一致する場合にのみ** 草を生やします。

| 設定するメール | メールが公開される？ | 草が生える？ |
| --- | --- | --- |
| `your_github_username@users.noreply.github.com` | されない | **生える** |
| `12345678+your_github_username@users.noreply.github.com` | されない | **生える** |
| 実際のメール | **される — スパムに収集される** | 生える |
| `github-actions[bot]@users.noreply.github.com` | されない | 生えない（bot 扱い） |

**推奨：** 自分の GitHub no-reply アドレス（Settings → Emails）を使ってください。実際のメールを非公開に保ったまま草も生やせます。各コミットは提出日時に遡って記録されるため、**解いた日**に草が生えます。

---

## スケジュールとタイムゾーン

毎日の実行は `.github/workflows/leetcode-backup.yml` の `cron` 行で制御されます。

```yaml
schedule:
  - cron: '0 5 * * *'   # 毎日 05:00 UTC
```

- **GitHub Actions の cron は常に UTC** です。`0 15 * * *` は 15:00 UTC（日本時間 JST の翌 0:00）。
- **夏時間は自動調整されません。**
- 定期実行はランナー混雑時に数分遅延することがあります。

---

## 対応言語

ファイルの拡張子は提出言語（LeetCode の言語スラグ）から選ばれます。

`bash` `c` `cpp` `csharp` `dart` `elixir` `erlang` `golang` `java` `javascript` `kotlin` `mysql` `mssql` `oraclesql` `postgresql` `php` `python` `python3` `pythondata` `racket` `ruby` `rust` `scala` `swift` `typescript`

リストにないものは `.txt` で保存されます。言語を追加するには `fetch_submissions.py` の `LANG_TO_EXTENSION` を編集してください。

---

## 注意点・制約

- **LeetCode Premium 限定のロック問題はスキップされます**（コード取得 API が 403 を返すため）。
- API は非公式のため仕様が変わることがあります。動かなくなったらリクエスト/レスポンス形式を確認してください。
- LeetCode に `requests` がブロックされる（Cloudflare）場合は、[`cloudscraper`](https://pypi.org/project/cloudscraper/) を導入し `fetch_submissions.py` の `requests.Session()` を差し替えてください。

---

## ローカルでの動作確認（dry run）

ファイル書き込みやコミットをせずに認証だけ確認できます。

```bash
export LEETCODE_SESSION="..."
export LEETCODE_CSRF_TOKEN="..."
export DRY_RUN=1
python fetch_submissions.py
```

AC 提出一覧と、作成される予定のファイル名が表示されます。

---

## クレジット

- API 実装方針は [@joshcai](https://github.com/joshcai) 氏の [leetcode-sync](https://github.com/joshcai/leetcode-sync) を参考にしています。
- 構成は [AtCoder 提出コードバックアップ](https://github.com/ny000815/AtCoder) を参考にしています。
