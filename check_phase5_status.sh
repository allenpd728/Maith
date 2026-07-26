#!/bin/bash
# Quick Phase 5 status check

cd /Users/philipallen/Projects/Maith

echo "=== Phase 5 Training Status ==="
echo ""
python3 python/watch_full_runs.py --log runs/full_abc_runs.log 2>&1
echo ""
echo "=== Last 3 lines of log ==="
tail -3 runs/full_abc_runs.log
echo ""
echo "=== Completed runs ==="
for v in A B C; do
  if [ -f "runs/variant_$v/results.json" ]; then
    ppl=$(jq '.eval_perplexity' runs/variant_$v/results.json)
    time=$(jq '.training_minutes' runs/variant_$v/results.json)
    echo "Variant $v: ✅ Complete | Perplexity: $ppl | Time: ${time}m"
  else
    echo "Variant $v: ⏳ Running"
  fi
done
