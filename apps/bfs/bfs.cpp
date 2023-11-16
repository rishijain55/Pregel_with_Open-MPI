#include "pregel.hpp"
#include<bits/stdc++.h>
using namespace std;

vector<Vertex*> load_graph(int workerId,int numWorkers){
    vector<Vertex*> vertices;
    int N = 1000;
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
        double value = 0.0;
        vertices.push_back(new Vertex(i,value,targets));
    }
    return vertices;
}

void Vertex::update(){
    if(id==0 && superstep==0) incomingMessages.push_back(0.0);
    if(incomingMessages.size()==0) active = false;
    else{
        active = true;
        if(value!=1.0){
            value = 1.0;
            for(auto message:incomingMessages){
                for(auto t:outVertices_id){
                    outgoingMessages.push_back({t,0.0});
                }
        }
        }
    }
}

int main(int argc,char** argv){
    srand(time(0));
    int numWorkers, workerId;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);
    MPI_Comm_rank(MPI_COMM_WORLD, &workerId);
    Node* node;
    if (workerId == 0) {

        node = new Master(workerId, numWorkers);
    } else {
        vector<Vertex*> vertices = load_graph(workerId, numWorkers);
        node = new Worker(workerId, numWorkers, vertices);
    }
    node->run();
    MPI_Finalize();
    return 0;
}