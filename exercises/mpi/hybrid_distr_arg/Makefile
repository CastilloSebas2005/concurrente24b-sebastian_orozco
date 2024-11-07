include ../../../common/Makefile

CC=mpicc
XC=mpic++
FLAG=-fopenmp

# macos: brew install gcc
ifneq (, $(shell which gcc-11))
	CC=MPICH_CC=gcc-11 mpicc
	XC=MPICH_CXX=g++-11 mpic++
endif