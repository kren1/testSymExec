#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
NULL=/dev/null
source $DIR_NAME/../settings.sh
source $GH_PROJECT_ROOT/scripts/EMI/setupEMI.sh

ls | grep -P "test\d+\.c" | sort -V | xargs -L1  $GH_PROJECT_ROOT/scripts/generalSingleExperiment.sh 2> $NULL
