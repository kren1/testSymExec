#!/bin/bash
#USAGE ./kleeCompileAndRun test1.c test1.c
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
NULL=/dev/null
#NULL=/dev/tty


$DIR_NAME/kleeCompileAndRun.sh $1 $2 > $NULL 
$DIR_NAME/nativeCompileAndRun.sh $1 $2
