#!/usr/bin/env bash
# run_v2_resume.sh — launch/resume Variant A v2 training robustly.
#
# Fixes the two things that sank the prior attempt:
#   1. Mid-epoch stalls left no checkpoint (save_strategy was "epoch").
#      train_v2_resume.py checkpoints every ~half-epoch, so --resume works.
#   2. The process died on SSH disconnect / Mac sleep.
#      We wrap in `caffeinate -i` (prevent idle sleep) + nohup + disown,
#      write to a repo-root log (~/maith-repo is writable; /tmp is not), and
#      record the PID.
#
# Usage:
#   bash Scripts/run_v2_resume.sh start   # fresh run
#   bash Scripts/run_v2_resume.sh resume  # resume from latest checkpoint
#   bash Scripts/run_v2_resume.sh status  # show process + last log lines
#
# NOTE on python: the repo's default `python3` (/usr/bin) has no torch.
# Use the framework python that has torch 2.13 + transformers.
set -euo pipefail

REPO="$HOME/maith-repo"
PY="/Library/Frameworks/Python.framework/Versions/3.14/bin/python3"
OUT="runs/variant_A_v2_full"
LOG="$REPO/run_A_v2_full.log"
PIDFILE="$REPO/run_A_v2_full.pid"

cd "$REPO"

case "${1:-status}" in
  start)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
      echo "Already running (PID $(cat "$PIDFILE")). Use 'resume' only after it stops."; exit 1
    fi
    rm -rf "$OUT"                 # fresh start: clear any stale/partial state
    caffeinate -i nohup "$PY" python/train_v2_resume.py \
      --variant A --datasets datasets/ --out "$OUT" > "$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    disown
    echo "Started fresh v2 A training. PID $(cat "$PIDFILE")"
    echo "Log: $LOG"
    ;;
  resume)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
      echo "Already running (PID $(cat "$PIDFILE"))."; exit 1
    fi
    if ! ls "$OUT/_trainer_tmp/checkpoint-"* >/dev/null 2>&1; then
      echo "No checkpoint in $OUT to resume from. Use 'start'."; exit 1
    fi
    caffeinate -i nohup "$PY" python/train_v2_resume.py \
      --variant A --datasets datasets/ --out "$OUT" --resume >> "$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    disown
    echo "Resumed v2 A training from latest checkpoint. PID $(cat "$PIDFILE")"
    echo "Log: $LOG"
    ;;
  status)
    if [ -f "$PIDFILE" ] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
      echo "RUNNING (PID $(cat "$PIDFILE"), elapsed $(ps -o etime= -p "$(cat "$PIDFILE")" | tr -d ' '))"
    else
      echo "NOT RUNNING"
      [ -f "$PIDFILE" ] && echo "(stale pidfile removed)" && rm -f "$PIDFILE"
    fi
    echo "--- checkpoints ---"
    ls -d "$OUT/_trainer_tmp/checkpoint-"* 2>/dev/null || echo "(none yet)"
    echo "--- results ---"
    [ -f "$OUT/results.json" ] && cat "$OUT/results.json" || echo "(no results.json yet)"
    echo "--- last 15 log lines ---"
    tail -15 "$LOG" 2>/dev/null || echo "(no log)"
    ;;
  *)
    echo "usage: $0 {start|resume|status}" >&2; exit 2
    ;;
esac
