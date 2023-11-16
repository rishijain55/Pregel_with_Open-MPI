#ifndef NODE
#define NODE

#include<bits/stdc++.h>
#include <mpi.h>
#include "vertex.hpp"
using namespace std;

template<typename Vertex>
class Node {
    public:
    int numWorkers;
    int workerId;
    vector<Vertex*> vertices;
    typedef pairsec<typename Vertex::valType> pairID;

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

    bool isActive() {
        for (auto vertex : vertices) {
            if (vertex->active) {
                return true;
            }
        }
        return false;
    }

    void superstep() {
        for (auto vertex : vertices) {
            vertex->update();
        }
    }

    virtual void sendMessages() = 0;
    virtual void output_results() = 0;
};

#endif // NODE_HPP