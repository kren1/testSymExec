#-------------------------------------------------------------------------------
# Sample makefile for building the code samples. Read inline comments for
# documentation.
#
# Eli Bendersky (eliben@gmail.com)
# This code is in the public domain
#-------------------------------------------------------------------------------

# The following variables will likely need to be customized, depending on where
# and how you built LLVM & Clang. They can be overridden by setting them on the
# make command line: "make VARNAME=VALUE", etc.

# LLVM_SRC_PATH is the path to the root of the checked out source code. This
# directory should contain the configure script, the include/ and lib/
# directories of LLVM, Clang in tools/clang/, etc.
#
# Alternatively, if you're building vs. a binary distribution of LLVM
# (downloaded from llvm.org), then LLVM_SRC_PATH can point to the main untarred
# directory of the binary download (the directory that has bin/, lib/, include/
# and other directories inside).
# See the build_vs_released_binary.sh script for an example.
LLVM_SRC_PATH := ${HOME}/dependencies/llvm-3.9

# LLVM_BUILD_PATH is the directory in which you built LLVM - where you ran
# configure or cmake.
# For linking vs. a binary build of LLVM, point to the main untarred directory.
# LLVM_BIN_PATH is the directory where binaries are placed by the LLVM build
# process. It should contain the tools like opt, llc and clang. The default
# reflects a release build with CMake and Ninja. binary build of LLVM, point it
# to the bin/ directory.
LLVM_BUILD_PATH := $(LLVM_SRC_PATH)/bin
LLVM_BIN_PATH 	:= $(LLVM_SRC_PATH)/bin

$(info -----------------------------------------------)
$(info Using LLVM_SRC_PATH = $(LLVM_SRC_PATH))
$(info Using LLVM_BUILD_PATH = $(LLVM_BUILD_PATH))
$(info Using LLVM_BIN_PATH = $(LLVM_BIN_PATH))
$(info -----------------------------------------------)

# CXX has to be a fairly modern C++ compiler that supports C++11. gcc 4.8 and
# higher or Clang 3.2 and higher are recommended. Best of all, if you build LLVM
# from sources, use the same compiler you built LLVM with.
# Note: starting with release 3.7, llvm-config will inject flags that gcc may
# not support (for example '-Wcovered-switch-default'). If you run into this
# problem, build with CXX set to a modern clang++ binary instead of g++.
CXX := ${LLVM_BIN_PATH}/clang++
CXXFLAGS := -fno-rtti -O0 -g #-fsanitize=address -fno-omit-frame-pointer
PLUGIN_CXXFLAGS := -fpic

LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`
#FIXME: use  a proper versioned lit
LIT := /usr/lib/llvm-3.4/build/utils/lit/lit.py

# Plugins shouldn't link LLVM and Clang libs statically, because they are
# already linked into the main executable (opt or clang). LLVM doesn't like its
# libs to be linked more than once because it uses globals for configuration
# and plugin registration, and these trample over each other.
LLVM_LDFLAGS_NOLIBS := `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared

# These are required when compiling vs. a source distribution of Clang. For
# binary distributions llvm-config --cxxflags gives the right path.
CLANG_INCLUDES := \
	-I$(LLVM_SRC_PATH)/tools/clang/include \
	-I$(LLVM_BUILD_PATH)/tools/clang/include

# List of Clang libraries to link. The proper -L will be provided by the
# call to llvm-config
# Note that I'm using -Wl,--{start|end}-group around the Clang libs; this is
# because there are circular dependencies that make the correct order difficult
# to specify and maintain. The linker group options make the linking somewhat
# slower, but IMHO they're still perfectly fine for tools that link with Clang.
CLANG_LIBS := \
	-Wl,--start-group \
	-lclangAST \
	-lclangASTMatchers \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangEdit \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangEdit \
	-lclangRewrite \
	-lclangRewriteFrontend \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangSerialization \
	-lclangToolingCore \
	-lclangTooling \
	-lclangFormat \
	-Wl,--end-group

# Internal paths in this project: where to find sources, and where to put
# build artifacts.
SRC_DIR := src
BUILDDIR := build
TESTDIR := tests

#i	$(BUILDDIR)/jit_orc_run \
.PHONY: all
all: make_builddir \
	emit_build_config \
	$(BUILDDIR)/tool \

.PHONY: test
test: ${BUILDDIR}/tool
	$(LIT) -v $(TESTDIR)

.PHONY: link_builtins
link_builtins: $(LLVM_SRC_PATH)/lib 
	@ln -s $(LLVM_SRC_PATH)/lib lib


.PHONY: emit_build_config
emit_build_config: make_builddir
	@echo $(LLVM_BIN_PATH) > $(BUILDDIR)/_build_config

.PHONY: make_builddir
make_builddir:
	@test -d $(BUILDDIR) || mkdir $(BUILDDIR)

$(BUILDDIR)/tool: ${BUILDDIR}/instrument ${BUILDDIR}/ToSSATransformer ${BUILDDIR}/Symbolizer ${BUILDDIR}/FunctionLogger $(BUILDDIR)/DeadConditionTransformer $(BUILDDIR)/SwapBranchesTransformer $(BUILDDIR)/RemoveFunction
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $^ \
		$(CLANG_LIBS) $(LLVM_LDFLAGS) -o $@


$(BUILDDIR)/instrument: src/instrument.cpp src/FunctionLogger.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		 -o $@

$(BUILDDIR)/FunctionLogger: src/FunctionLogger.cpp src/FunctionLogger.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@

$(BUILDDIR)/Symbolizer: src/Symbolizer.cpp src/Symbolizer.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@
$(BUILDDIR)/RemoveFunction: src/RemoveFunction.cpp src/RemoveFunction.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@


$(BUILDDIR)/ToSSATransformer: src/ToSSATransformer.cpp src/ToSSATransformer.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@

$(BUILDDIR)/DeadConditionTransformer: src/DeadConditionTransformer.cpp src/DeadConditionTransformer.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@

$(BUILDDIR)/SwapBranchesTransformer: src/SwapBranchesTransformer.cpp src/SwapBranchesTransformer.hpp
	$(CXX) -c $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $< \
		  -o $@




.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/* *.dot test/*.pyc test/__pycache__
