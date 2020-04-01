CXX = g++
CXXFLAGS = -Wall -O3 -std=c++11 -pedantic -lpthread -w

.PHONY: all
all: part2

part2: main.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

main.o: main.cpp main.hpp producer_consumer_monitor.hpp

.PHONY: clean
clean:
	rm -rf part2 *.o *.txt    