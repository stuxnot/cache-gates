## cache-gates

Implementation of logic gates using cache side effects presented in the paper [Optimization and Amplification of Cache Side Channel Signals](https://arxiv.org/abs/2303.00122) by David A. Kaplan.

Only works on x86_64 cpus. The number of delay ops can be configured in `src/gates.c` via `#define DELAY_OPS "<ops>"`. Timing of multiple cached vs non-cached accesses is measured and the mean used as the distinguisher
between a cached vs non-cached line.

Use at your own peril.

### TODO
- [ ] evaluation function which reports on overall performance of gates
- [ ] evaluation function which tests gates for different number of delay ops
      (maybe auto generate the assembly functions for ops 3-8)
