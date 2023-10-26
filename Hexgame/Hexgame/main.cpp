
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>

#include "cellState.hpp"
#include "hexBoard.hpp"

using namespace std;


int main() {

    srand(time(nullptr));

    // Create a customized Hex board
    int customerSize;
    cout << "How big hex game do you want to play? Recommended size: 5~11" << endl;
    cin >> customerSize;

    HexBoard board(customerSize);

    // Prompt the player to choose their color
    char color;
    cout << "Choose your color (B for Blue, R for Red): ";
    cin >> color;

    CellState player;
    CellState computer;

    if (color == 'B' || color == 'b') {
        player = CellState::PlayerBlue;
        computer = CellState::PlayerRed;
    }
    else if (color == 'R' || color == 'r') {
        player = CellState::PlayerRed;
        computer = CellState::PlayerBlue;
    }
    else {
        cout << "Invalid color selection. Defaulting to Blue." << endl;
        player = CellState::PlayerBlue;
        computer = CellState::PlayerRed;
    }

    // Make moves on the board
    int row, col;

    board.displayBoard();

    while (true) {

        if (player == CellState::PlayerBlue) {

            cout << "Enter the row and column for your move x y = :(ex. 1 1) ";
            cin >> row >> col;


            if (!board.isValidMove(row, col)) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }


            board.makeMove(row, col, player);
            board.displayBoard();

            // Check if the player has won
            // ...
            board.winner(player);

            // Computer player makes a move
            cout << "Computer played:" << endl;
            board.makeComputerMove(computer, board);

            board.displayBoard();

            // Check if the computer player has won
            // ...
            board.winner(computer);
        }
        else {

            cout << "Computer played:" << endl;
            board.makeComputerMove(computer, board);

            board.displayBoard();

            // Check if the computer player has won
            // ...
            board.winner(computer);

            cout << "Enter the row and column for your move x y = :(ex. 1 1) ";
            cin >> row >> col;


            if (!board.isValidMove(row, col)) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }


            board.makeMove(row, col, player);
            board.displayBoard();

            // Check if the player has won
            // ...
            board.winner(player);
        }

    }

    return 0;
}