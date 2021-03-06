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
#Stores location of the original file, so .out and .info files can be obtained
ORIG_LOC=$(realpath $1)
#echo $NULL $ORIG_RUN $INST_FILE $EMI_RUN
#Here we use > to make sure info file is created fresh
echo $1 > $INFO_FILE


BIN_FILE="$1.bin"
export BIN_FILE
case $REVERSE in 
    yes)
    RUN1=$INST_FILE
    RUN2=$1
    ;;
    *)
    RUN1=$1
    RUN2=$INST_FILE
    ;;
esac



#set -x
$INSTRUMENTER $1 $INST_FILE 2> $NULL &&\
$COMPILE_AND_RUN_1 $RUN1 $ORIG_LOC | grep -v checksum > $ORIG_RUN &&\
$COMPILE_AND_RUN_2 $RUN2 $ORIG_LOC | grep -v checksum > $EMI_RUN &&\
#(!(diff $EMI_RUN $ORIG_RUN |  grep '^> ' ) >&2 || grep timeout $INFO_FILE) &&\
(diff $EMI_RUN $ORIG_RUN  >&2 || grep timeout $INFO_FILE) &&\
echo "SUCCESS"  >> $INFO_FILE || echo "Fail" >> $INFO_FILE
#cat $ORIG_RUN $INFO_FILE


#vimdiff $EMI_RUN $ORIG_RUN 
RUN_STATS=$(cat $INFO_FILE | tr '\n' ' ')
echo $RUN_STATS
if [ -z "$EXPERIMENT_LOG" ];
then 
    :
else
    echo "$RUN_STATS" >> "$EXPERIMENT_LOG";
fi
rm $ORIG_RUN $INFO_FILE $EMI_RUN $INST_FILE $BIN_FILE
