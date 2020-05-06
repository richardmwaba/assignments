CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -pedantic -w

.PHONY: all
all: part1

part1: part1.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

part1.o: part1.cpp definitions.hpp

.PHONY: clean
clean:
	rm -rf part1 *.o *.txt    