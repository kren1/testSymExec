#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
NULL=/dev/null
source $DIR_NAME/../settings.sh
source $GH_PROJECT_ROOT/scripts/EMI/setupKleeEMI.sh

$GH_PROJECT_ROOT/scripts/runGeneralExperiment.sh

