#Running an experiment

The general steps to running an experiment:

* (optional) Make a new set of test programs as described in [Experiment setup](#experiment-setup)
* `cd` into the directory containing the test case (such as one made in the previous step)
*  source the desired experiment setup to set the environment variables 
 for the experiment: `source path/to/setups/desiredSetupScript`
* run `./runGeneralExperiment.sh`

This should go through all the test###.c files in the directory and do a run with them. For example running with
`setups/crestConstrainersModeB.sh` setup should produce output like:

```
test2.c n:.004928441 c:.044299752 SUCCESS
test3.c n:.004836839 c:.064464305 SUCCESS
test1.c n:.003534087 c:.044893205 SUCCESS
test4.c n:.002067395 c:.069534427 SUCCESS
test5.c n:.002452458 c:.102617454 Fail
test7.c n:.002334033 c:.004921909 SUCCESS
```

##Interpreting the output and debugging
The first column of the output tells us which program was being used. `n:.002452458` means native run took
0.00245s, `c:.102617454` says crest run took 0.1s and the final column indicates weather the oracles detected a 
mismatch. In this case there seems to be a problem with test5.c, so we can further investigate the issue by 
running `./generalSingleExperiment test5.c` which performs the experiment just on `test5.c` and prints 
the difference oracles detected. To delve even deeper one can run `$COMPILE_AND_RUN_1 test5.c` to run the 
program just natively (in this case) or `$COMPILE_AND_RUN_2 test5.c` to run it with crest. In fact these
are the environment variables set by the setup script.


##Mode A experiments

Mode A experiments can be ran with these setups:

* `. setups/kleeModeA.sh` setups up the experiment for KLEE in Mode A
* `. setups/crestModeA.sh` setups up the experiment for Crest in Mode A
* `. setups/fuzzballModeA.sh` setups up the experiment for FuzzBALL in Mode A


##Mode B experiments

We've prepared experiments to be run in Mode B for KLEE, Crest and FuzzBALL. Unless otherwise specified these
use crash, output, function call and performance oracles.

* `. setups/kleeLTConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than constrainer 
* `. setups/kleeLTEConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than or equal constrainer 
* `. setups/kleeRangeConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than or equal constrainer 
* `. setups/crestConstrainersModeB.sh` sets up the experiment for Crest in Mode B with less than constrainer 
* `. setups/fuzzballConstrainersModeB.sh` sets up the experiment for FuzzBALL in Mode B with less than constrainer 


#Experiment setup

To generate a new batch of test cases run ```./SetupExperiment.sh <experimentName>``` which will
generate a new batch of experiments. To change the number of generated test cases change ```seq 100 | ...```
to a number more approirate to your case.

To change the csmith settings used for the experiemnt edit the ```utils/generateTestCase.sh``` where 
csmith options can be changed.


