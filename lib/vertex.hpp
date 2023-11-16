#ifndef VERTEX_HPP
#define VERTEX_HPP
#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct pairsec {
    int first;
    T second;
};


template<typename T>
class baseVertex {
public:
    typedef T valType;
    typedef pairsec<valType> pairID;
    int id;
    valType value;
    vector<int> outVertices_id;
    vector< valType> incomingMessages;
    vector<pairID> outgoingMessages;
    bool active;
    int superstep;

    // Constructor, Update method, etc.
    baseVertex(int id, valType value, vector<int> outVertices_id) {
        this->id = id;
        this->value = value;
        this->outVertices_id = outVertices_id;
        this->active = true;
        this->superstep = 0;
    }

    void update();

};
#endif // VERTEX_HPP