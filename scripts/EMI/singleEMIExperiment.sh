#!/bin/bash
#example USAGE ./singleEMIExperiment test1.c
DIR_NAME=$(dirname "$(realpath $0)")
CSMITH_OUT=$1
NULL=/dev/null
#csmith > $CSMITH_OUT &&\
echo $1 >> $1.info
$DIR_NAME/emiTest.sh $CSMITH_OUT 2> $NULL > $NULL 
RUN_STATS=$(cat $1.info | tr '\n' ' ')
echo $RUN_STATS
rm $1.info
