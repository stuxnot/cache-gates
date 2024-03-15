## cache-gates

Implementation of logic gates using cache side effects presented in the paper [Optimization and Amplification of Cache Side Channel Signals](https://arxiv.org/abs/2303.00122) by David A. Kaplan.

Assumes a x86_64 cpu. The number of delay ops can currently neither be configured nor inferred, and must be hand-coded 
in src/gates.c. Timing of multiple cached vs non-cached accesses is measured and the mean used as the distinguisher
between a cached vs non-cached line.

Use at your own peril.