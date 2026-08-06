#!/usr/bin/env python3
"""
openhands_primer.py

Starts an ngrok TCP tunnel, then emits a ready-to-paste OpenHands primer
that includes:
  - SSH connection details (host, port, user, private key)
  - Repo path and branch instructions
  - Local file paths (gitignored files OpenHands needs to access via SSH)
  - The full task prompt

Usage:
    python3 ir-research/scripts/openhands_primer.py \
        --prompt   ir-research/OPENHANDS_STEP2_PROMPT.md \
        --branch   kit/ir-design-research \
        --push-to  kit/ir-design-research \
        --repo     ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo \
        --local-files Corpus/corpus.jsonl,datasets/vocab_A.json,datasets/train_A.jsonl,Corpus/stats.json \
        --key-path ~/.ssh/openhands_key \
        --ssh-port 22 \
        --user     philipallen

The script:
  1. Starts ngrok tcp <ssh-port> (if not already tunnelling on that port)
  2. Waits for the public TCP address
  3. Reads the prompt file
  4. Reads the private key file
  5. Prints a formatted primer block to stdout — copy-paste into OpenHands

Press Ctrl-C to stop ngrok when OpenHands is done.
"""

import argparse
import json
import os
import signal
import subprocess
import sys
import time
import urllib.request
from pathlib import Path


NGROK_API = "http://127.0.0.1:4040/api/tunnels"


def find_existing_ngrok_tcp():
    """Check if ngrok is already running and has a TCP tunnel. Return (host, port) or None."""
    try:
        with urllib.request.urlopen(NGROK_API, timeout=2) as resp:
            data = json.loads(resp.read())
        for t in data.get("tunnels", []):
            if t.get("proto") == "tcp":
                addr = t["public_url"].replace("tcp://", "")
                host, port = addr.rsplit(":", 1)
                return host, int(port)
    except Exception:
        pass
    return None


def start_ngrok(ssh_port: int) -> tuple[str, int, subprocess.Popen]:
    """Start ngrok tcp tunnel. Returns (host, port, process)."""
    print(f"Starting ngrok TCP tunnel → localhost:{ssh_port} ...", file=sys.stderr)
    proc = subprocess.Popen(
        ["ngrok", "tcp", str(ssh_port)],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )

    # Poll until the tunnel is up (up to 15s)
    for _ in range(30):
        time.sleep(0.5)
        result = find_existing_ngrok_tcp()
        if result:
            host, port = result
            print(f"  Tunnel ready: {host}:{port}", file=sys.stderr)
            return host, port, proc

    proc.terminate()
    print("ERROR: ngrok tunnel did not come up within 15 seconds.", file=sys.stderr)
    print("  Check: ngrok is authenticated (ngrok config check)", file=sys.stderr)
    sys.exit(1)


def authorized_keys_instructions(pub_key: str | None) -> str:
    """Return the manual step to add OpenHands public key, for inclusion in primer."""
    if pub_key:
        return f"""
## ⚠️  Before OpenHands can connect — add their public key

Run this in your terminal (not via Kit — Kit can't write to ~/.ssh):

    echo '{pub_key.strip()}' >> ~/.ssh/authorized_keys
    chmod 600 ~/.ssh/authorized_keys

Then proceed.
""".strip()
    return "## ⚠️  Add OpenHands' public key to ~/.ssh/authorized_keys before they connect."


def build_primer(
    *,
    ngrok_host: str,
    ngrok_port: int,
    ssh_user: str,
    repo_path: str,
    branch: str,
    push_to: str,
    local_files: list[str],
    private_key_content: str,
    prompt_content: str,
    pub_key: str | None,
) -> str:
    local_files_block = "\n".join(f"  - {repo_path}/{f}" for f in local_files)

    primer = f"""
# OpenHands Session Primer

## SSH Access

Connect to the sandbox machine via:

```
Host: {ngrok_host}
Port: {ngrok_port}
User: {ssh_user}
```

Private key (save as `~/.ssh/maith_sandbox` and `chmod 600`):

```
{private_key_content.strip()}
```

SSH command:
```
ssh -i ~/.ssh/maith_sandbox -p {ngrok_port} {ssh_user}@{ngrok_host}
```

---

## Repo

**Local path on sandbox**: `{repo_path}`

Clone is already present on the sandbox — do not re-clone. Just SSH in and work directly in that directory.

**Branch to work on**: `{branch}`
**Push to**: `{push_to}`

```bash
cd {repo_path}
git checkout {branch}
git pull origin {branch}
```

---

## Local files (gitignored — only accessible via SSH)

These files are not in the remote GitHub repo. Access them on the sandbox via SSH:

{local_files_block}

---

## Task

{prompt_content.strip()}

---

## Commit and push

When done:
```bash
git push origin {push_to}
```

Do NOT open a PR — the reviewer (Kit) will inspect the branch directly.
""".strip()

    return primer


