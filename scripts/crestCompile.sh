#!/bin/bash

#USAGE ./crestCompile test0.c test0.o
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh

#LINK_WITH="$INST_LIB_PATH/crest/symbolic.c $INST_LIB_PATH/crest/funCalls.c"
CILLY=${CREST_ROOT}/cil/bin/cilly
COMPILE_DIR=$(mktemp -d)
CALLING_DIR=$(pwd)

cd $COMPILE_DIR &&\
echo $COMPILE_DIR &&\
cat $LINK_WITH $CALLING_DIR/$1 > linked_temp.c &&\
${CILLY} linked_temp.c -o ${2} --save-temps --doCrestInstrument \
    -I${CREST_ROOT}/include -I$CSMITH_RUNTIME -L${CREST_ROOT}/lib -lcrest -lstdc++ 2> compile.errs &&\
$CREST_ROOT/bin/process_cfg 2> procCFG.out &&\
#cd - &&\
exit 0 || exit 1
