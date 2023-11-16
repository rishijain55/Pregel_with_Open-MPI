#ifndef VERTEX_HPP
#define VERTEX_HPP
#include<bits/stdc++.h>

using namespace std;
// Vertex.h
class Vertex {
public:
    int id;
    double value;
    vector<int> outVertices_id;
    vector< double> incomingMessages;
    vector<pair<int, double>> outgoingMessages;
    bool active;
    int superstep;

    // Constructor, Update method, etc.
    Vertex(int id, double value, vector<int> outVertices_id) {
        this->id = id;
        this->value = value;
        this->outVertices_id = outVertices_id;
        this->active = true;
        this->superstep = 0;
    }

    void update();

};
#endif // VERTEX_HPP