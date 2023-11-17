CCOMPILE=mpic++
CFLAGS=
ARCHIVE=
INCLUDE=../../lib

all: bfs

bfs: bfs.cpp 
	$(CCOMPILE) bfs.cpp -g -o bfs -I $(INCLUDE) $(CFLAGS) $(ARCHIVE) -Wno-deprecated -w

run: bfs
	time mpirun -np 4 ./bfs 100000 100

clean:
	-rm bfs
