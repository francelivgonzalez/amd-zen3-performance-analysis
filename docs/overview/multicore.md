# Multicore Scalability

Quintin Peterson

## Question

As threads are added, when does the EPYC 7V13 stop speeding up, and which shared
resource is responsible?

The current version compares a compute bound workload against a memory bound one, which 
tests if memory bandwidth is our limit. some other suspects that may limit it still need to be added and are listed in the TODO section.
(L3 / chiplet boundary, SMT, cache coherence)

## Design

Total work is fixed and split evenly across an n number of threads, so the ideal equation is
T(n) = T(1) / n. Any difference of ideal is the cost of sharing hardware.

 Metrics & Definition 

 T(n): median wall clock time of the specific timed parts. Use median of runs for skewed runs.
 Speedup: S(n) = T(1) / T(n), ideal = n
 Parallel efficiency: E(n) = S(n) / n (1.0 = perfect, measures the fraction of ideal)
 Throughput: work units / second
 Noise: if run has more than 5% difference between another run we rerun it

## Workloads (scaling.cpp)

 Workload , What it does , Hypothesis 

 compute: math in CPU registers and no memory traffic: perfect speedup up to the core count, because threads share almost nothing. (CONTROL GROUP BECAUSE THREADS SHARE ALMOST NOTHING)
 memory: reads a 2 GB array (must pull from RAM) 5 times: Speedup stops increasing early, once DRAM bandwidth is used up, before you reach 64 threads.

The difference between these measurments is the main idea. Both use the same threads and the
same timing, so the difference shows what memory bandwidth costs.

## Thread placement

Not controlled yet since the Linux scheduler decides which core each thread runs on, so
results can vary somewhat from run to run due to threads moving around.

## Procedure (for portability)

1. Log in to brooks and check uptime
2. run ./scripts/run_multicore.sh
   - Builds with g++ 
   - Each run does R = 5 timed repetitions
   - Threads are created and set up before the clock starts, then released together
   and the clock stops when the last thread finishes
   - Saves scaling.csv and system_info.txt (lscpu, per CPU layout, compiler, and load info)
3. Optional: run perf for hardware counters
4. The CSV will be used for data analysis

data columns: workload, threads, rep, seconds, work_units, throughput

## What could go wrong

- Shared server: other users add noise so i can check with uptime command
- Threads not fixed: the OS may move threads between cores or put two on the same core.
- Frequency boost: at high amount of cores the chip slows them down to limit temp and power limit
- VM restrictions: brooks could give access to less than 64 cores
- Compiler optimizations: each workload's result is written to a volatile variable which means the compiler can't delete it when trying to optimize.

## TODO

- Thread pinning to test the L3 and SMT.
- test the cost of keeping caches in sync between cores