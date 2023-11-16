#ifndef AGGREGATOR
#define AGGREGATOR
#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

class Aggregator {
    private:
    set<int> aggregated_vertices;
    public:
    void aggregate(int v){aggregated_vertices.insert(v);}
    set<int> value(){return aggregated_vertices;}
};
#endif