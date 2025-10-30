# Makefile for viterbi codec, bsc and BER

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

SRCS = viterbi.cpp bsc.cpp ber.cpp viterbi_bsc_ber_main.cpp
OBJ = $(SRCS:.cpp=.o)

all: build test clean

clean: 
	rm -rf *.o build

build: $(OBJ)
	$(CXX) $(CXXFLAGS) -o build.out $(OBJ)

test:
	./build.out

cppcheck:
	cppcheck --enable=all --std=c17 --suppress=missingIncludeSystem --check-level=exhaustive *.cpp
	cppcheck --enable=all --suppress=missingIncludeSystem *.hpp



