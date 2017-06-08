# Sets up a constrainer experiment with crest
DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$DIR" ]]; then DIR="$PWD"; fi
source $DIR/../settings.sh

NAT_LIB_CHOICE=native/build/pathRead
LIB_CHOICE=klee/modeC/build/path
OTHER_FLAGS="-D BRANCH_LOG"
REVERSE=yes

INSTRUMENTER=${GH_PROJECT_ROOT}/scripts/EMI/swapBranches.sh
COMPILE_AND_RUN_1=${GH_PROJECT_ROOT}/scripts/CaR/kleeCompileAndRun.sh
COMPILE_AND_RUN_2=${GH_PROJECT_ROOT}/scripts/CaR/nativeCompileAndRun.sh
export INSTRUMENTER
export COMPILE_AND_RUN_1
export COMPILE_AND_RUN_2
export LIB_CHOICE
export OTHER_FLAGS
export NAT_LIB_CHOICE
export REVERSE
