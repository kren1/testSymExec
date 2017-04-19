#!/bin/bash
#USAGE ./SetupExperiment dirname

DIR_NAME=$(dirname "$(realpath $0)")

if [ -z "$1" ]; then
echo "Please give folder name as first argument. Exiting ..."
exit 1
fi

case "$2" in
       -m)
              #Multiple paths option
              TOOL="$DIR_NAME/utils/generateSmallPathTestCase.sh" 
              ;;
        *)
              TOOL="$DIR_NAME/utils/generateTestCase.sh" 
              ;;
esac

echo "Using $TOOL"

GIT_DISCOVERY_ACROSS_FILESYSTEM=yes
export GIT_DISCOVERY_ACROSS_FILESYSTEM

source $DIR_NAME/settings.sh
mkdir $1
cd $1
#Making the system status dump
$DIR_NAME/utils/getEnvConfig.sh experimentConfigdump.txt
NODE_LIST_FILE=$DIR_NAME/nodelist
if [ -f $NODE_LIST_FILE ]; then
echo "using multiple nodes from $NODE_LIST_FILE"
PARALLEL="--sshloginfile $NODE_LIST_FILE --env '_'"
else
PARALLEL=""
fi

#seq 10 | xargs -L1 -I{} csmith -o test{}.c
seq 10 | parallel $PARALLEL --no-notice  -L1 -I{}  "$TOOL test{}.c 2> /dev/null"

