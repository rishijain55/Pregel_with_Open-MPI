#include "mpi.h"
#include<iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <omp.h>

using namespace std;
#define MAXSIZE 100

int *parProc;
vector<int> myNodes;



void bfs(unordered_map<int, vector<int> > &adj, int nProcs, int myRank,unordered_set<int> &level, unordered_map<int,int> &color, int curCol)
{
    vector<int> newlevel;
    for(auto i: level){
        for(auto j: adj[i]){
            if(color.find(j) == color.end()){
                newlevel.push_back(j);
                
            }
            color[j] = curCol;
        }
    }
    
    int* recvSizes = new int[nProcs];
    int si = newlevel.size();
    MPI_Allgather(&si, 1, MPI_INT, recvSizes, 1, MPI_INT, MPI_COMM_WORLD);

    int* recvDispls = new int[nProcs];
    recvDispls[0] = 0;
    for(int i = 1 ; i < nProcs ; i ++){
        recvDispls[i] = recvDispls[i-1] + recvSizes[i-1];
    }
    int* recvbuf = new int[recvDispls[nProcs-1] + recvSizes[nProcs-1]];
    int ns = newlevel.size();
    MPI_Allgatherv(newlevel.data(), ns, MPI_INT, recvbuf, recvSizes, recvDispls, MPI_INT, MPI_COMM_WORLD);
    level.clear();
    for(int i = 0 ; i < recvDispls[nProcs-1] + recvSizes[nProcs-1] ; i ++){
        int u = recvbuf[i];
            if(myRank == parProc[u]) level.insert(u);
            color[u] = curCol;
    }
    
    delete[] recvSizes;
    delete[] recvDispls;
    newlevel.clear();

    int flag =1;
    if(level.size() == 0) flag = 0;

    int newflag;
    MPI_Allreduce(&flag, &newflag, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    flag = newflag;
    if(flag == 0){
        return;
    }
    else{
        bfs(adj, nProcs, myRank, level, color, curCol);
    }
}

void get_graph(unordered_map<int, vector<int>> &adj, int N, int EperN, int myRank, int nProcs){
    int vert = N/nProcs;
    int start = myRank*vert;
    int end = start + vert;
    if(myRank == nProcs-1) end = N;
    for(int i = start ; i < end ; i ++){
        set<int> s;
        while(s.size() != EperN){
            int t = rand()%N;
            if(t == i) continue;
            s.insert(t);
        }
        vector<int> v(s.begin(), s.end());
        adj[i] = v;
    }

}


int main(int argc, char ** argv){
    int nProcs, myRank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    unordered_map<int, vector<int> > adj;
    unordered_set<int> level;
    unordered_map<int,int> color;



    int N = stoi(argv[1]);
    int EperN = stoi(argv[2]);

    parProc = new int[N];
    int div = N/nProcs;
    int st = 0;
    for(int i = 0 ; i < N ; i ++){
        parProc[i] = st;
        if((i+1)%div == 0) st++;
        if(st == nProcs) st--;
    }

    get_graph(adj, N, EperN, myRank, nProcs);


    int curNode = 0;
    int curCol = 1;

    while(curNode < N){
        if(parProc[curNode]== myRank){
            int flag =0;
            if(color.find(curNode) == color.end() ){ 
                color[curNode] = curCol;
                level.insert(curNode);
                flag =1;
                bfs(adj,nProcs, myRank, level, color, curCol);
                curCol++;
            }

        }
        else{
            int flag;
            if(color.find(curNode) == color.end() ){ 
                color[curNode] = curCol;
                bfs(adj,nProcs, myRank, level, color, curCol);
                curCol++;
            }
        }
        curNode++;
        level.clear();
    }
    cout<<"Rank: "<<myRank<<" Col: "<<curCol-1<<endl;
    MPI_Finalize();
    return 0;
}