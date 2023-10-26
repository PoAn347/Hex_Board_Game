#ifndef HEXBOARD_HPP
#define HEXBOARD_HPP

#include <iostream>
#include <vector>
#include "cellState.hpp"

using namespace std;


class HexBoard
{
private:

    int size;
    vector<vector<CellState>> board;
    vector<int> parent;
    vector<int> rank;
    int topSideSet;
    int bottomSideSet;
    int leftSideSet;
    int rightSideSet;


    // Function to find the root of a set
    int findSet(int x);

    //Function to merge two sets based on their ranks
    void unionSets(int x, int y);

    //Function to check if two sets are connected
    bool isConnected(int x, int y);

    // Function to merge the current cell with its neighboring cells of the same player
    void mergeNeighbors(int row, int col, CellState player);

public:

    //Default constructor
    //HexBoard();
    
    //Constructor with given size values
    HexBoard(int size);

    //Destructor
    ~HexBoard();

    //Function to make a move on the board
    void makeMove(int row, int col, CellState player);
    
    //Function to check if a move is valid
    bool isValidMove(int row, int col);

    //Function to display the current state of the board
    void displayBoard();

    void makeComputerMove(CellState CP, const HexBoard& originalBoard);

    vector<int> AI_move(CellState CP, const HexBoard& originalBoard, int times = 10000);

    void winner(CellState players);

    bool montecarlowinner(CellState computerPlayer);
};
#endif 