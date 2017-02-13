#!/bin/bash
#USAGE ./SetupExperiment dirname

DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
mkdir $1
cd $1
#Making the system status dump
$DIR_NAME/utils/getEnvConfig.sh experimentConfigdump.txt

#seq 10 | xargs -L1 -I{} csmith -o test{}.c
seq 100 | parallel -L1 -I{}  $DIR_NAME/utils/generateTestCase.sh test{}.c 2> /dev/null

