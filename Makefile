CPPFLAGS=-g -std=c++1z -O2 -fopenmp #-openmp
CPPCOMP=g++-7 #icpc

SRCS=main_kmeans.cpp
OBJS=$(subst .cpp,.exe,$(SRCS))
EXE=k_means

all: $(SRCS)
	$(CPPCOMP) -o $(EXE) $(SRCS) $(CPPFLAGS) -I/home/appanacca/boost -L/home/appanacca/boost/stage/lib -lboost_program_options

clean:
	rm -rf $(EXE)
