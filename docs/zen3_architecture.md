# AMD Zen 3 Architecture — Brooks Verification

**Measured host:** `brooks.cs.txstate.edu` (Texas State University), November 3, 2026.

## Observed hardware

- **Processor:** 2 × AMD EPYC 7V13 (Zen 3 / Milan), 64 cores per socket; 128 logical CPUs visible; 1 thread per core.
- **Operating system:** Ubuntu 22.04.5 LTS; Linux kernel shown at login: 5.15.0-198-generic.
- **L1d:** 32 KiB per core, 128 instances, 4 MiB total.
- **L1i:** 32 KiB per core, 128 instances, 4 MiB total.
- **L2:** 512 KiB per core, 128 instances, 64 MiB total.
- **L3:** 32 MiB per instance, 16 instances, 512 MiB total (not a unified 512 MiB cache).
- **Cache line:** 64 bytes.
- **NUMA:** node 0 CPUs 0–63, node 1 CPUs 64–127; reported node distance 10 (local) versus 32 (remote). Relative distance values are not latency in nanoseconds.
- **Cache sharing:** output showed CPUs 0–7 share L3 #0, 8–15 L3 #1, 16–23 L3 #2.

## Profiling validation

- `perf version 5.15.216`; `perf_event_paranoid = -1`.
- `perf stat` successfully measured cycles, retired instructions, cache-references, cache-misses, L1-dcache-loads, and L1-dcache-load-misses during a short Python diagnostic.
- The six-event test had multiplexing; use fewer simultaneous counters for accurate comparisons.
- `numactl`, `taskset`, `gcc`, and `python3` are available.
- 10 repeat runs pinned to CPU 0 were collected with cycles/instructions; the individual CSV source files remain on Brooks.

## Project analysis plan

- **A: Cache capacity:** measure latency/bandwidth across increasing working-set sizes, explicitly controlling CPU affinity and access patterns.
- **B: Memory access:** compare sequential versus randomized access under controlled working-set conditions and NUMA placement.
- **C: Multicore scaling:** measure runtime, speedup, and efficiency while controlling placement, workload, and contention.
- Collect individual measurements, track run-to-run variability, and do not infer per-level cache misses from generic `cache-misses` alone.

## Provenance

This document is reconstructed from terminal output captured in the project conversation. Exact raw `lscpu` dumps and benchmark CSV samples should be imported from Brooks if needed. No synthetic observations are presented as measurements.
