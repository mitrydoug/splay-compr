CXXFLAGS =-std=c++11 -Wall -Werror -O0 -g
CXX = g++

OBJECTS = Main.o

default: run-tests

run-tests : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Main.o: Main.cpp

%.o: %.cpp %.h

clean:
	rm -f run-tests *.0 *~
