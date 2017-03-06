#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
PREPROC=$(mktemp)
mv $PREPROC $PREPROC.c
PREPROC=$PREPROC.c
#cat $INST_LIB_PATH/native/*.c $1 > $PREPROC
echo '#include "instrument_lib.h"' | cat - $1 > $PREPROC
$TIGRESS -I$CSMITH_RUNTIME -I$INST_LIB_PATH -L$INST_LIB_PATH/native/build -lsymbolic --Transform=Flatten --Functions=* --Exclude=main,symbolize_and_constrain_u,print_symbolic,logFunction --out=$2 $PREPROC
rm $PREPROC
#$TIGRESS -include $CSMITH_RUNTIME/* --Transform=InitOpaque --Functions=* --Transform=AddOpaque --Functions=* --out=$2 $1

