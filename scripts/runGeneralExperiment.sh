#!/bin/bash
NULL=/dev/null
DIR_NAME=$(dirname "$(realpath $0)")
EXPERIMENT_LOG=${PWD##*/}.log
export EXPERIMENT_LOG

ls | grep -P "test\d+\.c" | sort -V | parallel -L1 --no-notice  $DIR_NAME/generalSingleExperiment.sh 2> $NULL

$DIR_NAME/utils/getEnvConfig.sh $EXPERIMENT_LOG
