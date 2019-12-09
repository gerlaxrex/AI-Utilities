#include<iostream>
#include<queue>
#include<map>
#include<utility>
#include<functional>
#include<string>
#include<cmath>
#include<fstream>
#include<set>

typedef std::pair<double,int> P;
typedef std::pair<double,std::pair<int,int>> Q;

class Graph{
private:

    double** graph; //costs matrix
    char** representation; //Eventual graphical representation of the graph
    std::pair<int,double>* predecessors; //best path predecessor from one node;
    std::pair<std::pair<int,int>,double>* path; //best predecessor point
    int nnodes,reprRows, reprCols;

public:

    //Constructor (for costs)
    Graph(int n, double**g):nnodes(n){
        graph = new double*[nnodes];
        predecessors = new std::pair<int,double>[nnodes];

        for(int i = 0; i != nnodes; ++i){
            graph[i] = new double[nnodes];
            predecessors[i] = std::make_pair(-1, 3000000);    
        }
        for(int i = 0; i != nnodes; ++i){
            for(int j = 0; j != nnodes; ++j){
                graph[i][j] = g[i][j];
            }
        }
    }

    //Constructor (also for graphics)
    Graph(const std::string& filename){
        std::fstream strm(filename);
        if(!strm){ std::cout << "File not exists."<< std::endl;}
        else{ 
            char input;
            strm >> reprRows;
            strm >> reprCols;
            path = new std::pair<std::pair<int,int>,double>[reprCols*reprRows];
            representation = new char*[reprCols];
            for(int i = 0; i != reprCols; ++i){
                representation[i] = new char[reprRows];
            }
            std::cout << reprRows << " " << reprCols << std::endl;
            for(int i = 0; i != reprRows; ++i){
                for(int j = 0; j != reprCols; ++j){
                    strm >> representation[i][j];
                    path[i+j] = std::make_pair(std::make_pair(-1,-1),60000);
                }
            }
            
        }
    }

    //Deconstructor
    ~Graph(){
        for(int i = 0; i != nnodes; ++i){
            delete [] graph[i];
        }
        delete [] graph;
        delete [] predecessors;
    }

    Graph(const Graph& g):nnodes(g.nnodes){
        graph = new double*[nnodes];
        predecessors = new std::pair<int,double>[nnodes];

        for(int i = 0; i != nnodes; ++i){
            graph[i] = new double[nnodes];
            predecessors[i] = g.predecessors[i];    
        }
        for(int i = 0; i != nnodes; ++i){
            for(int j = 0; j != nnodes; ++j){
                graph[i][j] = g.graph[i][j];
            }
        }
    }

    int arr(int i, int j){ return j + i*reprCols -1;}

    std::pair<int,int> state(int s){
        int j = (s % reprCols) + 1;
        int i = (s + 1 - j)/reprCols;
        return std::make_pair(i,j);
    }

    void printPath(int node, int init){
        if(node == init){
            std::cout << node << std::endl;
        }else{
            std::cout << predecessors[node].first << " -> ";
            printPath(predecessors[node].first,init);
        }
    }

     void printPath(const std::pair<int,int>& node, const std::pair<int,int>& init){
        if(node.first == init.first && node.second == init.second){
            representation[node.first][node.second] = 'S';
        }else{
            std::cout << node.first << " " << node.second << std::endl;
            int i = path[arr(node.first,node.second)].first.first;
            int j = path[arr(node.first,node.second)].first.second;
            representation[node.first][node.second] = 'P';
            std::cout << i << " " <<  j  << std::endl;
            printPath(path[arr(node.first,node.second)].first,init);
        }
    }

    void printRepresentation(){
        for(int i = 0; i!= reprRows; ++i){
            for(int j = 0; j != reprCols; ++j){
                std::cout << representation[i][j];
            }
            std::cout << std::endl;
        }
    }

    double computeCost(const std::pair<int,int>& p1,const std::pair<int,int>& p2){
        return abs(p1.first - p2.first) + abs(p1.second - p2.second);
    }

