# Makefile for viterbi codec, bsc and BER

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

SRCS = viterbi/viterbi.cpp BSC/bsc.cpp BER/ber.cpp viterbi_bsc_ber_main.cpp
OBJ = $(SRCS:.cpp=.o)

SRCS_TEST = viterbi/viterbi.cpp BSC/bsc.cpp BER/ber.cpp viterbi_bsc_ber_test.cpp
OBJ_TEST = $(SRCS_TEST:.cpp=.o)

all: build clean

clean: 
	rm -rf build* *.o viterbi/*.o BSC/*.o BER/*.o

build: $(OBJ)
	$(CXX) $(CXXFLAGS) -o build $(OBJ)
	@echo "--------------------"
	@echo "Запуск моделирования"
	@echo "--------------------"
	./build
	@echo "--------------------"
	@echo "Построение графика"
	@echo "--------------------"
	python3 BER/ber_plot.py

test: $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) -o build_test $(OBJ_TEST)
	./build_test
	rm -rf build* *.o viterbi/*.o BSC/*.o BER/*.o

cppcheck:
	cppcheck --enable=all --std=c17 --suppress=missingIncludeSystem --check-level=exhaustive *.cpp
	cppcheck --enable=all --suppress=missingIncludeSystem *.hpp



