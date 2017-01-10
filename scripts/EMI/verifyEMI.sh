#!/bin/bash
INSTRUMENTER=~/projects/testSymExec/scripts/EMI/deadConditionInject.sh
COMPILE_AND_RUN=~/projects/testSymExec/scripts/EMI/nativeCompileAndRun.sh
DIR_NAME=$(dirname "$(realpath $0)")
seq 10 | xargs -L1  $DIR_NAME/singleEMIExperiment.sh

