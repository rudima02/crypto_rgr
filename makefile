# Makefile для структуры:
# ./main/main.cpp, ./main/file_io.cpp, headers in ./src/,
# algorithms in ./algorithms/<dir>/<cpp,h>

CXX = g++
CXXFLAGS = -fPIC -std=c++17 -Wall -O2
LDFLAGS = -shared
INCLUDE = -Isrc

# main sources
MAIN_CPP = main/main.cpp
MAIN_EXTRA = main/file_io.cpp

# find all algorithm .cpp files under algorithms/* (one .cpp per algo dir expected)
ALG_DIRS := $(wildcard algorithms/*)
ALG_CPP := $(foreach d,$(ALG_DIRS),$(wildcard $(d)/*.cpp))
# produce SO names like libfixed_perms.so from algorithms/fixed_perms/fixed_perms.cpp
ALG_SO := $(patsubst algorithms/%/%.cpp,lib%.so,$(ALG_CPP))

.PHONY: all clean

all: main $(ALG_SO)
	@echo "Build finished. Binaries: ./main and $(ALG_SO)"

# build main, include headers from src
main: $(MAIN_CPP) $(MAIN_EXTRA)
	$(CXX) -o $@ $(MAIN_CPP) $(MAIN_EXTRA) $(INCLUDE) -ldl

# generic rule: build lib<dir>.so from algorithms/<dir>/*.cpp
# e.g. algorithms/fixed_perms/fixed_perms.cpp -> libfixed_perms.so
lib%.so:
	@echo "Building $@ ..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ algorithms/$*/$*.cpp $(INCLUDE)

clean:
	rm -f main *.so
