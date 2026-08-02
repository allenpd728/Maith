#!/usr/bin/env python3
"""
watcher_log.py — append one structured line to the watcher poll log.

Usage:
    python3 python/watcher_log.py --variant B --status skipped --epochs 1 --ppl 1.1471 --minutes 41.2
    python3 python/watcher_log.py --variant B --status reported --epochs 3 --ppl 1.0812 --minutes 112.4
    python3 python/watcher_log.py --variant none --status silent

Output format (one line per poll event, tab-separated):
    2026-07-29T08:17:00Z  B  skipped  epochs=1  ppl=1.1471  minutes=41.2
    2026-07-29T08:25:00Z  B  reported  epochs=3  ppl=1.0812  minutes=112.4
    2026-07-29T08:25:00Z  none  silent  -  -  -

Log file: runs/watcher_poll.log (next to results.json files)
Changes file: runs/watcher_changes.log (only written on status=reported or status=all_complete)
Rotation: if watcher_poll.log exceeds 1MB, archive to watcher_poll.YYYY-MM-DD.log and start fresh.
"""

import argparse
import os
import datetime

RUNS_DIR = os.path.join(os.path.dirname(__file__), '..', 'runs')
POLL_LOG = os.path.join(RUNS_DIR, 'watcher_poll.log')
CHANGES_LOG = os.path.join(RUNS_DIR, 'watcher_changes.log')
MAX_BYTES = 1_000_000  # 1 MB rotation threshold

def rotate_if_needed(path):
    if os.path.exists(path) and os.path.getsize(path) > MAX_BYTES:
        date_str = datetime.datetime.utcnow().strftime('%Y-%m-%d')
        archive = path.replace('.log', f'.{date_str}.log')
        os.rename(path, archive)

def append_line(path, line):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    rotate_if_needed(path)
    with open(path, 'a') as f:
        f.write(line + '\n')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--variant', required=True, help='A, B, C, or none')
    parser.add_argument('--status', required=True,
                        choices=['silent', 'skipped', 'reported', 'all_complete'],
                        help='silent=nothing found, skipped=found but epochs!=3, reported=valid result posted, all_complete=all three done')
    parser.add_argument('--epochs', default='-')
    parser.add_argument('--ppl', default='-')
    parser.add_argument('--minutes', default='-')
    parser.add_argument('--note', default='')
    args = parser.parse_args()

    ts = datetime.datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')
    fields = [ts, args.variant, args.status,
              f'epochs={args.epochs}', f'ppl={args.ppl}', f'minutes={args.minutes}']
    if args.note:
        fields.append(f'note={args.note}')
    line = '\t'.join(fields)

    # Always append to poll log
    append_line(POLL_LOG, line)

    # Only append to changes log on meaningful state transitions
    if args.status in ('reported', 'all_complete'):
        append_line(CHANGES_LOG, line)

    print(line)

if __name__ == '__main__':
    main()
