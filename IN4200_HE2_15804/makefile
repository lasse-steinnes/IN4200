##### makefile compile only #####
all: compile

compile:
mpiCC -Wall -o main.out $(wildcard *.cpp .c) -std=c++11 -Ofast

#CC := gcc
#MPICC := mpicc
#FLAGS = -Wall

#PROGRAMS =   main.exe $(wildcard *.cpp .c) #$(patsubst %.cpp,%.exe,$(wildcard *.cpp))
#DEP = convolution.hpp

#.PHONY : all clean

#all: $(PROGRAMS)

#main.exe: main.cpp
#	$(MPICC) $< -o $@ $(FLAGS)

#README.pdf : README.md
#	pandoc -o $@ $<

#clean :
#	rm -f *.o $(PROGRAMS)

#print-%  : ; @echo $* = $($*)
