GH_ROOT=$(realpath $(CURDIR)/..)
DEPS=${HOME}/dependencies
CSMITH_RUNTIME=${DEPS}/csmith/runtime
CREST=${DEPS}/crest
STP=${DEPS}/stp/build/stp
FUZZBALL=${DEPS}/fuzzball/exec_utils/fuzzball
all:
	@sed -e "s\%GH_PROJECT_ROOT%\${GH_ROOT}\g" \
	     -e "s\%CSMITH_RUNTIME%\${CSMITH_RUNTIME}\g" \
	     -e "s\%CREST_ROOT%\${CREST}\g" \
	     -e "s\%FUZZBALL%\${FUZZBALL}\g" \
	     -e "s\%TIGRESS%\${DEPS}/tigress\g" \
	     -e "s\%STP%\${STP}\g" \
	     -e "s\%CLANG3.4%\clang-3.4\g" \
	     -e "s\%LLVM_LINK3.4%\llvm-link-3.4\g" \
	     -e "s\%KLEE%\klee\g" \
         Settings.in > settings.sh
