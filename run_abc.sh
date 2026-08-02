#!/usr/bin/env bash
# run_abc.sh — run A, B, C training serially from the maith-repo sandbox.
# Each variant is verified (epochs==3, no smoke_test) before the next starts.
# Run from: ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
EXPECTED_DIR="/Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo"

echo "=== Maith A/B/C serial training run ==="
echo "Working directory: $SCRIPT_DIR"

if [ "$SCRIPT_DIR" != "$EXPECTED_DIR" ]; then
  echo "ERROR: This script must be run from $EXPECTED_DIR"
  echo "       Currently in: $SCRIPT_DIR"
  exit 1
fi

run_variant() {
  VARIANT=$1
  echo ""
  echo "--- Variant $VARIANT ---"
  echo "pwd: $(pwd)"
  echo "Starting at: $(date)"
  python3 python/train.py --variant "$VARIANT" --datasets datasets/
  RESULT="runs/variant_$VARIANT/results.json"
  if [ ! -f "$RESULT" ]; then
    echo "ERROR: $RESULT not found after training. Aborting."
    exit 1
  fi
  EPOCHS=$(python3 -c "import json; d=json.load(open('$RESULT')); print(d.get('epochs','?'))")
  SMOKE=$(python3 -c "import json; d=json.load(open('$RESULT')); print(d.get('smoke_test','?'))")
  PPL=$(python3 -c "import json; d=json.load(open('$RESULT')); print(d.get('eval_perplexity','?'))")
  echo "Variant $VARIANT complete: epochs=$EPOCHS smoke_test=$SMOKE perplexity=$PPL"
  if [ "$EPOCHS" != "3" ]; then
    echo "ERROR: Expected 3 epochs, got $EPOCHS. Aborting before next variant."
    exit 1
  fi
  if [ "$SMOKE" = "True" ]; then
    echo "ERROR: smoke_test=True in results. Aborting before next variant."
    exit 1
  fi
  echo "Variant $VARIANT verified OK."
}

run_variant A
run_variant B
run_variant C

echo ""
echo "=== All three variants complete ==="
python3 -c "
import json
for v in ['A','B','C']:
    d = json.load(open(f'runs/variant_{v}/results.json'))
    print(f'  {v}: perplexity={d[\"eval_perplexity\"]} epochs={d[\"epochs\"]} time={d[\"training_minutes\"]}min')
"
echo ""
echo "Next step: python3 python/eval_completion.py --checkpoint-A runs/variant_A --samples 200 --mask-last 10"
