#!/bin/bash
#EXAMPLE usage ./emiTest.sh test0.c 
#REQUIRES env variables
#COMPILE_AND_RUN_1 - takes a c file and produces output - Runs first
#COMPILE_AND_RUN_2 - takes a c file and produces output - Runs second
#INSTRUMENTER - script that transformers the file

#Imagined usage,
#   * if compile and run are different and instrumenter is a noop, then it's a constrainer experiment.
#   * if compile and run are the same  and instrumenter is an EMI transform, then it's an EMI experiment.


INFO_FILE="$1.info"
NULL=/dev/null
ORIG_RUN=$(mktemp)
INST_FILE=$(mktemp)
EMI_RUN=$(mktemp)
#echo $NULL $ORIG_RUN $INST_FILE $EMI_RUN

echo $1 >> $INFO_FILE

$COMPILE_AND_RUN_1 $1 $1.out > $ORIG_RUN &&\
#shortcircuit if the previous run timeouts
#grep timeout $INFO_FILE && exit 0
$INSTRUMENTER $1 $INST_FILE 2> $NULL &&\
$COMPILE_AND_RUN_2 $INST_FILE $1.out > $EMI_RUN &&\
diff $EMI_RUN $ORIG_RUN &&\
echo "SUCCESS"  >> $INFO_FILE || echo "Fail" >> $INFO_FILE

RUN_STATS=$(cat $INFO_FILE | tr '\n' ' ')
echo $RUN_STATS
if [ -z "$EXPERIMENT_LOG" ];
then 
    :
else
    echo "$RUN_STATS" >> "$EXPERIMENT_LOG";
fi
rm $INFO_FILE
