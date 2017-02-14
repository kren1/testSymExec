# Sets up a constrainer experiment with crest
DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$DIR" ]]; then DIR="$PWD"; fi
source $DIR/../settings.sh



INSTRUMENTER=${GH_PROJECT_ROOT}/scripts/utils/fakeInstrumenter
COMPILE_AND_RUN_1=${GH_PROJECT_ROOT}/scripts/modeC/crestFullRun.sh
COMPILE_AND_RUN_2=${GH_PROJECT_ROOT}/scripts/modeC/nativeFullRun.py
export INSTRUMENTER
export COMPILE_AND_RUN_1
export COMPILE_AND_RUN_2
