#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
NULL=/dev/null
source $DIR_NAME/../settings.sh
source $GH_PROJECT_ROOT/scripts/EMI/setupKleeEMI.sh

EXPERIMENT_LOG=${PWD##*/}.log
export EXPERIMENT_LOG

ls | grep -P "test\d+\.c" | sort -V | parallel -L1 --no-notice  $GH_PROJECT_ROOT/scripts/generalSingleExperiment.sh 2> $NULL

$GH_PROJECT_ROOT/scripts/utils/getEnvConfig.sh $EXPERIMENT_LOG
echo "===================================================" >> $EXPERIMENT_LOG

