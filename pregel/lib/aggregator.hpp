#ifndef AGGREGATOR
#define AGGREGATOR
#include<bits/stdc++.h>
#include<mpi.h>
#include "mpiType.hpp"
using namespace std;

template <typename T>
class Aggregator {
    private:
    vector<T> aggregated_data;
    int numWorkers;
    int workerId;
    public:
    Aggregator(int workers,int id){numWorkers = workers; workerId = id;}
    void aggregate(T v){aggregated_data.push_back(v);}
    vector<T>& value(){return aggregated_data;}
    void all_aggregate(){
        vector<int> sendcounts(numWorkers, 0);
        vector<int> displs(numWorkers, 0);
        vector<int> recvcounts(numWorkers, 0);
        vector<int> recvdispls(numWorkers, 0);

        int tot_send = 0;

        for(int i=0;i<numWorkers;i++){
            if(i==workerId) continue;
            else{
                sendcounts[i] = aggregated_data.size();
                tot_send += aggregated_data.size();
            }
        }

        for(int i=1;i<numWorkers;i++) displs[i] = displs[i-1] + sendcounts[i-1];
        
        MPI_Alltoall(sendcounts.data(), 1, MPI_INT, recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD);
        
        for(int i=1;i<numWorkers;i++) recvdispls[i] = recvdispls[i-1] + recvcounts[i-1];

        int tot_recv = 0;
        for(int i=0;i<numWorkers;i++) tot_recv += recvcounts[i];
        T* receiver = new T[tot_recv];
        MPI_Datatype U = getMPIType<T>();

        MPI_Alltoallv(aggregated_data.data(),sendcounts.data(),displs.data(),U,receiver,recvcounts.data(),recvdispls.data(),U,MPI_COMM_WORLD);
        
        for(int i=0;i<tot_recv;i++){
            T temp = receiver[i];
            if(aggregated_data.find(temp)==aggregated_data.end()) aggregated_data.push_back(temp);
        }
    }
};
#endif