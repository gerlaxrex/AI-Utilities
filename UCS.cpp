#include<iostream>
#include<queue>
#include<map>
#include<utility>
#include<functional>
#include<string>
#include<cmath>
#include<fstream>
#include<set>
#include<stdexcept>

typedef std::pair<double,int> P;
typedef std::pair<double,std::pair<int,int>> Q;

class Graph{
private:
    double** graph; //costs matrix
    char** representation; //Eventual graphical representation of the graph
    std::map<int,std::pair<int,double>> predecessors; //Map for node -> (previous node, cost) in normal graph representation
    std::map<int,std::pair<std::pair<int,int>,double>> path; //Map for point -> (previous point, cost) in space graph representation
    int nnodes,reprRows, reprCols;
public:

    //Constructor (for costs)
    Graph(int n, double**g):nnodes(n){
        graph = new double*[nnodes];
        for(int i = 0; i != nnodes; ++i){
            graph[i] = new double[nnodes];
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
            representation = new char*[reprCols];
            for(int i = 0; i != reprCols; ++i){
                representation[i] = new char[reprRows];
            }
            std::cout << reprRows << " " << reprCols << std::endl;
            for(int i = 0; i != reprRows; ++i){
                for(int j = 0; j != reprCols; ++j){
                    strm >> representation[i][j];
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
    }

    Graph(const Graph& g):nnodes(g.nnodes){
        graph = new double*[nnodes];
        predecessors = g.predecessors;

        for(int i = 0; i != nnodes; ++i){
            graph[i] = new double[nnodes];    
        }

        for(int i = 0; i != nnodes; ++i){
            for(int j = 0; j != nnodes; ++j){
                graph[i][j] = g.graph[i][j];
            }
        }
    }

    //Function used to represent the place in the graph in a unique number
    int arr(int i, int j){ return j + i*reprCols;}

    //Inverse of the arr function
    std::pair<int,int> state(int s){
        int j = (s % reprCols);
        int i = (s - j)/reprCols;
        return std::make_pair(i,j);
    }

    //Function used to print the path traversed by the agent in a simple graph(Problem here)
    void printPath(int node, int init){
        if(predecessors.at(node).first == init){
            std::cout << init << "-> " << node << " -> ";
        }else{
            try{
                auto prev = predecessors.at(node).first; 
                printPath(prev, init);
                std::cout << node << " -> ";
            }catch(std::out_of_range e){
                std::cout << node <<" index not found in map!" << std::endl;
            }
        }
    }

    //Function used when we have a plane with states to be traversed (Overloaded)
     void printPath(const std::pair<int,int>& node, const std::pair<int,int>& init){
        if(node.first == init.first && node.second == init.second){
            representation[node.first][node.second] = 'S';
        }else{
            try{
                int n = arr(node.first,node.second);
                //std::cout << node.first << " " << node.second <<" "<< n <<std::endl;
                int i = path.at(n).first.first;
                int j = path.at(n).first.second;
                representation[node.first][node.second] = 'P';
                //std::cout << i << " " <<  j  << std::endl;
                printPath(path.at(n).first,init);
            }catch(std::out_of_range e){
                std::cout << "index (" << node.first << "," << node.second <<  ") not found in map!"<< std::endl;
            }
        }
    }

    //Function to print the graph in a graphical way
    void printRepresentation(){
        for(int i = 0; i!= reprRows; ++i){
            for(int j = 0; j != reprCols; ++j){
                std::cout << representation[i][j];
            }
            std::cout << std::endl;
        }
    }

    //Function that computes the cost of doing a path from one point to another
    double computeCost(const std::pair<int,int>& p1,const std::pair<int,int>& p2){
        return abs(p1.first - p2.first) + abs(p1.second - p2.second);
    }


    //Function for the UCS in a simple graph with nodes
    void uniform_cost_search(int init, int goal){
        //Implementation of the UCS algorithm in order to find the best possible paths for all 
        //nodes in a graph.
        //First step: Put in a priority queue the initial node, with cost 0.
        std::priority_queue<P,std::vector<P>, std::greater<P>> frontier;  // priority queue of (cost,node)
        frontier.push(std::make_pair(0,init));
        predecessors[init] = std::make_pair(init,0); 
        //Now do expansion and iteration of the process of search
        std::cout << "Next to expand: " << frontier.top().second  << std::endl;
        while(!frontier.empty()){
            double actual_cost = frontier.top().first;
            int actual_expansion = frontier.top().second;
            frontier.pop();
            if(actual_expansion != goal){
                for(int i = 0; i != nnodes; ++i){
                    if(graph[actual_expansion][i] > 0){
                        frontier.push(std::make_pair((actual_cost+graph[actual_expansion][i]),i)); //put the successor in the graph
                        try{
                            if(predecessors.at(i).second > (actual_cost+graph[actual_expansion][i])){
                                predecessors[i] = std::make_pair(actual_expansion,(actual_cost + graph[actual_expansion][i]));
                            }
                        }catch(std::out_of_range e){
                            predecessors[i] = std::make_pair(actual_expansion,(actual_cost + graph[actual_expansion][i]));
                            continue;
                        }
                    }
                }
            std::cout << "Next to expand: " << frontier.top().second  << std::endl;    
            }else{
                std::cout << "Goal found with cost " << actual_cost << std::endl;
                printPath(actual_expansion,init);
                return;
            }
        }
    }

    //Function defined for an operational space
    void uniform_cost_search(const std::pair<int,int>& init, const std::pair<int,int>& goal){
        std::set<Q> frontier;  //create a set of nodes 
        frontier.insert(std::make_pair(0,init));
        path[arr(init.first,init.second)] = std::make_pair(init,0); 
        //Now do expansion and iteration of the process of search
    
        while(!frontier.empty()){
            std::pair<int,int> actual_expansion = frontier.begin()->second;
            double actual_cost = frontier.begin()->first;
            frontier.erase(frontier.begin());
            if(actual_expansion.first != goal.first || actual_expansion.second != goal.second){
                for(int i = 1; i >= -1; --i){
                    for(int j = 1; j >= -1; --j){
                        if(!(j == 0 && i == 0)){
                            std::pair<int,int> actual_near = std::make_pair(actual_expansion.first + i,actual_expansion.second + j);
                            if((actual_near.first >= 0 && actual_near.first < reprRows) && (actual_near.second >= 0 && actual_near.second < reprCols)){                                    //std::cout << "insert: (" << actual_near.first << " , " << actual_near.second << " )" << std::endl;
                                    double cost = computeCost(actual_expansion,actual_near) /* + computeCost(actual_near,goal)*/;
                                if(representation[actual_near.first][actual_near.second] == '#'){ cost = 60000; }
                                frontier.insert(std::make_pair((actual_cost + cost ),actual_near)); //put the successor in the graph
                                try{
                                    if( path.at(arr(actual_expansion.first+i,actual_expansion.second+j)).second > (actual_cost + cost)){
                                        path[arr(actual_expansion.first+i,actual_expansion.second+j)] = std::make_pair(actual_expansion,(actual_cost + cost)); //I set as the father of (i,j), the actual expansion
                                    }
                                }catch(std::out_of_range e){
                                    path[arr(actual_expansion.first+i,actual_expansion.second+j)] = std::make_pair(actual_expansion,(actual_cost + cost)); 
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
    int nnodes = 5;
    
    g = new double*[nnodes];
    for(int i = 0; i != nnodes; ++i){
        g[i] = new double[nnodes];
    }
    for(int i = 0; i != nnodes; ++i){
        for(int j = 0; j != nnodes; ++j){
            g[i][j] = 0;
        }
    }

    //For this graph the path is 0 3 2 4       
    g[0][1] = 1;
    g[0][2] = 6;
    g[0][3] = 2;
    g[0][4] = 6;
    g[1][4] = 6;
    g[2][3] = 1;
    g[3][2] = 2;
    g[2][4] = 1;

    //Graph *gr = new Graph(nnodes,g);
    
    Graph *gr = new Graph("op_space.txt");
    
    gr->uniform_cost_search(std::make_pair(5,3),std::make_pair(5,25));
    
    for(int i = 0; i != nnodes; i++){
        delete [] g[i];
    }
    
    delete [] g;
    delete gr;
    return 0;
}