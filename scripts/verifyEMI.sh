#!/bin/bash
INSTRUMENTER=~/projects/testSymExec/scripts/deadConditionInject.sh
COMPILE_AND_RUN=~/projects/testSymExec/scripts/nativeCompileAndRun.sh
DIR_NAME=$(dirname "$(realpath $0)")
seq 1000 | xargs -L1  $DIR_NAME/singleEMIExperiment.sh

