Quintin Peterson - Multicore Scalability Benchmark
CS3339 Fall 2026 | Performance Edge Cases in AMD Zen Architecture


Primary branch: feature/multicore
Primary directories: benchmarks/multicore/, scripts/, data/raw/, docs/methodology/


Implement and run a controlled C/C++ multicore benchmark on AMD EPYC 7V13 (brooks.cs.txstate.edu) to determine where additional threads stop providing proportionate speedup and to investigate plausible sources of contention.


1. Define a workload that can be compared fairly
- [ ] Read the proposal and agree with Franceli on the CSV schema, exact measurement definitions and target thread counts.
- [ ] Implement your own thread-based benchmark under benchmarks/multicore/ using the course-approved language/library (for example C++ threads or OpenMP if permitted).
- [ ] Choose a repeatable workload with useful computation and controlled memory behavior. State whether the experiment is compute-bound, memory-bound or deliberately mixes the two.
- [ ] Use the same total problem size for thread counts 1, 2, 4, 8 and additional values supported/allowed on the host. Divide work among threads; verify all tasks actually complete.
- [ ] Do not measure thread creation or data initialization accidentally as the primary workload unless explicitly studying those costs. Document the timing boundary.
- [ ] Avoid unintentional data races and false sharing; if demonstrating contention intentionally, clearly label that as a separate variant.
- [ ] Make sure optimizing compilers cannot eliminate the computation; validate a checksum or other deterministic result for every thread count.




2. Design a meaningful scaling series
- [ ] Verify hardware topology using lscpu: physical cores, SMT threads, sockets, NUMA nodes and shared caches.
- [ ] Choose tested thread counts with rationale; test physically distinct cores before or in addition to SMT siblings when feasible.
- [ ] Document CPU affinity and placement. A thread-count increase may cross a socket or NUMA boundary, changing the meaning of the comparison.
- [ ] Distinguish strong scaling (fixed total work; suitable for T1/Tp speedup) from weak scaling (work per thread fixed). If both are tested, report them separately.
- [ ] Run each configuration several times under comparable conditions; use median and variability rather than only fastest run.
- [ ] Measure wall-clock time and derive throughput = completed operations / elapsed seconds where meaningful.
- [ ] Calculate speedup = T1 / Tp and parallel efficiency = speedup / p for strong scaling. Ideal speedup is p, but measured speedup can differ because of overhead, contention, noise and frequency behavior.








3. Collect performance measurements
- [ ] Check perf --version and benchmark perf stat permissions; record failures or unsupported events honestly.
- [ ] When available, collect CPU cycles, instructions, IPC and cache references/misses with precise event semantics and scope. Note that aggregate IPC across threads requires compatible instruction/cycle aggregation and cannot be compared blindly with per-core IPC.
- [ ] Consider more advanced events (memory bandwidth, LLC events, context switches, CPU migrations) only if verified available and meaningful on Brooks.
- [ ] Record compile flags, CPU placement, NUMA behavior, repeats, elapsed time, operations, thread count and timestamps for every run.
- [ ] Investigate whether the workload becomes limited by memory bandwidth, shared-cache contention, synchronization, SMT, NUMA locality or serial work. Do not claim that perf alone proves a particular cause.
- [ ] Re-run unexpected regressions; document anomalies rather than deleting them.





4. Results and interpretation
- [ ] Produce raw per-run measurements for Franceli; do not replace raw data with only averaged results.
- [ ] Identify the thread count at which the efficiency curve begins to decline, if there is a reproducible decline.
- [ ] Explain whether performance plateaus, degrades, or simply grows sublinearly; these are different findings.
- [ ] Compare changes in speedup with relevant measurement evidence and the physical-core/SMT/topology boundaries.
- [ ] If possible, perform a small diagnostic follow-up (for example change affinity or working-set size) to test the leading bottleneck hypothesis.









5. Documentation and hand-off
- [ ] Create docs/methodology/multicore_experiment.md with workload definition, threading model, timing boundaries, compiler flags, affinity/topology and repeat policy.
- [ ] Supply compile/run commands or repeatable scripts and preserve your measured outputs in data/raw/.
- [ ] Define the output field meanings and units so Franceli can compute throughput, speedup and efficiency accurately.
- [ ] Collaborate on the multicore results/report section and participate in the in-class 8-minute team presentation.






Required deliverables
- Student-authored source under benchmarks/multicore/
- Repeatable run/compile instructions and verified perf commands
- docs/methodology/multicore_experiment.md
- Raw data with runs at multiple thread counts and repeated trials
- Brief findings + limitations and your portion of the final report/presentation





Definition of done
Another person can build and run the program and reproduce the thread-scaling series; correctness and comparable total work are verified; speedup/efficiency have correct definitions; thread placement and hardware topology are documented; conclusions are tied to measured evidence; your branch is pushed and reviewed through a pull request.