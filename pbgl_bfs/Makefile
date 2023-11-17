CCOMPILE=mpic++
CFLAGS= -lboost_mpi -lboost_serialization -lboost_graph -lboost_graph_parallel
ARCHIVE=

all: bfs

bfs: bfs.cpp 
	$(CCOMPILE) bfs.cpp -o bfs $(CFLAGS) $(ARCHIVE)

run: bfs
	time mpirun -np 2 ./bfs 10 2 

clean:
	-rm bfs