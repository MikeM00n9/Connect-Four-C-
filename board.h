#pragma once


#include <vector>
using namespace std;

const int ROWS = 6;
const int COLS = 7;


class Board {
public:
   
    Board();//constructor
    ~Board();//deconstructor
    void display() const;//display the board
    bool dropPiece(int col, char token);//drop the token ina column
    bool isFull() const;//check if it drop a token in th column
    char getCell(int r, int c) const;//checks bounds for row and column
    bool checkWinForToken(char token) const;//check for a win
    bool validColumn(int col) const;//check if the column is valid
    bool canDrop(int col) const;//check if you drop the token in the column

private:
    char** grid;    
    int getAvailableRow(int col) const;//finds the lowest empty spot in the given column
    int countDirectionRecursive(int r, int c, int dr, int dc, char token) const;//counts how many tokens ina direction
};


