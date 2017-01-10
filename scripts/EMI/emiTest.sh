#!/bin/bash
#EXAMPLE usage ./emiTest.sh test0.c 
#REQUIRES env variables
#COMPILE_AND_RUN - takes a c file and produces output
#INSTRUMENTER - script that transformers the file
NULL=/dev/null
#NULL=$(mktemp)
ORIG_RUN=$(mktemp)
INST_FILE=$(mktemp)
EMI_RUN=$(mktemp)
#echo $NULL $ORIG_RUN $INST_FILE
$COMPILE_AND_RUN $1 2> $NULL > $ORIG_RUN &&\
$INSTRUMENTER $1 $INST_FILE 2> $NULL &&\
$COMPILE_AND_RUN $INST_FILE 2> $NULL > $EMI_RUN &&\
diff $EMI_RUN $ORIG_RUN &&\
exit 0 || echo "Fail"
exit 1

