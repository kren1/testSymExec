#!/bin/bash
#prefixes all idnetfires in the csmith generated program
#USAGE: ./prefix prefix_string source_file 
DIR_NAME=$(dirname "$(realpath $0)")
PREFIX=$1
cat $2 | sed -e "s/\(g_[[:digit:]]\+\)/$PREFIX\1/g" | sed -e "s/\(func_[[:digit:]]\+\)/$PREFIX\1/g" | sed -e "s/\(U[[:digit:]]\+\)/$PREFIX\1/g" | sed -e "s/\(S[[:digit:]]\+\)/$PREFIX\1/g" 

