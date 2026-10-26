Santiago Bolaños - Cache & Memory Benchmarks
CS3339 Fall 2026 | Performance Edge Cases in AMD Zen Architecture

Primary branch: feature/cache-memory
Primary directories: benchmarks/cache/, benchmarks/memory/, scripts/, data/raw/, docs/methodology/


Develop and test two separate, controlled C/C++ microbenchmarks on the AMD EPYC 7V13 system at brooks.cs.txstate.edu: (1) cache working-set capacity and (2) sequential versus random memory access. Supply trustworthy measurements and clearly documented procedures to the team.


1. Understand constraints and coordinate before coding
- [ ] Read the proposal and the shared measurement schema from Franceli. Confirm compiler, compiler flags, process pinning, iterations, repetitions and CSV column names.
- [ ] Get confirmed Brooks access and verify whether perf stat is allowed (perf --version; attempt a small harmless test). Record unavailable counters rather than substituting made-up values.
- [ ] Read the hardware notes to understand which cache sizes are per core, which are shared, and which may be aggregate totals. Do not assume a single core can use all reported system-wide L3 capacity.
- [ ] Work on feature/cache-memory; communicate major changes to the team before merging.


2. Experiment A — cache working-set size
Research question: At what working-set sizes does access cost change, and do performance counters support a cache-capacity interpretation?
- [ ] Implement your own benchmark under benchmarks/cache/, with clear arguments for working-set size, repetitions/iterations, and output name.
- [ ] Choose a repeatable read/write or pointer-chasing operation; document what it measures and why. Consider a dependency chain if trying to expose memory latency rather than bandwidth/prefetch benefits.
- [ ] Test a sweep of sizes well below, around, and above the observed L1D/L2/L3 reference capacities, including enough intermediate sizes to reveal gradual rather than only abrupt effects.
- [ ] Keep the number of measured accesses comparable across sizes when the question is per-access latency. Explicitly record any choice to instead fix number of passes; this changes total work.
- [ ] Allocate and initialize the data before the timed phase; document whether runs are warm-cache or cold-cache and whether warm-up occurs.
- [ ] Guard against compiler dead-code elimination (consume a result/checksum) without adding dominating output or I/O inside timed loops.
- [ ] Capture time plus available perf stat counters such as cycles and instructions. Record generic cache references/misses only if supported; use cache-level-specific events only after validating that the exact event is supported and what it means on this CPU.
- [ ] Test repeated runs under stable conditions; document compiler flags, CPU affinity, timing method and obvious measurement noise.



3. Experiment B — sequential vs random access
Research question: How does access order affect execution cost when amount of work and data are held as constant as practical?
- [ ] Implement your own benchmark under benchmarks/memory/ with both patterns over the same-size data and same number of meaningful accesses.
- [ ] Choose a random pattern appropriate to the question: a reproducibly shuffled permutation/pointer chain is better than calling an RNG inside the timed loop if you want memory behavior rather than RNG overhead.
- [ ] Use a fixed documented random seed. Build the sequence outside the timed region and record initialization/ordering overhead separately.
- [ ] Keep element type, operations per access, allocation, total working set and compiler configuration comparable. Document where sequential and random cases cannot be perfectly identical.
- [ ] Evaluate several working-set sizes, not just one; compare where data fits within cache against memory-sized data.
- [ ] Collect time, instruction/cycle counters and applicable cache-related counters across repeated runs; report per-access latency/throughput as well as wall-clock duration.
- [ ] Interpret cautiously: hardware prefetching, TLB behavior, branch prediction, memory-level parallelism and bandwidth can also influence results.



4. Measurement and reproducibility checklist
- [ ] Record each run ID, timestamp, hostname, CPU model, compiler version and flags, working-set bytes, access pattern, operations count, elapsed time, counters and thread count (normally 1 for these tests).
- [ ] Document exactly which CPU core(s) were used and whether thread affinity was fixed (taskset only where permitted and appropriate).
- [ ] Use a meaningful run length and several repeats (aim for at least 5 if practical); do not report a single lucky execution.
- [ ] Check process-wide vs per-CPU counter scope. Mark unsupported/permission-denied events as missing; don't report 0.
- [ ] Do not fabricate results, strip inconvenient results, or alter original logs. Investigate and document anomalous measurements.
- [ ] Save raw measurement files to data/raw/; if large, coordinate a data-storage policy before committing. Provide a short README describing schema and reproduction steps.



5. Hand-off to Franceli
- [ ] Provide source code, compile commands and exact run commands or a reproducible shell script under scripts/.
- [ ] Provide raw observations with units and run IDs—not just graphs or summarized conclusions.
- [ ] Identify likely performance changes and offer hypotheses, explaining what additional evidence would be needed to test them.
- [ ] Write docs/methodology/cache_memory_experiments.md describing design, controls, limitations and counters.
- [ ] Review graphs and analysis with Franceli to confirm that axis meanings and test configurations match the code.



Required deliverables
- Student-authored source under benchmarks/cache/ and benchmarks/memory/
- Compilation/run instructions, including perf stat examples validated on Brooks
- docs/methodology/cache_memory_experiments.md
- Repeated raw measurement files under data/raw/, with no invented values
- Your section of the final report and contribution to the 8-minute team presentation


Definition of done
Another team member can compile and rerun both experiments using your written instructions; measured work is comparable; raw data includes repeated runs and environment details; any proposed cache threshold is argued from results rather than assumed; your branch is pushed and reviewed through a pull request.