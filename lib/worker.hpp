#include<bits/stdc++.h>
#include "vertex.hpp"
#include<mpi.h>
using namespace std;

class Worker {
    public:
    vector<Vertex*> vertices;
    int numWorkers;
    int workerId;

    Worker( int workerId, int numWorkers, vector<Vertex*> vertices) {
        this->vertices = vertices;
        this->numWorkers = numWorkers;
        this->workerId = workerId;
    }

    void run() {
        superstep();
        sendMessages();
        // Cleanup logic, if any
    }

    void superstep() {
        for (auto vertex : vertices) {
            if(vertex->active) {
                vertex->update();
            }
        }
    }

    bool isActive() {
        for (auto vertex : vertices) {
            if (vertex->active) {
                return true;
            }
        }
        return false;
    }

    int numActive() {
        int totalActive = 0;
        int active = isActive();
        MPI_Allreduce(&active, &totalActive, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        return totalActive;
    }

    void sendMessages() {
        // note that 0 is the master. use alltoall for transfering messages

        vector<int> sendcounts(numWorkers, 0);
        vector<int> displs(numWorkers, 0);
        vector<int> recvcounts(numWorkers, 0);
        vector<int> recvdispls(numWorkers, 0);
        // Prepare messages to send
        vector<pair<int,double>> messagesToSend;
        int tot_send = 0;
        for (auto vertex : vertices) {
            vertex->superstep++;
            for (auto message : vertex->outgoingMessages) {
                sendcounts[worker(vertex)]++;
                tot_send++;
            }
            vertex->incomingMessages.clear();
            vertex->outgoingMessages.clear();
        }
        // Calculate displacements for sending
        for (int i = 1; i < numWorkers; i++) {
            displs[i] = displs[i-1] + sendcounts[i-1];
        }

        messagesToSend.resize(tot_send);
        vector<int> cur_cnt(numWorkers, 0);
        for (auto vertex : vertices) {
            for (auto message : vertex->outgoingMessages) {
                messagesToSend[displs[worker(vertex)] + cur_cnt[worker(vertex)]] = message;
                cur_cnt[worker(vertex)]++;
            }
        }

        // Send counts
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);

        // Calculate displacements for receiving
        for (int i = 1; i < numWorkers; i++) {
            recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];
        }

        // Receive messages
        vector<pair<int,double>> messagesToReceive;
        int tot_recv = 0;
        for (int i = 1; i < numWorkers; i++) {
            tot_recv += recvcounts[i];
        }
        messagesToReceive.resize(tot_recv);
        //create a type for pair<int,double>
        MPI_Datatype MPI_PAIR;
        MPI_Datatype types [2] = {MPI_INT, MPI_DOUBLE};
        int bl[2] = {1,1}, offsets[2] = {0, sizeof(int)};
        MPI_Type_create_struct(2, bl, offsets, types, &MPI_PAIR);
        MPI_Type_commit(&MPI_PAIR);

        MPI_Alltoallv(messagesToSend.data(), sendcounts.data(), displs.data(), MPI_PAIR, messagesToReceive.data(), recvcounts.data(), recvdispls.data(), MPI_PAIR, MPI_COMM_WORLD);

        // Distribute received messages
        for (int i = 0; i < tot_recv; i++) {
            int vid = messagesToReceive[i].first;
            double value = messagesToReceive[i].second;
            vertices[vid]->incomingMessages.push_back(value);
        }
    }

    int worker(Vertex* vertex) {
        int vid = vertex->id;
        return vid % (numWorkers-1) +1;
    }

};