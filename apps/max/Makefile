CCOMPILE=mpic++
CFLAGS=
ARCHIVE=
INCLUDE=../../lib

all: max

max: max.cpp 
	$(CCOMPILE) max.cpp -o max -I $(INCLUDE) $(CFLAGS) $(ARCHIVE) -Wno-deprecated -w

run: max
	time mpirun -np 3 ./max

clean:
	-rm max