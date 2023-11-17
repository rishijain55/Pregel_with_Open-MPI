CCOMPILE=mpic++
CFLAGS= -lboost_mpi -lboost_serialization -lboost_graph -lboost_graph_parallel
ARCHIVE=

all: bfs

bfs: bfs.cpp 
	$(CCOMPILE) bfs.cpp -o bfs $(CFLAGS) $(ARCHIVE)

run: bfs
	time mpirun -np 4 ./bfs 100000 100 

clean:
	-rm bfs