    void uniform_cost_search(int init, int goal){
        //Implementation of the UCS algorithm in order to find the best possible paths for all 
        //nodes in a graph.
        //First step: Put in a priority queue the initial node, with cost 0.
        std::priority_queue<P,std::vector<P>, std::greater<P>> frontier;
        frontier.push(std::make_pair(0,init));
        predecessors[init] = std::make_pair(init,0); 
        //Now do expansion and iteration of the process of search
         std::cout << "Next to expand: ( " << frontier.top().second << " , " << frontier.top().first << ")" << std::endl;
        while(!frontier.empty()){
            double actual_cost = frontier.top().first;
            int actual_expansion = frontier.top().second;
            frontier.pop();
                if(actual_expansion != goal){
                    for(int i = 0; i != nnodes; ++i){
                        if(graph[actual_expansion][i] > 0){
                            frontier.push(std::make_pair((actual_cost+graph[actual_expansion][i]),i)); //put the successor in the graph
                            if(predecessors[i].second > (actual_cost+graph[actual_expansion][i])){
                                predecessors[i] = std::make_pair(actual_expansion,(actual_cost+graph[actual_expansion][i]));
                            }
                        }
                    }
                std::cout << "Next to expand: ( " << frontier.top().second << " , " << frontier.top().first << ")" << std::endl;
                }else{
                    std::cout << "Goal found with cost " << actual_cost << std::endl;
                    printPath(actual_expansion,init);
                    return;
                }
        }
    }

    void uniform_cost_search(const std::pair<int,int>& init, const std::pair<int,int>& goal){
        std::set<Q> frontier;  //create a set of nodes 
        frontier.insert(std::make_pair(0,init));
        path[arr(init.first,init.second)] = std::make_pair(init,0); 
        //Now do expansion and iteration of the process of search
    
        while(!frontier.empty()){
            std::pair<int,int> actual_expansion = frontier.begin()->second;
            double heuristic = computeCost(actual_expansion,goal);
            double actual_cost = frontier.begin()->first /*- heuristic*/;
            //std::cout << "Next to expand: ( " << frontier.begin()->second.first << " , " << frontier.begin()->second.second << ") with cost " << frontier.begin()->first << std::endl;
            frontier.erase(frontier.begin());
                if(actual_expansion.first != goal.first || actual_expansion.second != goal.second){
                    for(int i = 1; i >= -1; --i){
                        for(int j = 1; j >= -1; --j){
                            if(!(j == 0 && i == 0)){
                                std::pair<int,int> actual_near = std::make_pair(actual_expansion.first + i,actual_expansion.second + j);
                                if((actual_near.first >= 0 && actual_near.first < reprRows) && (actual_near.second >= 0 && actual_near.second < reprCols)){
                                    //std::cout << "insert: (" << actual_near.first << " , " << actual_near.second << " )" << std::endl;
                                    double cost = computeCost(actual_expansion,actual_near) /* + computeCost(actual_near,goal)*/;
                                    if(representation[actual_near.first][actual_near.second] == '#'){ cost = 60000; }
                                    frontier.insert(std::make_pair((actual_cost + cost ),actual_near)); //put the successor in the graph
                                    if( path[arr(i,j)].second > (actual_cost + cost)){
                                        path[arr(i,j)].second = actual_cost + cost;
                                        path[arr(i,j)].first = actual_expansion; //I set as the father of (i,j), the actual expansion
                                    }
                                }
                            }
                        }
                    }
                }else{
                    std::cout << "Goal found with cost " << actual_cost << " in [" << actual_expansion.first<< ","<< actual_expansion.second <<"] " << std::endl;
                    printPath(goal,init);
                    printRepresentation();
                    return;
                }
        }
    }
    

};


int main(){
    double** g;
    int nnodes = 7;
    g = new double*[nnodes];
    for(int i = 0; i != nnodes; ++i){
        g[i] = new double[nnodes];
    }
    for(int i = 0; i != nnodes; ++i){
        for(int j = 0; j != nnodes; ++j){
            g[i][j] = 0;
        }
    }

    //Graph *gr = new Graph(nnodes,g);
    Graph *gr = new Graph("op_space.txt");
    gr->printRepresentation();
    std::pair<int,int> init = std::make_pair(6,3);
    std::pair<int,int> goal = std::make_pair(6,25);
    gr->uniform_cost_search(init,goal);
    std::cout << gr->computeCost(init,goal) << " " << gr->computeCost(init,std::make_pair(5,4));
    for(int i = 0; i != nnodes; i++){
        delete [] g[i];
    }
    delete [] g;
    delete gr;
    return 0;
}