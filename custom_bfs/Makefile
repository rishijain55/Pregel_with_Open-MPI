CCOMPILE=mpic++
CFLAGS=
ARCHIVE=

all: bfs

bfs: bfs.cpp
	$(CCOMPILE) bfs.cpp -o bfs -I ../../lib $(CFLAGS) $(ARCHIVE) -Wno-deprecated

run: bfs
	mpirun -np 4 ./bfs 10000 100

clean:
	-rm bfs
