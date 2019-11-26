# Author: Michael J. Fischer
# Date: September 4, 2018
# Modified by Ferhat Erata <ferhat@cs.yale.edu> on October 10, 2019

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O2 -std=c++17
# CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o driver.o solver.o tools.o
TARGET = microsat++
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
# c++ -MM *.cpp >> Makefile
driver.o: driver.cpp driver.hpp solver.hpp tools.hpp
main.o: main.cpp driver.hpp solver.hpp tools.hpp
solver.o: solver.cpp solver.hpp tools.hpp
tools.o: tools.cpp tools.hpp
