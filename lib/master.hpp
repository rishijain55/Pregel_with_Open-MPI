#include<bits/stdc++.h>
#include "vertex.hpp"
#include "node.hpp"
#include<mpi.h>
using namespace std;

class Master : public Node {
    public:
    Master( int workerId, int numWorkers) {
        this->numWorkers = numWorkers;
        this->workerId = workerId;
    }

    void superstep() {
        //implement
        return;
    }

    bool isActive() {
        return false;
    }

    void sendMessages() {
        // note that 0 is the master. so here no message is sent
        vector<int> sendcounts(numWorkers, 0);
        vector<int> displs(numWorkers, 0);
        vector<int> recvcounts(numWorkers, 0);
        vector<int> recvdispls(numWorkers, 0);
        // Prepare messages to send
        pairID *messagesToSend = new pairID[0];
        // Send counts
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        pairID *messagesToReceive = new pairID[0];

        //create a type for pairID
        MPI_Datatype MPI_PAIR;
        MPI_Datatype types [2] = {MPI_INT, MPI_DOUBLE};
        int bl[2] = {1,1};
        MPI_Aint offsets[2] = {offsetof(pairID, first), offsetof(pairID, second)};
        MPI_Type_create_struct(2, bl, offsets, types, &MPI_PAIR);
        MPI_Type_commit(&MPI_PAIR);
        MPI_Alltoallv(messagesToSend, sendcounts.data(), displs.data(), MPI_PAIR, messagesToReceive, recvcounts.data(), recvdispls.data(), MPI_PAIR, MPI_COMM_WORLD);
    }

    void output_results();
    
};