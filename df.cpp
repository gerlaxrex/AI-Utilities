#include<iostream>
#include<vector>
#define N 10
#define INF 600000

std::vector<int> fifo;


//Function to print the path through the nodes of the graph
void printPath(int* path, int i, int init){
    if(init == i){
        std::cout << init << " ";
    }else{
        printPath(path,path[i],init);
        std::cout << i << " ";
    }
}

//Function to compute costs
int computeCost(int*path,int**graph,int i,int init,int cost){
    if(init == i){
        return cost;
    }else{
        int prevNode = path[i];
        computeCost(path,graph,prevNode,init,cost + graph[prevNode][i]);
    }
}

// Function for the Breadth First Algorithm
int breadthfirst(int in, int*path, int**graph, int goal){
    if(in == goal) return 1; //Goal checking
    for(int i = 0; i != N; ++i){
        if(graph[in][i] > 0 && path[i] >= INF){
                fifo.push_back(i);
                path[i] = in;
            }
    }
    int next = *(fifo.begin());
    fifo.erase(fifo.begin());
    breadthfirst(next,path,graph,goal);
}

//Function for the Depth First Algorithm
int depthfirst(int in, int* path,int** graph, int goal){
    if(in == goal){
        return 1;
    }else{
        for(int i = 0; i != N;++i){
            if(graph[in][i] > 0 && path[i] >= INF){
                path[i] = in;
                depthfirst(i,path,graph,goal);
            }
        }
    }
}

//Main in order to test the
int main(){
    int* path = new int[N]; 
    for(int i = 0; i != N; ++i){path[i] = INF;}
    int**graph = new int*[N];
    for(int i = 0; i != N; ++i){
        graph[i] = new int[N];
    }
    path[0] = 0;
    
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

    depthfirst(0,path,graph,9);
    printPath(path,9,0);
    int c = computeCost(path,graph,9,0,0);
    std::cout <<"DF: " << c << std::endl;


    for(int i = 0; i != N; ++i){path[i] = INF;}
    path[0] = 0;
    
    breadthfirst(0,path,graph,9);
    c = computeCost(path,graph,9,0,0);
    std::cout <<"BF: "<< c << std::endl;
    printPath(path,9,0);
    return 0;
}