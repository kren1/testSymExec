#Running an experiment

The general steps to running an experiment:

* Make a new set of test programs as described in [Experiment setup](#experiment-setup) (or use a previously generated one)
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

* `. setups/kleeModeA.sh` sets up the experiment for KLEE in Mode A
* `. setups/crestModeA.sh` sets up the experiment for Crest in Mode A
* `. setups/fuzzballModeA.sh` sets up the experiment for FuzzBALL in Mode A


##Mode B experiments

We've prepared experiments to be run in Mode B for KLEE, Crest and FuzzBALL. Unless otherwise specified these
use crash, output, function call and performance oracles.

* `. setups/kleeLTConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than constrainer 
* `. setups/kleeLTEConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than or equal constrainer 
* `. setups/kleeRangeConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with less than or equal constrainer 
* `. setups/kleeDivisorConstrainerModeB.sh` sets up the experiment for KLLE in Mode B with the divisor constrainer
* `. setups/crestConstrainersModeB.sh` sets up the experiment for Crest in Mode B with less than constrainer 
* `. setups/fuzzballConstrainersModeB.sh` sets up the experiment for FuzzBALL in Mode B with less than constrainer 

### Mode B - coverage

An example of a coverage oracle being used for crest can be found by `. setups/crestCoverageModeB.sh`

##Mode C experiments

An example of Mode C implementation for Crest can be set up by `. setups/modeCCrestSetup.sh`

#Experiment setup

To generate a new batch of test cases run ```./SetupExperiment.sh <experimentName>``` which will
generate a new batch of experiments. To change the number of generated test cases change ```seq 100 | ...```
to a number more approirate to your case.

To change the csmith settings used for the experiemnt edit the ```utils/generateTestCase.sh``` where 
csmith options can be changed.

#Extending to new symbolic execution engines

All the complexity of compiling, running and settting up oracles is pushed into `compileAndRunScripts`. To leverage this infrastacture for another symbolic execution engines, one needs to only write the compileAndRunScripts (which included writting the constrainer library). 


## Compile and Run script contract

A compile and run script is given two arguments:
* Location of the source code file that is meant to be compiled and run
* Original name of the file (This is used for function call oracle and passing data around. It can be safely ignored if not needed)

The script should then compile this file, link it with the appropriate constraining and/or instrumentation library and run it using the desired symbolic execution engine. The stdout of this script should contain all the data needed to compare this run with the one of the complimentary script. The output should have a deterministic order. For example first part could be output of the run, second part function call chains and last part coverage information.


The stderr of this script is ignored and can be used for debugging purposes. 

## Example 

To adapt the infrastcutre for a hypotetical symbolic executor AwesomeSE using crash, output and function call oracles we first need to implement the linked in library methods:

* `void symbolize_and_constrain_u(uint32_t *var, int size, uint32_t value, char* name)`: constrain the memory pointed to by `var` of size `size` (1,2,4 bytes) to `value`
* `void print_symbolic(const char* name, int64_t *val, char size)`: print symbolic variable of size `size` (8,16,32 bits) with the specified name
* ```logFunction(char* name)```: log the function name. Can be a noop if function call oracle is not desired. 

Inspiration for the implementation can be found in `testSymExec/instrument_lib`.

Now by writing a compile and run script that compiles the file, links it with the above implemntation of the methods and pipes output to `stdout` and then prints out the function calls to `stdout` we are done.

We can set the enviroment variables as `COMPILE_AND_RUN_1=CaR/nativeCompileAndRun.sh` and `COMPILE_AND_RUN_2=CaR/AwsomeSECompileAndRun.sh` and run the experiments.


Note that oracles can freely change if we also implemnt them in the native version.
