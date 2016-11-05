#!/bin/bash

#USAGE ./crestCompile test0.c test0.o

TEMP_FILE=crest_linked_temp${1}.c

LIB_PATH=/home/tim/projects/testSymExec/instrument_lib
LINK_WITH="$LIB_PATH/crest/symbolic.c $LIB_PATH/crest/funCalls.c"

cat $LINK_WITH $1 > $TEMP_FILE &&\
crestc  $TEMP_FILE $2
rm $TEMP_FILE
