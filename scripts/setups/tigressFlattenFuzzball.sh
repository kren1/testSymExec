# Sets up a constrainer experiment with crest
DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$DIR" ]]; then DIR="$PWD"; fi
source $DIR/../settings.sh

LIB_CHOICE=fuzzball/build/symbolic
INSTRUMENTER=${GH_PROJECT_ROOT}/scripts/EMI/tigressFlatten.sh
COMPILE_AND_RUN_1=${GH_PROJECT_ROOT}/scripts/CaR/fuzzballCompileAndRun.sh
COMPILE_AND_RUN_2=${GH_PROJECT_ROOT}/scripts/CaR/fuzzballCompileAndRun.sh
export INSTRUMENTER
export COMPILE_AND_RUN_1
export COMPILE_AND_RUN_2
export LIB_CHOICE
