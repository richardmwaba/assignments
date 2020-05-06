CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -pedantic -w

.PHONY: all
all: part2

part2: part2.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

part2.o: part2.cpp definitions.hpp

.PHONY: clean
clean:
	rm -rf part2 *.o *.txt    