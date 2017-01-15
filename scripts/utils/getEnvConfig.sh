#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh

#Making the system status dump
echo "========== Git ==============" >> $1
git diff  >> $1
echo ""  >> $1
git log -n 1 >> $1
echo ""  >> $1

#echo "========== Scripts ==============" >> $1
#tail -n +1 $DIR_NAME/*.sh  >> $1
#echo ""  >> $1

echo "========== CSmith  ==============" >> $1
csmith -v  >> $1
echo ""  >> $1

echo "========== KLEE ==============" >> $1
$KLEE --version  >> $1
echo ""  >> $1


