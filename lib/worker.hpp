#ifndef WORKER
#define WORKER
#include<bits/stdc++.h>
#include "aggregator.hpp"
#include "node.hpp"
#include "vertex.hpp"
#include "mpiType.hpp"
#include<mpi.h>
using namespace std;

template<typename Vertex>
class Worker : public Node<Vertex> {
    public:
    typedef pairsec<typename Vertex::valType> pairID;

    Aggregator agg;

    Worker( int workerId, int numWorkers, vector<Vertex*> vertices) {
        this->vertices = vertices;
        this->numWorkers = numWorkers;
        this->workerId = workerId;
    }


    void sendMessages() {
        // note that 0 is the master. use alltoall for transfering messages
        vector<int> sendcounts(this->numWorkers, 0);
        vector<int> displs(this->numWorkers, 0);
        vector<int> recvcounts(this->numWorkers, 0);
        vector<int> recvdispls(this->numWorkers, 0);
        // Prepare messages to send
        
        int tot_send = 0;
        for (auto vertex : this->vertices) {
            vertex->superstep++;
            for (auto message : vertex->outgoingMessages) {
                // cout<<"Worker "<<workerId<<" sending "<<message.second<<" to "<<message.first<<endl;
                sendcounts[this->workerFromId(message.first)]++;
                tot_send++;
            }
            vertex->incomingMessages.clear();
        }

        sendcounts[this->workerId] = 0;
        // Calculate displacements for sending
        for (int i = 1; i < this->numWorkers; i++) {
            displs[i] = displs[i-1] + sendcounts[i-1];
        }

        pairID *messagesToSend = new pairID[tot_send];
        vector<int> cur_cnt(this->numWorkers, 0);
        for (auto vertex : this->vertices) {
            for (auto message : vertex->outgoingMessages) {
                int torecv = this->workerFromId(message.first);
                // cout<<"Worker "<<workerId<<" sending "<<message.second<<" to "<<message.first<<" which is server "<<torecv<<endl;
                int sid = this->workerFromId(message.first);
                if(sid==this->workerId){
                    int vid = message.first;
                    this->vertices[this->getIndex(vid)]->incomingMessages.push_back(message.second);
                }
                else{
                    messagesToSend[displs[torecv] + cur_cnt[torecv]] = message;
                    cur_cnt[torecv]++;
                }
            }
            vertex->outgoingMessages.clear();
        }
        // cout<<"printing send counts"<<endl;
        // for (int i = 0; i < this->numWorkers; i++) {
        //     cout<<sendcounts[i]<<" from server "<<workerId<<" to "<<i<<endl;
        // }
        // Send counts
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        // cout<<"printing counts"<<endl;
        // for (int i = 0; i < this->numWorkers; i++) {
        //     cout<<recvcounts[i]<<" for server "<<workerId<<" from "<<i<<endl;
        // }

        // Calculate displacements for receiving
        for (int i = 1; i < this->numWorkers; i++) {
            recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];
        }

        // Receive messages
        int tot_recv = 0;
        for (int i = 0; i < this->numWorkers; i++) {
            tot_recv += recvcounts[i];
        }
        pairID *messagesToReceive = new pairID[tot_recv];
        //create a type for pairID
        MPI_Datatype MPI_PAIR;
        MPI_Datatype types[2];
        types[0] = MPI_INT;
        types[1] = getMPIType<typename Vertex::valType>();
        int bl[2] = {1,1};


        MPI_Aint offsets[2] = {offsetof(pairID, first), offsetof(pairID, second)};
        MPI_Type_create_struct(2, bl, offsets, types, &MPI_PAIR);
        MPI_Type_commit(&MPI_PAIR);

        MPI_Alltoallv(messagesToSend, sendcounts.data(), displs.data(), MPI_PAIR, messagesToReceive, recvcounts.data(), recvdispls.data(), MPI_PAIR, MPI_COMM_WORLD);

        // Distribute received messages
        for (int i = 0; i < tot_recv; i++) {
            int vid = messagesToReceive[i].first;
            double value = messagesToReceive[i].second;
            this->vertices[this->getIndex(vid)]->incomingMessages.push_back(value);
            // cout<<"Worker "<<workerId<<" received "<<value<<" for "<<vid<<endl;
        }
    }

    void output_results();

};
#endif