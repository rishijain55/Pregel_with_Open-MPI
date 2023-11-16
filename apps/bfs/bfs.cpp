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
    int N = 20;
    int start = (N/(numWorkers-1))*(workerId-1);
    int end = start + N/(numWorkers-1);
    if(workerId==numWorkers-1) end = N;
    for(int i=start;i<end;i++){
        set<int> adj;
        while(adj.size()!=10){
            int t = rand()%N;
            if(t==i) continue;
            adj.insert(t);
        }
        vector<int> targets(adj.begin(),adj.end());
        cout<<"Worker "<<workerId<<" vertex "<<i<<" has "<<targets.size()<<" out edges"<<endl;
        for(auto t:targets){
            cout<<t<<" ";
        }
        cout<<endl;
        int value = 0;
        vertices.push_back(new Vertex(i,value,targets));
    }
    return vertices;
}


void Vertex::update(){
    if(id==0 && superstep==0) incomingMessages.push_back(0);
    if(value ==1 || incomingMessages.size()==0) active = false;
    else{
        active = true;
        value = 1;
        for(auto t:outVertices_id){
            outgoingMessages.push_back({t,0});
        }
    }
}

template<>
void Worker<Vertex>::output_results() {
    for(auto vertex:vertices){
        cout<<vertex->id<<" "<<vertex->value<<endl;
    }
}

template<>
void Master<Vertex>::output_results() {
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