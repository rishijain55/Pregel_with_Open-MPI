//create a base class node and then inherit master and worker from it. proc 0 is master and rest are workers
#include "pregel.hpp"
#include<bits/stdc++.h>
using namespace std;


class Vertex : public baseVertex<int> {
    public:
    using baseVertex<int>::baseVertex;
    void update();
};

vector<Vertex*> get_graph(int workerId,int numWorkers){
    vector<Vertex*> vertices;
    int N = 400;
    int start = (N/(numWorkers-1))*(workerId-1);
    int end = start + N/(numWorkers-1);
    if(workerId==numWorkers-1) end = N;
    for(int i=start;i<end;i++){
        set<int> adj;
        while(adj.size()!=3){
            int t = rand()%N;
            if(t==i) continue;
            adj.insert(t);
        }
        vector<int> targets(adj.begin(),adj.end());

        int value = workerId;
        vertices.push_back(new Vertex(i,value,targets));
    }
    return vertices;
}


template<>
void Worker<Vertex>::output_results() {
    for (auto vertex : this->vertices) {
        cout << vertex->id << " " << vertex->value << endl;
    }
}

template<>
void Master<Vertex>::output_results() {
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
    Node<Vertex>* node;
    if (workerId == 0) {

        node = new Master<Vertex>(workerId, numWorkers);
    } else {
        vector<Vertex*> vertices = get_graph(workerId, numWorkers);
        node = new Worker<Vertex>(workerId, numWorkers, vertices);
    }

    node->run();
    MPI_Finalize();
    return 0;
}