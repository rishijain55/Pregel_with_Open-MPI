#ifndef NODE
#define NODE

#include<bits/stdc++.h>
#include "vertex.hpp"
#include <mpi.h>
using namespace std;


class Node {
    public:
    int numWorkers;
    int workerId;

    void run() {
        do{
            superstep();
            sendMessages();
        }while((numActive() > 0));
        output_results();
    }

    int numActive() {
        int totalActive = 0;
        int active = isActive();
        MPI_Allreduce(&active, &totalActive, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        return totalActive;
    }

    int workerFromId(int vid) {
        return ((vid%(numWorkers-1)) +1);
    }

    int getIndex(int vid) {
        return vid/(numWorkers-1);
    }

    virtual void sendMessages() = 0;
    virtual void superstep() = 0;
    virtual bool isActive() = 0;
    virtual void output_results() = 0;
};

#endif // NODE_HPP