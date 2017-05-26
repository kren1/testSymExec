#!/bin/bash
#EXAMPLE usage ./crossCheckExperiment.sh test0.c 
#REQUIRES env variables
#COMPILE_AND_RUN_1 - takes a c file and produces output - Runs first
#INSTRUMENTER - script that transformers the file


INFO_FILE="$1.info"
INST_FILE=$(mktemp)
CROSS_RUN=$(mktemp)
CROSSCHECK_VER=$(mktemp)

DIR_NAME=$(dirname "$(realpath $0)")
NULL=/dev/null
ORIG_LOC=$(realpath $1)

#Here we use > to make sure info file is created fresh
echo $1 > $INFO_FILE

$INSTRUMENTER $1 $INST_FILE 2> $NULL &&\
$DIR_NAME/klee_crosscheck_prepare.sh $1 $INST_FILE $CROSSCHECK_VER &&\
$COMPILE_AND_RUN_1 $CROSSCHECK_VER $ORIG_LOC 2>&1 | tee  $CROSS_RUN | grep -v -P 'func_\d+$'>&2  && ((!PIPESTATUS[0])) &&\
! grep 'ASSERTION FAIL:' $CROSS_RUN  &&\
echo "SUCCESS"  >> $INFO_FILE || echo "Fail" >> $INFO_FILE

RUN_STATS=$(cat $INFO_FILE | tr '\n' ' ')
echo $RUN_STATS
if [ -z "$EXPERIMENT_LOG" ];
then 
    :
else
    echo "$RUN_STATS" >> "$EXPERIMENT_LOG";
fi
rm $CROSS_RUN $INFO_FILE $CROSSCHECK_VER $INST_FILE
