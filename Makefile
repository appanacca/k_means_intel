CPPFLAGS=-g -std=c++11 -O2 -fopenmp #-openmp
CPPCOMP=g++ #icpc

SRCS=main_kmeans.cpp
OBJS=$(subst .cpp,.exe,$(SRCS))
EXE=$(subst .cpp,.exe,$(SRCS))

all: $(SRCS)
	$(CPPCOMP) -o $(EXE) $(SRCS) $(CPPFLAGS)
