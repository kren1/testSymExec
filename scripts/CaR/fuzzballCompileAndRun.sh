#!/bin/bash
#USAGE ./fuzzballCompileAndRun /path/to/source.c funCallsOutName.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
NULL=/dev/null
#NULL=/dev/tty

O_FILE=$(mktemp)
#LINKED_FILE=linked.o #$(mktemp)
LINKED_FILE=$(mktemp)
$CLANG -xc -c -g -I$INST_LIB_PATH -m32 -I$CSMITH_RUNTIME -o $O_FILE $1 2> $NULL &&\
$CLANG -o $LINKED_FILE -m32 $O_FILE $INST_LIB_PATH/fuzzball/build/*.o 2> $NULL &&\
ADDRESSES=$(objdump -d $LINKED_FILE | grep -P "call [\s\w\d]+ <symbolic\d+" \
 | awk '{print  "-skip-call-ret-symbol 0x" substr($1, 1, length($1)-1) "=n"}' | tail -n1 |  head -n1) &&\
ADDRESSES=$(objdump -D $LINKED_FILE | grep -P "magic_symbols" \
 | awk '{print  "-symbolic-string 0x"i $1 "+16"}' | tail -n1 |  head -n1) &&\
#echo $ADDRESSES &&\
START=$(date +%s.%N) &&\
timeout 100 $FUZZBALL  -solver-path /home/tim/dependencies/fuzzball/stp/stp/build/stp -linux-syscalls $ADDRESSES $LINKED_FILE -- $LINKED_FILE
#timeout 100 $FUZZBALL  -solver-path /home/tim/dependencies/fuzzball/stp/stp/build/stp -linux-syscalls -trace-stopping -trace-conditions $ADDRESSES $LINKED_FILE -- $LINKED_FILE
#timeout 100 $FUZZBALL -linux-syscalls $ADDRESSES $LINKED_FILE -- $LINKED_FILE
EXIT_STATUS=$?
DURATION=$(echo "$(date +%s.%N) - $START" | bc) &&\

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $(basename $2).out &&\
rm $(basename $2).out


rm $O_FILE $LINKED_FILE
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout" >> $2.info
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    echo "n:${DURATION}" >> $2.info
    exit 0;
fi

exit 1
