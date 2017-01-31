#Requires mktemp, dirname, realpath, timeout, basename
#TODO: assert those
GH_PROJECT_ROOT=/home/tim/projects/testSymExec

CSMITH_RUNTIME=/home/tim/dependencies/csmith/runtime
INST_LIB_PATH=$GH_PROJECT_ROOT/instrument_lib
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
CLANG=clang-3.4
LINK=llvm-link-3.4
KLEE=klee

#CREST
CREST_ROOT=/home/tim/dependencies/crest

#FuzzBALL
FUZZBALL=/home/tim/dependencies/fuzzball/exec_utils/fuzzball
STP=/home/tim/dependencies/stp/build/stp 
#STP=/home/tim/dependencies/fuzzball/stp/stp/build/stp 
