#!/bin/bash
#This script renames test case
#usage: renameTestCase path/to/oldname path/to/newname
DIR_NAME=$(dirname "$(realpath $0)")
OLD_NAME=$(basename $1)
NEW_NAME=$(basename $2)
cat $1 | sed -e "s/$OLD_NAME/$NEW_NAME/g" > $2 &&\
rm $1

