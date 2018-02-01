#!/bin/bash
#This script should do the same as generla single experiemnt without oracles, for maunal inspection
DIR_NAME=$(dirname "$(realpath $0)")
INSTRUMENTED_FILE=instrumented_$1
ORIGINAL_FILE=$1
$INSTRUMENTER $ORIGINAL_FILE $INSTRUMENTED_FILE 2> /dev/null
$COMPILE_AND_RUN_1 $ORIGINAL_FILE $ORIGINAL_FILE > orig.run
$COMPILE_AND_RUN_2 $INSTRUMENTED_FILE $ORIGINAL_FILE > instrumneted.run