def main():
    parser = argparse.ArgumentParser(description="Generate OpenHands session primer")
    parser.add_argument("--prompt",      required=True,  help="Path to task .md prompt file")
    parser.add_argument("--branch",      required=True,  help="Branch OpenHands works on")
    parser.add_argument("--push-to",     default=None,   help="Branch to push to (default: same as --branch)")
    parser.add_argument("--repo",        default="~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo",
                        help="Absolute local path to the repo on the sandbox machine")
    parser.add_argument("--local-files", default="",
                        help="Comma-separated list of gitignored files (relative to repo root)")
    parser.add_argument("--key-path",    required=True,  help="Path to the SSH private key to share with OpenHands")
    parser.add_argument("--ssh-port",    type=int, default=22, help="Local SSH port (default: 22)")
    parser.add_argument("--user",        default=os.environ.get("USER", "user"), help="SSH username")
    parser.add_argument("--openhands-pub-key", default=None,
                        help="OpenHands public key to include in authorized_keys instructions")
    parser.add_argument("--no-ngrok",   action="store_true",
                        help="Skip starting ngrok — use if tunnel is already running")
    args = parser.parse_args()

    push_to = args.push_to or args.branch

    # --- Resolve paths ---
    prompt_path = Path(args.prompt).expanduser()
    key_path    = Path(args.key_path).expanduser()
    repo_path   = str(Path(args.repo).expanduser())

    if not prompt_path.exists():
        print(f"ERROR: prompt file not found: {prompt_path}", file=sys.stderr)
        sys.exit(1)

    if not key_path.exists():
        print(f"ERROR: private key not found: {key_path}", file=sys.stderr)
        sys.exit(1)

    prompt_content = prompt_path.read_text()
    private_key    = key_path.read_text()
    local_files    = [f.strip() for f in args.local_files.split(",") if f.strip()]

    # --- ngrok ---
    ngrok_proc = None
    if args.no_ngrok:
        result = find_existing_ngrok_tcp()
        if not result:
            print("ERROR: --no-ngrok set but no existing TCP tunnel found.", file=sys.stderr)
            sys.exit(1)
        ngrok_host, ngrok_port = result
        print(f"Using existing ngrok tunnel: {ngrok_host}:{ngrok_port}", file=sys.stderr)
    else:
        existing = find_existing_ngrok_tcp()
        if existing:
            ngrok_host, ngrok_port = existing
            print(f"ngrok already running: {ngrok_host}:{ngrok_port}", file=sys.stderr)
        else:
            ngrok_host, ngrok_port, ngrok_proc = start_ngrok(args.ssh_port)

    # --- Build primer ---
    primer = build_primer(
        ngrok_host=ngrok_host,
        ngrok_port=ngrok_port,
        ssh_user=args.user,
        repo_path=repo_path,
        branch=args.branch,
        push_to=push_to,
        local_files=local_files,
        private_key_content=private_key,
        prompt_content=prompt_content,
        pub_key=args.openhands_pub_key,
    )

    print("\n" + "=" * 70)
    print("OPENHANDS PRIMER — copy everything below this line")
    print("=" * 70 + "\n")
    print(primer)
    print("\n" + "=" * 70)
    print("END OF PRIMER")
    print("=" * 70)

    if ngrok_proc:
        print("\nngrok is running. Press Ctrl-C to stop the tunnel when done.", file=sys.stderr)
        try:
            signal.pause()
        except (KeyboardInterrupt, AttributeError):
            pass
        finally:
            ngrok_proc.terminate()
            print("\nngrok stopped.", file=sys.stderr)


if __name__ == "__main__":
    main()
