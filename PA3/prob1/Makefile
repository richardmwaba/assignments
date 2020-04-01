CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -pedantic -lpthread -w

.PHONY: all
all: part1

part1: main.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

main.o: main.cpp

.PHONY: clean
clean:
	rm -rf part1 *.o *.txt
