#include "hexBoard.hpp"
#include <random>

//Public

//Default constructor
//HexBoard::HexBoard() {}

//Constructor with given size values
HexBoard::HexBoard(int size) : size(size), board(size, vector<CellState>(size, CellState::Empty))
{
    // Initialize disjoint-set data structure
    int totalSets = (size * size) + 4; // Number of cells + 4 side sets
    parent.resize(totalSets);
    rank.resize(totalSets, 0);

    for (int i = 0; i < totalSets; ++i) 
    {
        parent[i] = i;
    }

    // Set indices for side sets
    topSideSet = size * size;
    bottomSideSet = size * size + 1;
    leftSideSet = size * size + 2;
    rightSideSet = size * size + 3;
}

//Destructor
HexBoard::~HexBoard()
{

}

//Function to make a move on the board
void HexBoard::makeMove(int row, int col, CellState player)
{
    if (isValidMove(row, col)) 
    {

        if (board[row][col] == CellState::Empty) 
        {

            board[row][col] = player;

            // Merge the current cell with its neighboring cells of the same player
            mergeNeighbors(row, col, player);


        }
        else 
        {
            cout << "Cell is occupied. Choose another one" << endl;

            int new_row;
            int new_col;
            cout << "Enter the row and column for your move x y = :(ex. 1 1) ";
            cin >> new_row >> new_col;
            makeMove(new_row, new_col, player);
        }


    }
    else 
    {
        cout << "Choosing Cell is out of range. Choose another one" << endl;
        int new_row;
        int new_col;
        cout << "Enter the row and column for your move x y = :(ex. 1 1) ";
        cin >> new_row >> new_col;
        makeMove(new_row, new_col, player);
    }
}

//Function to check if a move is valid
bool HexBoard::isValidMove(int row, int col)
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

//Function to display the current state of the board
void HexBoard::displayBoard()
{
    // Display the labels for Player Red (left and right sides)
    cout << " ";
    for (int i = 0; i < size; ++i) 
    {
        cout << i;
        if (i != size - 1) 
        {
            cout << " B ";
        }
    }
    cout << endl;

    for (int i = 0; i < size; ++i) 
    {
        // Indent rows to represent the hexagonal shape
        cout << string(2 * i, ' ');

        // Display the label for Player Red (left side)
        cout << "  ";

        for (int j = 0; j < size; ++j) 
        {
            switch (board[i][j]) 
            {
            case CellState::Empty:
                cout << ".";
                break;
            case CellState::PlayerBlue:
                cout << "B";
                break;
            case CellState::PlayerRed:
                cout << "R";
                break;
            }

            if (j < size - 1) 
            {
                cout << " - ";
            }
        }

        // Display the label for Player Red (right side)
        cout << "  ";
        cout << i;

        cout << endl;

        // Draw line
        if (i < size - 1) 
        {
            cout << string(2 * i + 3, ' ');
            for (int j = 0; j < size - 1; ++j) 
            {
                cout << "\\ / ";
            }
            cout << "\\  R";
            cout << endl;
        }
    }
}

void HexBoard::makeComputerMove(CellState CP, const HexBoard& originalBoard)
{
    int row, col;
    vector<int> ans;
    ans = AI_move(CP, originalBoard);
    row = ans[0];
    col = ans[1];
    cout << row << " " << col << endl;
    makeMove(row, col, CP);
}

