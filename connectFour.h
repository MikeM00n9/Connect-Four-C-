#pragma once


#include "board.h"
using namespace std;

class Game {
public:
   
    Game();//constrructor
    ~Game();//destructor
    char currentToken() const;//current player's token
    bool playerMove(int col);//player moves
    int computerMove();//computer's move
    bool isGameOver() const;//check is the game over
    char winner() const;//returns win
    const Board& getBoard() const;//returrns the board

private:
    Board* board;
    char curToken;
    char winToken; 
    void switchTurn();//swicth the turn
    int findWinningMove(char token) const;//look for a winning move
    int simplePick() const;//sikm pick for the computer
    int scoreColumn(int col, char token) const;
    void checkAndSetWinner();//updates wintoken basedc on the board state
};

