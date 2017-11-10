#!/bin/bash
NULL=/dev/null
NULL=coverage
DIR_NAME=$(dirname "$(realpath $0)")
EXPERIMENT_LOG=${PWD##*/}.log

case $1 in 
    -c)
    EXP=$DIR_NAME/crosscheck/generalCrosscheckExperiment.sh 
    ;;
    *)
    EXP=$DIR_NAME/generalSingleExperiment.sh 
    ;;
esac


export EXPERIMENT_LOG

ls | grep -P "test\d+\.c$" | sort -V | parallel -L1 -j4  --no-notice $EXP 2> $NULL

$DIR_NAME/utils/getEnvConfig.sh $EXPERIMENT_LOG
