#!/bin/bash
#example USAGE ./singleEMIExperiment 1
DIR_NAME=$(dirname "$(realpath $0)")
CSMITH_OUT="test${1}.c"
csmith > $CSMITH_OUT &&\
$DIR_NAME/emiTest.sh $CSMITH_OUT &&\
echo "$CSMITH_OUT SUCCESFULL" || echo "$CSMITH_OUT fail"
