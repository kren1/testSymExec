#!/bin/bash

INSTRUMENT_TOOL=/home/tim/projects/testSymExec/src2src/build/tool
$INSTRUMENT_TOOL -extra-arg=-I$CSMITH_RUNTIME  $1 --  > $2  2> /dev/null
