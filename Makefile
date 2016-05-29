CXXFLAGS =-std=c++11 -Wall -Werror -O0 -g
CXX = g++

OBJECTS = SplayPrefixEncoder.o Main.o StreamTransformer.o VerbatimTransformer.o PrefixTransformer.o VerbatimBitTransformer.o ReverseTransformer.o StreamEncoder.o AlphaRunLengthEncoder.o SeriesTransformer.o ParallelTransformer.o PPMImageEncoder.o RGBSplitEncoder.o SpaceFillingCurveEncoder.o MoveToFrontEncoder.o PPMDelegateEncoder.o HuffmanEncoder.o

default: run-tests

run-tests : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Main.o: Main.cpp

%.o: %.cpp %.h

clean:
	rm -f run-tests *.o *~
