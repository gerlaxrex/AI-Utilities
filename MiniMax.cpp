#include<iostream>
#include<vector>
#include<utility>
#include<stdexcept>
#include<list>
#include<algorithm>
#include<functional>
#include<sstream>
#include<string>

// In this program a simple tic-tac-toe algorithm is implemented in order to simulate a game between two players
// (both CPUs or one Human against a CPU)
typedef std::pair<int, std::pair<int,int>> P;

class Game{
private:
    char grid[3][3]; //the grid of the game.
    bool alphabeta;
    //Function for resetting the element on the grid
    inline void reset(int row, int col){
        grid[row][col] = ' ';
    }

    //Check if a cell is avaliable
    inline bool checkAvailable(int row, int col){
        return (grid[row][col] != ' ' ? false : true);
    }

    //Swaps the actual sign on the game
    inline void swapSign(char& sign){
        if(sign == 'X'){
            sign = 'O';
        }else{
            sign = 'X';
        }
    }

    //Checks if the grid is full
    bool checkDraw(){
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                if(grid[i][j] == ' ') return false;
            }
        }
        return true;
    }

    //Checks if the player with a given sign wins
    bool checkWin(char sign){
        if (grid[0][0] == sign && grid[1][1] == sign && grid[2][2] == sign) return true;
        if (grid[0][2] == sign && grid[1][1] == sign && grid[2][0] == sign) return true;
        for(int i = 0; i != 3; ++i){
            if (grid[i][i] == sign && grid[(i+1)%3][i] == sign && grid[(i+2)%3][i] == sign) return true;
            if (grid[i][i] == sign && grid[i][(i+1)%3] == sign && grid[i][(i+2)%3] == sign) return true;
        }
        return false;
    }

    //Function to print the Grid
    void printGrid(){
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                std::cout << grid[i][j] << " | "; 
            }
            std::cout << std::endl;
        }
    }

    //Get an element of the grid
    const char getElement(int row, int col) const {
        return grid[row][col];
    }

    //Sets an element on the grid
    char& setElement(char c, int row, int col){
        if(checkAvailable(row,col)){
            grid[row][col] = c;
            return grid[row][col];
        }else{
            std::stringstream out;
            out << "Cell (" << row << " , " << col << ") is not available. Try another one.\n"; 
            throw std::runtime_error(out.str());
        }
    }

    //Recursive function for the decision
    int MiniMax(char sign,bool initial,int& computations){
        swapSign(sign);
        int value;
        std::pair<int,int> chosenAction;
        std::list<std::pair<int,int>> freeList; //list of actual freePositions
        //Initalize the freeList        
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                if(grid[i][j] == ' '){
                    freeList.push_back(std::make_pair(i,j));
                }
            }
        }

        value = (sign == 'X'? -100 : 100);

        
        if(checkWin('X')){
            return 1;
        }else if(checkWin('O')){
            return -1;
        }else if(checkDraw()){
            return 0;
        }else{
        //Check all possible actions and apply recursion
            while(!freeList.empty()){
                auto el = freeList.front();
                freeList.pop_front();
                setElement(sign,el.first,el.second);
                int v = MiniMax(sign,false,computations);
                if(sign == 'X' && value < v || sign == 'O' && value > v){
                    value = v;
                    chosenAction = std::make_pair(el.first,el.second);
                }
                reset(el.first,el.second);
                ++computations;
            }
        }

        if(initial){
            setElement(sign,chosenAction.first,chosenAction.second);
            std::cout << "\nDone! It's your turn now!" << std::endl;
            std::cout << computations << " computations done." << std::endl;
        }
        return value;
    }

    //Recursive function with alpha-beta pruning, in the first call they are set to -100 and 100
    int MiniMax(char sign,bool initial,int alpha, int beta,int& computations){
        swapSign(sign);
        int value,localAlpha, localBeta;
        localAlpha = alpha;
        localBeta = beta;
        std::pair<int,int> chosenAction;
        std::list<std::pair<int,int>> freeList; //list of actual freePositions
        //Initalize the freeList        
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                if(grid[i][j] == ' '){
                    freeList.push_back(std::make_pair(i,j));
                }
            }
        }

        value = (sign == 'X'? -100 : 100);

        
        if(checkWin('X')){
            return 1;
        }else if(checkWin('O')){
            return -1;
        }else if(checkDraw()){
            return 0;
        }else{
        //Check all possible actions and apply recursion
            while(!freeList.empty()){
                if(localAlpha < localBeta){
                    auto el = freeList.front();
                    freeList.pop_front();
                    setElement(sign,el.first,el.second);
                    int v = MiniMax(sign,false,localAlpha,localBeta,computations);
                    if(sign == 'X' && value < v || sign == 'O' && value > v){
                        value = v;
                        if(sign == 'X'){
                            localAlpha = value;
                        }else{
                            localBeta = value;
                        }
                        chosenAction = std::make_pair(el.first,el.second);
                    }
                    reset(el.first,el.second);
                    ++computations;
                }else{
                    freeList.pop_front();
                }
            }
        }

        if(initial){
            setElement(sign,chosenAction.first,chosenAction.second);
            std::cout << "\nDone! It's your turn now!" << std::endl;
            std::cout << computations <<" computations done." << std::endl;
        }
        return value;
    }

    void playHH(){
        bool win = false;
        bool retry = false;
        char current_sign = 'X';
        int i, j;

        while(!win){
            
            //Insertion of the position in the grid
            do{
                std::cout << "Moving player " << current_sign << " into: ";
                std::cin >> i;
                std::cin >> j;
                try{
                    setElement(current_sign,i,j);
                    retry = false;
                }catch(std::runtime_error err){
                    std:: cout << err.what();
                    retry = true;
                }
            }while(retry);
            
            //Print the State
            printGrid();

            //Win or Draw checking
            if(checkWin(current_sign)){
                std::cout << "\nPlayer " << current_sign << " wins!" << std::endl;
                return;
            }else if(checkDraw()){
                std::cout << "\nDraw! None wins..." << std::endl;
            }

            //Swapping of signs
            swapSign(current_sign);
        }
    }

    void playHC(){
        bool win = false;
        bool retry = false;
        char current_sign = 'X';
        int i, j;
        while(!win){
            //Insertion of the position in the grid
            do{
                std::cout << "Moving player " << current_sign << " into: ";
                std::cin >> i;
                std::cin >> j;
                try{
                    setElement(current_sign,i,j);
                    retry = false;
                }catch(std::runtime_error err){
                    std:: cout << err.what();
                    retry = true;
                }
            }while(retry);
            
            //Print the State
            printGrid();

            //Win or Draw checking
            if(checkWin(current_sign)){
                std::cout << "\nPlayer " << current_sign << " wins!" << std::endl;
                return;
            }else if(checkDraw()){
                std::cout << "\nDraw! None wins..." << std::endl;
            }

            std::cout << "\nCPU is thinking..." << std::endl;
            
            if(this->alphabeta){
                int computations = 0;
                MiniMax(current_sign,true,-100,100,computations); //Using Alpha-Beta pruning
            }else{
                int computations = 0;
                MiniMax(current_sign,true,computations);          //When not using Alpha-Beta pruning
            }

            printGrid();

            swapSign(current_sign);
            if(checkWin(current_sign)){
                std::cout << "\nPlayer O wins!" << std::endl;
                return;
            }else if(checkDraw()){
                std::cout << "\nDraw! None wins..." << std::endl;
                return;
            }
            swapSign(current_sign);
        }
    }

public:
    
    //Default constructor
    Game():alphabeta(false){
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                grid[i][j] = ' ';
            }
        }
    }

    Game(bool alphabeta):alphabeta(alphabeta){
        for(int i = 0; i != 3; ++i){
            for(int j = 0; j != 3; ++j){
                grid[i][j] = ' ';
            }
        }
    }

    void start(){
        int response;
        char replay;
        do{
        std::cout << "Welcome! This is a Tic-tac-toe game implementing MINIMAX Algorithm." << std::endl;
        std::cout << "Please, select the play mode:\n\t1. Human vs. Human\n\t2.Human vs. CPU\n(insert number as input)\n>";
        std::cin >> response;
        while(response < 1 || response > 2){
            std::cout << "Please, select the play mode:\n\t1. Human vs. Human\n\t2.Human vs. CPU\n(insert number as input)\n>";
            std::cin >> response;
        }
        if(response == 1) playHH();
        if(response == 2) playHC();
        std::cout << "Game Finished!" << std::endl;
        std::cout << "Do you want to play again?(y or n)\n>";
        std::cin >> replay;
        }while(replay == 'y');
    }
};

int main(){
    Game* g = new Game(false);
    g->start();
    delete g;
    return 0;
}