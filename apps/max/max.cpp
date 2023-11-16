//create a base class node and then inherit master and worker from it. proc 0 is master and rest are workers
#include "master.hpp"
#include "worker.hpp"
#include<mpi.h>
#include<bits/stdc++.h>
using namespace std;

vector<Vertex*> get_graph(int workerId, int numWorkers) {
    vector<Vertex*> vertices;
    int cv = workerId-1;
    int nv = (cv+1)%(numWorkers-1);
    double value = workerId*1.0;
    vertices.push_back(new Vertex(cv, value, {nv}));
    return vertices;
}

void Worker::output_results() {
    for (auto vertex : vertices) {
        cout << vertex->id << " " << vertex->value << endl;
    }
}

void Master::output_results() {
    return;
}


void Vertex::update() {

    double old = value;
    for (auto message : incomingMessages) {
        value = max(value, message);
    }
    if (old >= value && superstep > 0) {
        active = false;
    }
    else{
        active = true;
    }
    if(active){
        for (auto outVertex : outVertices_id) {
            outgoingMessages.push_back({outVertex, value});
        }
    }
    return;
}

int main(int argc, char** argv) {
    int numWorkers, workerId;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);
    MPI_Comm_rank(MPI_COMM_WORLD, &workerId);
    Node* node;
    if (workerId == 0) {

        node = new Master(workerId, numWorkers);
    } else {
        vector<Vertex*> vertices = get_graph(workerId, numWorkers);
        node = new Worker(workerId, numWorkers, vertices);
    }
    node->run();
    MPI_Finalize();
    return 0;
}