CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -pedantic

.PHONY: all
all: osh

osh: main.o parser.o executor.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

main.o: main.cpp parser.hpp command.hpp executor.hpp
parser.o: parser.cpp parser.hpp
executor.o: executor.cpp executor.hpp

.PHONY: clean
clean:
	rm -rf osh *.o *.txt
