"""Plot IPC for individual perf runs; requires matplotlib and raw CSV files."""
import csv
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

root = Path(__file__).resolve().parents[1]
runs, ipc = [], []
for path in sorted((root / 'results' / 'individual_runs').glob('run_*.csv'), key=lambda p: int(p.stem.split('_')[-1])):
    events = {}
    with path.open(newline='') as file:
        for row in csv.reader(file):
            if len(row) < 3 or row[0].startswith('#'):
                continue
            try:
                events[row[2].strip()] = float(row[0])
            except ValueError:
                continue
    if events.get('cycles', 0) > 0 and 'instructions' in events:
        runs.append(int(path.stem.split('_')[-1]))
        ipc.append(events['instructions'] / events['cycles'])
if not ipc:
    raise SystemExit('Raw run_*.csv data unavailable; cannot recreate original graph.')
mean = sum(ipc) / len(ipc)
plt.figure(figsize=(9,5))
plt.plot(runs, ipc, marker='o', label='Measured IPC')
plt.axhline(mean, linestyle='--', label=f'Mean IPC = {mean:.4f}')
plt.xlabel('Benchmark Run')
plt.ylabel('Instructions Per Cycle (IPC)')
plt.title('AMD EPYC 7V13 - IPC Across Diagnostic Runs')
plt.xticks(runs)
plt.grid(True, alpha=.3)
plt.legend()
plt.tight_layout()
output = root / 'results' / 'ipc_baseline.png'
plt.savefig(output, dpi=200)
plt.close()
print(f'Graph saved to {output}')
