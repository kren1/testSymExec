#!/bin/bash
#example USAGE ./singleEMIExperiment test1.x
DIR_NAME=$(dirname "$(realpath $0)")
CSMITH_OUT=$1
NULL=/dev/null
#csmith > $CSMITH_OUT &&\
$DIR_NAME/emiTest.sh $CSMITH_OUT 2> $NULL > $NULL &&\
echo "$CSMITH_OUT SUCCESFULL" || echo "$CSMITH_OUT fail"
