\# Benchmarking and Data Collection Standard



\*\*Project:\*\* Performance Edge Cases in AMD Zen Architecture  

\*\*Course:\*\* CS3339 Fall 2026  

\*\*Processor:\*\* AMD EPYC 7V13 (Zen 3)



\## 1. Overview



Since we are working on different parts of the project, we need to make sure everyone is collecting and organizing their results the same way. This will make it easier to combine our measurements and compare what is happening across the different experiments.



The project has three main experiments: cache capacity, sequential versus random memory access, and multicore scalability. Each experiment will have different workloads, but we should try to keep our testing process as consistent as possible.



The goal is to collect reliable measurements that we can use to identify where the processor starts losing performance and understand what might be causing it.



\## 2. Testing Environment



All of our official benchmarks will be run on the AMD EPYC 7V13 system available through `brooks.cs.txstate.edu`.



For consistency, we should follow these guidelines:



\- Write the benchmarks in C or C++.

\- Use Linux `perf` to collect performance measurements.

\- Compile with optimization enabled, starting with `-O3`.

\- Record the compiler version and compilation commands.

\- Keep the testing environment as consistent as possible between runs.

\- Document which CPU cores are being used when relevant.

\- Avoid running multiple project benchmarks at the same time.

\- Record anything unusual that might affect the results.



Since Brooks is a shared system, other users may be running programs at the same time. We should keep this in mind if we notice inconsistent results.



We will also need to check which `perf` measurements are available on Brooks before deciding which hardware counters we can collect.



\## 3. Running the Benchmarks



Each benchmark configuration should be tested multiple times instead of relying on one result.



For our project, we will use the following process:



1\. Run the benchmark at least once as a warm-up.

2\. Run the same configuration 10 times and record each result.

3\. Keep the workload consistent when comparing different configurations.

4\. Record the execution time and available performance counters.

5\. Make sure the program produces the expected output.

6\. Document any unusual results instead of deleting them without explanation.



We will use nanoseconds for execution time and bytes for dataset sizes.



Using multiple runs will help us calculate averages, compare variability, and determine whether the performance differences are consistent.



\## 4. CSV Data Format



Everyone should use the same column names when saving benchmark results.



The required CSV format is:



`experiment,run\_id,size\_bytes,access\_pattern,threads,operations,elapsed\_ns,cycles,instructions,cache\_references,cache\_misses`



| Column | Description |

|---|---|

| experiment | Type of experiment: cache, memory, or multicore |

| run\_id | Run number, from 1 to 10 |

| size\_bytes | Working-set size in bytes |

| access\_pattern | Sequential, random, pointer chase, or another documented access pattern |

| threads | Number of threads used |

| operations | Number of useful operations performed |

| elapsed\_ns | Execution time in nanoseconds |

| cycles | CPU cycles measured using perf |

| instructions | Retired instructions measured using perf |

| cache\_references | Cache references measured using perf |

| cache\_misses | Cache misses measured using perf |



Some measurements may not apply to every experiment or may not be available through `perf`. If this happens, leave the value empty and explain why in the experiment documentation.



We should not put zero for a measurement that was not collected.



\## 5. Cache Capacity Experiment



\*\*Responsible:\*\* Santiago



This experiment will test how performance changes as the working-set size increases.



The main idea is to gradually increase the amount of data being processed and look for noticeable changes when the data becomes too large for different levels of cache.



The experiment should:



\- Test different working-set sizes.

\- Include sizes smaller and larger than the processor's cache capacities.

\- Keep the workload consistent between comparable tests.

\- Measure execution time, cycles, instructions, and available cache events.

\- Include additional measurements around noticeable performance changes.

\- Save the results using our shared CSV format.



The results should help us determine whether changes in performance might be related to the processor's cache hierarchy.



\## 6. Memory Access Experiment



\*\*Responsible:\*\* Santiago



This experiment will compare sequential and random memory access.



Both versions should process approximately the same amount of data and perform comparable work. The main difference should be the order in which memory is accessed.



The experiment should:



