#include<bits/stdc++.h>
#include "vertex.hpp"
#include "node.hpp"
#include<mpi.h>
using namespace std;


class Worker : public Node {
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
        do{
            superstep();
            sendMessages();
            // cout<<"Worker "<<workerId<<endl;
        }while((numActive() > 0));
        cout<<"Worker "<<workerId<<" done and value is "<<vertices[0]->value<<endl;
    }

    void superstep() {
        for (auto vertex : vertices) {
            vertex->update();
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



    void sendMessages() {
        // note that 0 is the master. use alltoall for transfering messages

        vector<int> sendcounts(numWorkers, 0);
        vector<int> displs(numWorkers, 0);
        vector<int> recvcounts(numWorkers, 0);
        vector<int> recvdispls(numWorkers, 0);
        // Prepare messages to send
        
        int tot_send = 0;
        for (auto vertex : vertices) {
            vertex->superstep++;
            for (auto message : vertex->outgoingMessages) {
                // cout<<"Worker "<<workerId<<" sending "<<message.second<<" to "<<message.first<<endl;
                sendcounts[workerFromId(message.first)]++;
                tot_send++;
            }
            vertex->incomingMessages.clear();
            // vertex->outgoingMessages.clear();
        }
        // Calculate displacements for sending
        for (int i = 0; i < numWorkers; i++) {
            displs[i] = displs[i-1] + sendcounts[i-1];
        }

        pairID *messagesToSend = new pairID[tot_send];
        vector<int> cur_cnt(numWorkers, 0);
        for (auto vertex : vertices) {
            for (auto message : vertex->outgoingMessages) {
                int torecv = workerFromId(message.first);
                // cout<<"Worker "<<workerId<<" sending "<<message.second<<" to "<<message.first<<" which is server "<<torecv<<endl;
                messagesToSend[displs[torecv] + cur_cnt[torecv]] = message;
                cur_cnt[torecv]++;
            }
            vertex->outgoingMessages.clear();
        }
        // cout<<"printing send counts"<<endl;
        // for (int i = 0; i < numWorkers; i++) {
        //     cout<<sendcounts[i]<<" from server "<<workerId<<" to "<<i<<endl;
        // }
        // Send counts
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        // cout<<"printing counts"<<endl;
        // for (int i = 0; i < numWorkers; i++) {
        //     cout<<recvcounts[i]<<" for server "<<workerId<<" from "<<i<<endl;
        // }

        // Calculate displacements for receiving
        for (int i = 1; i < numWorkers; i++) {
            recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];
        }

        // Receive messages
        
        int tot_recv = 0;
        for (int i = 0; i < numWorkers; i++) {
            tot_recv += recvcounts[i];
        }
        pairID *messagesToReceive = new pairID[tot_recv];
        //create a type for pairID
        MPI_Datatype MPI_PAIR;
        MPI_Datatype types [2] = {MPI_INT, MPI_DOUBLE};
        int bl[2] = {1,1};


        MPI_Aint offsets[2] = {offsetof(pairID, first), offsetof(pairID, second)};
        MPI_Type_create_struct(2, bl, offsets, types, &MPI_PAIR);
        MPI_Type_commit(&MPI_PAIR);

        MPI_Alltoallv(messagesToSend, sendcounts.data(), displs.data(), MPI_PAIR, messagesToReceive, recvcounts.data(), recvdispls.data(), MPI_PAIR, MPI_COMM_WORLD);

        // Distribute received messages
        for (int i = 0; i < tot_recv; i++) {
            int vid = messagesToReceive[i].first;
            double value = messagesToReceive[i].second;
            vertices[getIndex(vid)]->incomingMessages.push_back(value);
            // cout<<"Worker "<<workerId<<" received "<<value<<" for "<<vid<<endl;
        }
    }

    int workerFromId(int vid) {
        return ((vid%(numWorkers-1)) +1);
    }

    int getIndex(int vid) {
        return vid/(numWorkers-1);
    }
};