vector<int> HexBoard::AI_move(CellState CP, const HexBoard& originalBoard, int times)
{
    vector<vector<int>> coordinator;
    HexBoard copiedBoard(*this);
    CellState player_mc;
    CellState computer_mc;
    vector<int> result_computer(originalBoard.size * originalBoard.size, 0);
    vector<int> result_player(originalBoard.size * originalBoard.size, 0);
    int computer_win = 0, player_win = 0;


    // Find empty cells and push them into 2D vector
    for (int i = 0; i < originalBoard.size; ++i) 
    {
        for (int j = 0; j < originalBoard.size; ++j) 
        {
            if (copiedBoard.board[i][j] == CellState::Empty) 
            {
                coordinator.push_back({ i, j });
            }
        }
    }

    // Create Monte Carlo simulation players
    if (CP == CellState::PlayerBlue) 
    {
        computer_mc = CellState::PlayerBlue;
        player_mc = CellState::PlayerRed;
    }
    else 
    {
        computer_mc = CellState::PlayerRed;
        player_mc = CellState::PlayerBlue;
    }


    for (int t = 0; t < times; ++t) 
    {
        copiedBoard = *this; // Reset the copied board to the original state
        // Random number generator
        random_device rd;
        mt19937 gen(rd());

        // Shuffling the numbers
        shuffle(coordinator.begin(), coordinator.end(), gen);

        for (int c = 0; c < coordinator.size() / 2; ++c) 
        {
            int x = coordinator[c][0];
            int y = coordinator[c][1];
            copiedBoard.makeMove(x, y, computer_mc);
            copiedBoard.mergeNeighbors(x, y, computer_mc);
        }

        for (int c = coordinator.size() / 2; c < coordinator.size(); ++c) 
        {
            int x = coordinator[c][0];
            int y = coordinator[c][1];
            copiedBoard.makeMove(x, y, player_mc);
            copiedBoard.mergeNeighbors(x, y, player_mc);
        }

        //copiedBoard.displayBoard();

        if (copiedBoard.montecarlowinner(computer_mc)) 
        {
            computer_win += 1;

            for (int c = 0; c < coordinator.size() / 2; ++c) 
            {
                int x = coordinator[c][0];
                int y = coordinator[c][1];
                result_computer[x * size + y] += 1;
            }
        }

        /*
        if (copiedBoard.montecarlowinner(player_mc)) 
        {
            player_win += 1;

            for (int c = 0; c < coordinator.size() / 2; ++c) 
            {
                int x = coordinator[c][0];
                int y = coordinator[c][1];
                result_player[x * size + y] += 1;
            }
        }
        */


    }
    //cout << "Computer wins ? in 10000 times: " << computer_win << endl;

    auto maxElement = std::max_element(result_computer.begin(), result_computer.end());
    int index = std::distance(result_computer.begin(), maxElement);

    /*
    int index;

    if (computer_win >= player_win) 
    {

    }
    else 
    {
        auto maxElement = std::max_element(result_player.begin(), result_player.end());
        index = std::distance(result_player.begin(), maxElement);
    }
    */

    int row = index / size;
    int col = index % size;
    vector<int> coordinates = { row, col };
    //cout << row << col << endl;
    return coordinates;
}

void HexBoard::winner(CellState players)
{
    if (players == CellState::PlayerBlue && isConnected(topSideSet, bottomSideSet)) 
    {
        cout << "Player Blue has won!" << endl;
        exit(0);
    }
    else if (players == CellState::PlayerRed && isConnected(leftSideSet, rightSideSet)) 
    {
        cout << "Player Red has won!" << endl;
        exit(0);
    }
}

bool HexBoard::montecarlowinner(CellState computerPlayer)
{
    if (computerPlayer == CellState::PlayerBlue && isConnected(topSideSet, bottomSideSet)) 
    {
        //cout << "Computer Player Blue has won!" << endl;
        return true;
    }
    else if (computerPlayer == CellState::PlayerRed && isConnected(leftSideSet, rightSideSet)) 
    {
        //cout << "Computer Player Red has won!" << endl;
        return true;
    }

    return false;
}

//Private
// Function to find the root of a set
int HexBoard::findSet(int x)
{
    if (x != parent[x]) 
    {
        parent[x] = findSet(parent[x]);
    }

    return parent[x];
}

//Function to merge two sets based on their ranks
void HexBoard::unionSets(int x, int y)
{
    int rootX = findSet(x);
    int rootY = findSet(y);

    if (rank[rootX] < rank[rootY]) 
    {
        parent[rootX] = rootY;
    }
    else if (rank[rootX] > rank[rootY]) 
    {
        parent[rootY] = rootX;
    }
    else 
    {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

//Function to check if two sets are connected
bool HexBoard::isConnected(int x, int y)
{
    return findSet(x) == findSet(y);
}

// Function to merge the current cell with its neighboring cells of the same player
void HexBoard::mergeNeighbors(int row, int col, CellState player)
{
    int cellIndex = row * size + col;

    if (player == CellState::PlayerBlue) 
    {
        if (row == 0) 
        {
            // Merge with the top-side set
            unionSets(cellIndex, topSideSet);
        }
        else if (row == size - 1) 
        {
            // Merge with the bottom-side set
            unionSets(cellIndex, bottomSideSet);
        }
    }
    else if (player == CellState::PlayerRed) 
    {
        if (col == 0) 
        {
            // Merge with the left-side set
            if (leftSideSet == -1) 
            {
                leftSideSet = cellIndex;
            }
            else 
            {
                unionSets(cellIndex, leftSideSet);
            }
        }
        else if (col == size - 1) 
        {
            // Merge with the right-side set
            if (rightSideSet == -1) 
            {
                rightSideSet = cellIndex;
            }
            else 
            {
                unionSets(cellIndex, rightSideSet);
            }
        }
    }

    // Merge with neighboring cells of the same player
    const vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, 1}, {1, -1} };
    
    for (const auto& dir : directions) 
    {
        int newRow = row + dir.first;
        int newCol = col + dir.second;
        if (isValidMove(newRow, newCol) && board[newRow][newCol] == player) 
        {
            int neighborIndex = newRow * size + newCol;
            unionSets(cellIndex, neighborIndex);
        }
    }
}