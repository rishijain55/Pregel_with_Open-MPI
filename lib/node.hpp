#ifndef NODE_HPP
#define NODE_HPP

#include<bits/stdc++.h>
#include "vertex.hpp"
#include <mpi.h>
using namespace std;

class Node {
    public:

    virtual void run() = 0;
    virtual bool isActive() = 0;
    int numActive() {
        int totalActive = 0;
        int active = isActive();
        MPI_Allreduce(&active, &totalActive, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        return totalActive;
    }
    virtual void sendMessages() = 0;
    virtual void superstep() = 0;
    virtual int worker(Vertex* vertex) = 0;
};

#endif // NODE_HPP