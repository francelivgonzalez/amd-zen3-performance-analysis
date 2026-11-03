\** Performance Edge Cases in AMD Zen Architecture**



Experimental analysis of cache behavior, memory access patterns, and multicore scalability on AMD Zen 3 processors.



\## **Project Overview**



This project investigates when AMD Zen-based processors experience performance degradation or reduced scaling efficiency under controlled workloads.



Experiments are conducted on an AMD EPYC 7V13 (Zen 3) system using C/C++ microbenchmarks and Linux perf.



\##** Research Question**



Under what workloads does an AMD Zen-based processor start losing performance or stop scaling efficiently, and what architectural resources contribute to these limitations?



\## **Experiments**



\### 1. Cache Capacity

Evaluate execution time, CPU cycles, IPC, and cache behavior across different working-set sizes.



\### 2. Memory Access Patterns

Compare sequential and random memory access while maintaining similar amounts of computational work.



\### 3. Multicore Scalability

Measure execution time, throughput, speedup, and parallel efficiency across different thread counts.



\## Hardware



\- \*\*Processor:\*\* AMD EPYC 7V13

\- \*\*Microarchitecture:\*\* AMD Zen 3

\- \*\*System:\*\* brooks.cs.txstate.edu

\- \*\*Operating System:\*\* Linux

\- \*\*Performance Monitoring:\*\* Linux perf



Exact CPU topology and cache specifications will be verified on the testing system.



\## Technologies



\- C / C++

\- Python

\- Pandas

\- NumPy

\- Matplotlib

\- Linux perf

\- Git



\## Repository Structure



\- `benchmarks/cache/` — Cache-capacity experiments

\- `benchmarks/memory/` — Memory access benchmarks

\- `benchmarks/multicore/` — Multithreaded scalability tests

\- `analysis/` — Python performance analysis

\- `data/raw/` — Original benchmark measurements

\- `data/processed/` — Cleaned experimental data

\- `results/figures/` — Performance visualizations

\- `results/tables/` — Summary tables

\- `docs/architecture/` — AMD Zen 3 architecture research

\- `docs/methodology/` — Experimental procedures

\- `scripts/` — Benchmark execution and automation scripts



\## Team



\*\*Franceli Gonzalez — AMD Architecture and Data Analysis\*\*



Research processor architecture, process benchmark data, calculate performance metrics, generate visualizations, and interpret bottlenecks.



\*\*Santiago Bolaños — Cache and Memory Testing\*\*



Develop and run cache-capacity and memory-access benchmarks.



\*\*Quintin Peterson — Multicore Testing\*\*



Develop and run multicore scalability benchmarks.



All team members contribute to the final report and presentation.



\## Deliverables



\- C/C++ benchmark implementations

\- Benchmark commands and scripts

\- Raw and processed experimental data

\- Performance graphs and calculations

\- Written technical report

\- Eight-minute in-class presentation



\## Status



Project setup and experimental development.

