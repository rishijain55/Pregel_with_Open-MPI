#include<bits/stdc++.h>
#include "vertex.hpp"
#include "node.hpp"
#include<mpi.h>
using namespace std;

class Master : public Node {
    public:
    int numWorkers;
    int workerId;

    Master( int workerId, int numWorkers) {
        this->numWorkers = numWorkers;
        this->workerId = workerId;
    }

    void run() {
        do{
            superstep();
            sendMessages();
        }while((numActive() > 0));
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
        vector<pair<int,double>> messagesToSend;
        // Send counts
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        vector<pair<int,double>> messagesToReceive;
        //create a type for pair<int,double>
        MPI_Datatype MPI_PAIR;
        MPI_Datatype types [2] = {MPI_INT, MPI_DOUBLE};
        int bl[2] = {1,1};
        MPI_Aint offsets[2] = {0, sizeof(int)};
        MPI_Type_create_struct(2, bl, offsets, types, &MPI_PAIR);
        MPI_Type_commit(&MPI_PAIR);

        MPI_Alltoallv(messagesToSend.data(), sendcounts.data(), displs.data(), MPI_PAIR, messagesToReceive.data(), recvcounts.data(), recvdispls.data(), MPI_PAIR, MPI_COMM_WORLD);
    }

    int worker(Vertex* vertex) {
        int vid = vertex->id;
        return vid % (numWorkers-1) +1;
    }

};