\- Implement sequential and random memory access.

\- Test both patterns using the same dataset sizes.

\- Keep the amount of useful work comparable.

\- Record how the random order is generated.

\- Measure execution time, IPC, and available cache events.

\- Save the results using our shared CSV format.



We expect sequential access to generally perform better because it is easier for the processor to take advantage of caching and prefetching.



However, the conclusion will depend on the actual measurements.



\## 7. Multicore Scalability Experiment



\*\*Responsible:\*\* Quintin



This experiment will test how performance changes as more threads are added to the same workload.



The goal is to see when adding threads stops giving the same level of improvement.



The experiment should:



\- Test thread counts such as 1, 2, 4, and 8, with additional counts depending on the system.

\- Keep the total amount of work constant when measuring speedup.

\- Record how threads are assigned to CPU cores.

\- Measure execution time, throughput, cycles, instructions, and available cache events.

\- Calculate speedup and parallel efficiency.

\- Save the results using our shared CSV format.



The results should help us identify when additional threads are no longer improving performance as efficiently.



We will compare these changes with the processor's architecture and available performance measurements before deciding what might be causing them.



\## 8. Performance Calculations



After collecting the measurements, we will calculate several performance metrics.



\*\*Instructions per Cycle (IPC)\*\*



IPC = Instructions / CPU Cycles



\*\*Cache Miss Rate\*\*



Cache Miss Rate = Cache Misses / Cache References



This calculation should only be used when the two counters measure compatible types of events.



\*\*Throughput\*\*



Throughput = Operations / Execution Time in Seconds



\*\*Speedup\*\*



Speedup(N) = Execution Time with 1 Thread / Execution Time with N Threads



\*\*Parallel Efficiency\*\*



Parallel Efficiency(N) = Speedup(N) / N



These calculations will be used to compare performance across the experiments.



For multithreaded measurements, we will also need to consider whether performance counters represent one thread or multiple threads combined.



\## 9. Organizing the Results



To keep the repository organized, everyone should save their files in the appropriate folders.



| Folder | Contents |

|---|---|

| `benchmarks/cache/` | Cache benchmark code |

| `benchmarks/memory/` | Memory access benchmark code |

| `benchmarks/multicore/` | Multicore benchmark code |

| `data/raw/` | Original benchmark measurements |

| `data/processed/` | Cleaned and organized datasets |

| `analysis/` | Python analysis scripts |

| `results/figures/` | Graphs and visualizations |

| `results/tables/` | Calculations and summary tables |

| `docs/methodology/` | Testing procedures and documentation |



Raw measurements should be preserved so we can return to the original results if something looks incorrect.



\## 10. Documentation Requirements



Each person should document enough information for another team member to reproduce their experiment.



This includes:



\- Benchmark code and relevant Git commit.

\- Compilation commands and compiler version.

\- Commands used to run the benchmark.

\- Dataset sizes and workload parameters.

\- Number of threads, when applicable.

\- Date and time of testing.

\- Number of warm-up and measured runs.

\- Performance measurements collected.

\- Any errors, limitations, or unusual results.



We should also explain any changes made to the testing procedure.



\## 11. Checking the Results



Before submitting results, each person should verify that:



\- The benchmark runs correctly.

\- The required configurations have been tested.

\- Each configuration has 10 measured runs.

\- The CSV files use the correct column names.

\- The units are consistent.

\- There are no missing or duplicated runs without explanation.

\- Unexpected measurements have been investigated.



If a result looks unusual, we should rerun the experiment before making a conclusion.



\## 12. Combining Our Work



Once the benchmark results are collected, Franceli will organize the data, calculate performance metrics, and generate graphs using Python, Pandas, and Matplotlib.



We will compare execution time, IPC, cache behavior, throughput, speedup, and parallel efficiency to look for noticeable changes in performance.



The goal is not just to show which configuration ran faster. We want to connect the measurements back to the AMD Zen 3 architecture and explain what might be causing the performance differences.



All three team members will review the results and contribute to the final report and presentation.

