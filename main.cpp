//help was from a University of West Florida alumni Jake Hutto Cybersecurity
#include <iostream>
#include "connectFour.h"
using namespace std;
int main() {
    cout << "Welcome to Connect Four (6 rows x 7 columns)\n";
    cout << "You are Red (R). Computer is Black (B).\n";
    Game game;
    while (!game.isGameOver()) {
        // display board
        game.getBoard().display();
        if (game.currentToken() == 'R') {
            // player's turn
            int colInput = -1;
            bool moved = false;
            while (!moved) {
                cout << "Enter column (1-" << COLS << ") to drop your R token, or 0 to quit: ";
                if (!(cin >> colInput)) {
                    // handle non integers input
                    cin.clear();
                    string junk;
                    
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }
                if (colInput == 0) {
                    cout << "Quitting. Goodbye.\n";
                    return 0;
                }
                colInput -= 1; // convert to 0 
                if (!game.getBoard().validColumn(colInput)) {
                    cout << "Column out of range. Try again.\n";
                    continue;
                }
                if (!game.getBoard().canDrop(colInput)) {
                    cout << "Column full. Try another column.\n";
                    continue;
                }
                moved = game.playerMove(colInput);
                if (!moved) cout << "Move failed. Try again.\n";
            }
        }
        else {
            // the computer's turn
            cout << "Computer is thinking...\n";
            int chosen = game.computerMove();
            if (chosen >= 0)
                cout << "Computer dropped in column " << (chosen + 1) << ".\n";
            else
                cout << "Computer couldn't find a move (unexpected).\n";
        }
    }

    // final board and result
    game.getBoard().display();
    char w = game.winner();
    if (w == 'R') {
        cout << "Congratulations — you (R) win!\n";
    }
    else if (w == 'B') {
        cout << "Computer (B) wins. Better luck next time.\n";
    }
    else if (w == 'T') {
        cout << "It's a tie!\n";
    }
    else {
        cout << "Game over.\n";
    }

    // prompt to play again
    char again = 'n';
    cout << "Play again? (y/n): ";
    cin >> again;
    if (again == 'y' || again == 'Y') {
        
        cout << "restart \n";
    }
    else {
        cout << "Thanks for playing. Goodbye.\n";
    }
    return 0;
}
