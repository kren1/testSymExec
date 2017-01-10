#!/bin/bash
#USAGE ./setupExperiment dirname

DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
mkdir $1
cd $1
#Making the system status dump
echo "========== Git ==============" >> experimentConfigdump.txt
git status  >> experimentConfigdump.txt
echo ""  >> experimentConfigdump.txt

echo "========== Scripts ==============" >> experimentConfigdump.txt
tail -n +1 $DIR_NAME/*.sh  >> experimentConfigdump.txt
echo ""  >> experimentConfigdump.txt

echo "========== CSmith  ==============" >> experimentConfigdump.txt
csmith -v  >> experimentConfigdump.txt
echo ""  >> experimentConfigdump.txt

echo "========== KLEE ==============" >> experimentConfigdump.txt
$KLEE --version  >> experimentConfigdump.txt
echo ""  >> experimentConfigdump.txt

seq 10 | xargs -L1 -I{} csmith -o test{}.c

