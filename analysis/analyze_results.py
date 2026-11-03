"""Read aggregated Linux perf CSV and report collected counters and derived IPC."""
import csv
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
input_file = ROOT / 'results' / 'baseline_perf.csv'
output_file = ROOT / 'results' / 'baseline_summary.json'

events = {}
with input_file.open(newline='') as file:
    for row in csv.reader(file):
        if len(row) < 3 or row[0].startswith('#'):
            continue
        try:
            events[row[2].strip()] = float(row[0])
        except ValueError:
            continue

cycles = events.get('cycles')
instructions = events.get('instructions')
ipc = instructions / cycles if cycles and instructions is not None else None
print('AMD Zen 3 Performance Analysis')
print('--------------------------------')
for name in ('cycles', 'instructions'):
    if name in events:
        print(f'{name.title()}: {events[name]:,.0f}')
print(f'Calculated IPC: {ipc:.4f}' if ipc is not None else 'IPC unavailable')
summary = {'processor': 'AMD EPYC 7V13', 'cycles': cycles,
           'instructions': instructions, 'calculated_ipc': ipc,
           'source': '10-run aggregated perf stat CSV',
           'caution': 'ratio of aggregate counter means differs from average per-run IPC'}
output_file.write_text(json.dumps(summary, indent=2) + '\n')
print(f'Results saved to {output_file}')
