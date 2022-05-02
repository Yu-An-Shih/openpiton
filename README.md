# Integrating Victim Cache into OpenPiton to Reduce Conflict Misses

This is a course project of the ECE 575 Computer Architecture course in Princeton University. We integrated a victim cache into the OpenPiton research framework. This repository is forked from [https://github.com/PrincetonUniversity/openpiton.git](https://github.com/PrincetonUniversity/openpiton.git).

The master branch is the baseline without victim cache, and the victim_cache branch contains our integrated victim cache design. Most changes can be found in ```piton/design/chip/tile/l15/rtl/```, including the added victim cache and modified L1.5 cache control logic. The c test files are in ```piton/verif/diag/c/riscv/ariane/```.

#### Environment Setup
- Run ```source piton/ariane_setup.sh``` to setup the environment.

==========================

#### Building a simulation model
1. ```cd $PITON_ROOT/build```
2. ```sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_build -ariane``` builds a single tile OpenPiton simulation model with the Ariane core.

==========================

#### Running the self-generated tests
1. ```cd $PITON_ROOT/build```
2. Run ```sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_run -ariane <self-generated test> -gcc_args '-O0'``` for our self-generated simple demonstrative test cases.

> Example: ```sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_run -ariane vc_load_conflict.c -gcc_args '-O0'```

==========================

#### Running the self-generated tests
1. ```cd $PITON_ROOT/build```
2. Run ```sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_run -ariane <benchmark>``` for the original and later added benchmarks.

> Example: ```sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_run -ariane hello_world.c```

==========================

#### Configuring the victim cache
To test the victim cache with different sizes, go to piton/design/include/l15.h.pyv and modify ```vc_numentries``` on line 149. Rebuild simulation models and run tests.

If you encounter any issue, please contact yashih@princeton.edu or hongjiewang@princeton.edu.