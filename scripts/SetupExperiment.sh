#!/bin/bash
#USAGE ./SetupExperiment dirname
if [ -z "$1" ]; then
echo "Please give folder name as first argument. Exiting ..."
exit 1
fi

DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
mkdir $1
cd $1
#Making the system status dump
$DIR_NAME/utils/getEnvConfig.sh experimentConfigdump.txt
NODE_LIST_FILE=$DIR_NAME/nodelistt
if [ -f $NODE_LIST_FILE ]; then
echo "using parallel"
PARALLEL="--sshloginfile $NODE_LIST_FILE --env '_'"
else
PARALLEL=""
fi


#seq 10 | xargs -L1 -I{} csmith -o test{}.c
seq 10 | parallel $PARALLEL  --bibtex -L1 -I{}  "$DIR_NAME/utils/generateTestCase.sh test{}.c 2> /dev/null"

