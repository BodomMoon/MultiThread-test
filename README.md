# MultiThread-test

Compile with this commond

g++ -std=c++11 -pthread

Test with

perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-prefetches,L1-dcache-prefetch-misses,LLC-loads,LLC-load-misses,LLC-prefetches,LLC-prefetch-misses ./FileName
