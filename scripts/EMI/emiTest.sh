#!/bin/bash
#EXAMPLE usage ./emiTest.sh test0.c 
#REQUIRES env variables
#COMPILE_AND_RUN - takes a c file and produces output
#INSTRUMENTER - script that transformers the file
NULL=/dev/null
ORIG_RUN=$(mktemp)
INST_FILE=$(mktemp)
EMI_RUN=$(mktemp)
#echo $NULL $ORIG_RUN $INST_FILE $EMI_RUN
$COMPILE_AND_RUN $1 $1.out $ORIG_RUN &&\
#shortcircuit if the previous run timeouts
grep timeout $1.info && exit 0
$INSTRUMENTER $1 $INST_FILE 2> $NULL &&\
$COMPILE_AND_RUN $INST_FILE $1.out > $EMI_RUN &&\
diff $EMI_RUN $ORIG_RUN &&\
echo "SUCCESS"  >> $1.info &&\
exit 0 || echo "Fail" >> $1.info
exit 1

