Franceli Gonzalez - AMD Architecture & Data Analysis
CS3339 Fall 2026 | Performance Edge Cases in AMD Zen Architecture

Primary branch: feature/architecture-analysis
Primary directories: docs/architecture/, analysis/, data/processed/, results/figures/, results/tables/


Characterize the actual AMD EPYC 7V13 system on brooks.cs.txstate.edu, design a reproducible analysis workflow, and independently explain when performance changes and which evidence supports possible architectural bottlenecks. Do not claim a bottleneck solely because a curve changes.


1. Verify the hardware (before interpreting any results)
- [ ] Connect to Brooks and record the date, hostname, operating system, compiler version, and kernel (hostname, uname -a, gcc --version or g++ --version).
- [ ] Run lscpu and save its output; identify architecture, sockets, cores per socket, threads per core, NUMA nodes, CPU model, and cache topology as reported on this machine.
- [ ] Inspect lscpu -C (if supported) and /sys/devices/system/cpu/cpu0/cache/index*/ for cache sizes, types, line sizes and sharing information. Note that L3 is shared across groups of cores, so a whole-socket L3 total is not necessarily available to one thread.
- [ ] Check perf --version and whether perf stat works on the accessible system. If permission or event support is limited, document which measurements are unavailable; do not invent replacements.
- [ ] Inspect whether the host is shared and document potential noise (other users, background load, clock variation, affinity, NUMA placement). Avoid disruptive tests.
- [ ] Write docs/architecture/epyc_7v13_system.md with sourced architectural background and separate direct observations from Brooks. Label verified vs inferred details.


2. Agree on an experiment data contract with the team
- [ ] Define a shared CSV schema before large-scale collection so datasets can be combined without manual guesswork.
- [ ] Minimum columns: experiment, run_id, timestamp, hostname, cpu_model, compiler, compiler_flags, threads, working_set_bytes, access_pattern, operations, elapsed_seconds, cycles, instructions, cache_references, cache_misses, notes.
- [ ] Use blank/NA values for unavailable counters; do not treat unsupported events as zero.
- [ ] Specify units and whether counters refer to the entire process, one CPU, or selected threads. Record both configuration and run identifiers.
- [ ] Agree on warm-up, number of repeats (for example 5 or more when feasible), fixed problem size vs per-thread size, pinning policy, and compiler optimization. Document choices and any deviations.
- [ ] Store immutable original measurements in data/raw/; put derived tables in data/processed/. Do not edit raw CSVs to make results cleaner.



3. Develop the analysis yourself
- [ ] Build your own Python scripts in analysis/ to validate column names/types, detect missing measurements, flag suspicious runs, and group repeated runs.
- [ ] Compute IPC = instructions / cycles only when both counters are valid, from the same scope, and cycles > 0.
- [ ] Compute cache-miss rate = cache_misses / cache_references only for the measured counter definition and valid positive denominator. Explain that generic perf cache-misses is not automatically L1/L2/L3-specific.
- [ ] Prefer normalized time such as ns/access, and throughput such as accesses/s or bytes/s where workload semantics make this meaningful; retain total execution time as well.
- [ ] For multicore scaling, compute speedup = T1 / Tp and parallel efficiency = speedup / p only when each run completes the same total work and has comparable configuration.
- [ ] Summarize repeats with median and spread (range, standard deviation, or IQR); flag run-to-run variability. Consider confidence intervals if supported by repeat counts.
- [ ] Build and review graphs: working-set size vs normalized latency; working-set size vs relevant misses; sequential vs random access; thread count vs actual and ideal speedup; thread count vs parallel efficiency. Annotate known cache sizes as reference points, not predetermined cliffs.
- [ ] Compare observed breakpoints to cache sharing, prefetch behavior, TLB effects, bandwidth pressure, NUMA layout, and SMT; mark hypotheses that require further tests.



4. Integrate Santiago's and Quintin's results
- [ ] Review their code and the exact command line for each run before accepting comparisons.
- [ ] Ensure that experiments vary one main factor at a time, that compilers did not optimize the workload away, and that test lengths are long enough to measure reliably.
- [ ] Ask for reruns of anomalies and log the reason, never silently discard outliers.
- [ ] Distinguish causal evidence from correlations: a timing cliff plus generic cache misses may suggest but does not prove an L2 or L3 capacity miss.
- [ ] Record environmental confounders and counter availability in docs/methodology/measurement_notes.md.



5. Technical report and presentation
- [ ] Write the hardware and analysis-method sections in your own words, citing external references used.
- [ ] Build concise figure captions stating metric, units, configuration, repetitions and main finding.
- [ ] Give the team an evidence-based summary: strongest result, limitation, alternative explanation, proposed follow-up.
- [ ] Prepare your presentation segment and rehearse explaining the difference between IPC, miss rate, throughput, speedup, and efficiency.
- [ ] Keep the AI disclosure current and accurate for any assistance you actually used; the course prohibits AI-generated full data analyses and fabricated results.


Required deliverables
- docs/architecture/epyc_7v13_system.md
- docs/methodology/measurement_notes.md and shared CSV field definitions
- Student-written scripts under analysis/
- Original data preserved under data/raw/, derived tables under data/processed/
- Figures under results/figures/ and summary tables under results/tables/
- Your contributions to the final conference-style report and 8-minute team presentation


Definition of done
All figures can be regenerated from preserved original measurements; calculations can be explained; hardware details are verified on Brooks; conclusions distinguish observations from plausible explanations; all work is committed and pushed on your feature branch with a pull request for review.