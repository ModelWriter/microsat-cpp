# Author: Michael J. Fischer
# Date: September 4, 2018
# Modified by Ferhat Erata <ferhat@cs.yale.edu> on October 10, 2019

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -Wextra -Wnon-virtual-dtor -Wpedantic -Wnull-dereference -std=c++17
# CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = kalaha.o board.o game.o referee.o tools.o machineAgent.o \
 humanAgent.o test.o snapshot.o
TARGET = kalah
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

board.o: board.cpp board.hpp pit.hpp player.hpp tools.hpp
exception.o: exception.cpp exception.hpp
game.o: game.cpp game.hpp board.hpp pit.hpp player.hpp tools.hpp \
 snapshot.hpp
humanAgent.o: humanAgent.cpp humanAgent.hpp agent.hpp player.hpp \
 tools.hpp exception.hpp
kalaha.o: kalaha.cpp referee.hpp agent.hpp player.hpp tools.hpp game.hpp \
 board.hpp pit.hpp humanAgent.hpp machineAgent.hpp snapshot.hpp test.hpp
machineAgent.o: machineAgent.cpp machineAgent.hpp agent.hpp player.hpp \
 tools.hpp game.hpp board.hpp pit.hpp
referee.o: referee.cpp referee.hpp agent.hpp player.hpp tools.hpp \
 game.hpp board.hpp pit.hpp humanAgent.hpp machineAgent.hpp snapshot.hpp \
 exception.hpp
snapshot.o: snapshot.cpp snapshot.hpp game.hpp board.hpp pit.hpp \
 player.hpp tools.hpp
test.o: test.cpp test.hpp board.hpp pit.hpp player.hpp tools.hpp
tools.o: tools.cpp tools.hpp
