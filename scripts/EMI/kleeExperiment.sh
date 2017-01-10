#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENTER=$GH_PROJECT_ROOT/scripts/EMI/deadConditionInject.sh
COMPILE_AND_RUN=$GH_PROJECT_ROOT/scripts/EMI/kleeCompileAndRun.sh
ls | grep -P "test\d+\.c" | sort -V | xargs -L1  $DIR_NAME/singleEMIExperiment.sh

