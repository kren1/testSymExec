#!/bin/bash
#USAGE ./crestCompileAndRun test0.c test0.c.out
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

LINK_WITH="$INST_LIB_PATH/crest/symbolic.c $INST_LIB_PATH/crest/funCalls.c"
CILLY=${CREST_ROOT}/cil/bin/cilly
COMPILE_DIR=$(mktemp -d)
CALLING_DIR=$(pwd)
CREST_EXEC=$CREST_ROOT/bin/run_crest
INPUT_FILE_FULL_PATH=$(readlink -m $1)

cd $COMPILE_DIR &&\
echo $COMPILE_DIR &&\
cat $LINK_WITH $INPUT_FILE_FULL_PATH > linked_temp.c &&\
${CILLY} linked_temp.c -o executable.o --save-temps --doCrestInstrument \
    -I${CREST_ROOT}/include -I$CSMITH_RUNTIME -L${CREST_ROOT}/lib -lcrest -lstdc++ 2> compile.errs &&\
$CREST_ROOT/bin/process_cfg 2> procCFG.out &&\
timeout 100 $CREST_EXEC  ./executable.o 1000 -dfs
EXIT_STATUS=$?

#Prints the function call trace to stdout and deletes it for cleanup. 
cat $2 &&\
rm $2

rm $BC_FILE $LINKED_BC_FILE
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout" >> $1.info
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    exit 0;
fi

exit 1
