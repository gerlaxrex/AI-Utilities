#include<iostream>
#include<vector>
#define N 10
#define INF 600000

std::vector<int> fifo;


void printPath(int* path, int i, int init){
    if(init == i){
        std::cout << init << " ";
    }else{
        printPath(path,path[i],init);
        std::cout << i << " ";
    }
}

int computeCost(int*path,int**graph,int i,int init,int cost){
    if(init == i){
        return cost;
    }else{
        computeCost(path,graph,path[i],init,cost+graph[path[i]][i]);
    }
}

int breadthfirst(int in, int*path, int**graph, int goal, int cost){
    if(in == goal) return cost;
    for(int i = 0; i != N; ++i){
        if(graph[in][i] > 0 && path[i] >= INF){
            if(i != goal){
                fifo.push_back(i);
                path[i] = in;
            }else{
                path[i] = in;
                return computeCost(path,graph,goal,in,0);
            }
        }
    }
    int next = *(fifo.begin());
    fifo.erase(fifo.begin());
    breadthfirst(next,path,graph,goal,cost+graph[in][next]);
}

int depthfirst(int in, int* path,int** graph, int goal, int cost){
    if(in == goal){
        std::cout << cost << std::endl;
        return cost;
    }else{
        for(int i = 0; i != N;++i){
            if(graph[in][i] > 0 && path[i] >= INF){
                path[i] = in;
                depthfirst(i,path,graph,goal,cost+graph[in][i]);
            }
        }
    }
}

int main(){
    int* path = new int[N]; 
    for(int i = 0; i != N; ++i){path[i] = INF;}
    int**graph = new int*[N];
    for(int i = 0; i != N; ++i){
        graph[i] = new int[N];
    }
    path[0] = 0;
    int cost = 0;
    graph[0][1] = 1;
    graph[0][4] = 3;
    graph[0][5] = 3;
    graph[1][2] = 1;
    graph[2][3] = 1;
    graph[3][1] = 1;
    graph[4][0] = 1;
    graph[4][5] = 2;
    graph[4][6] = 4;
    graph[5][7] = 1;
    graph[6][5] = 3;
    graph[6][8] = 1;
    graph[6][9] = 3;
    graph[7][6] = 2;
    graph[7][8] = 2;
    graph[9][4] = 2;
    graph[9][8] = 1;

    cost = depthfirst(0,path,graph,9,0);
    printPath(path,9,0);
    
    for(int i = 0; i != N; ++i){path[i] = INF;}
    path[0] = 0;
    
    cost = breadthfirst(0,path,graph,9,cost);
    std::cout << cost << std::endl;
    printPath(path,9,0);
    return 0;
}