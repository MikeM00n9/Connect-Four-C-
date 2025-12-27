#include "board.h"
#include <iostream>

Board::Board() {
    grid = new char* [ROWS];
    for (int r = 0; r < ROWS; ++r) {
        grid[r] = new char[COLS];
        for (int c = 0; c < COLS; ++c) grid[r][c] = ' ';
    }
}

Board::~Board() {
    for (int r = 0; r < ROWS; ++r) delete[] grid[r];
    delete[] grid;
}

void Board::display() const {
    cout << "\n";
    //rows
    for (int r = 0; r < ROWS; ++r) {
        cout << "|";
        //columns
        for (int c = 0; c < COLS; ++c) {
            cout << " " << grid[r][c] << " |";
        }
        cout << "\n";
    }
    // column numbers
    cout << " ";
    for (int c = 1; c <= COLS; ++c) cout << " " << c << "  ";
    cout << "\n\n";
}

bool Board::validColumn(int col) const {
    return col >= 0 && col < COLS;
}

int Board::getAvailableRow(int col) const {
    if (!validColumn(col)) return -1;
    // drop to the bottom-most empty row higher row index
    for (int r = ROWS - 1; r >= 0; --r) {
        if (grid[r][col] == ' ') return r;
    }
    return -1;
}

bool Board::dropPiece(int col, char token) {
    int row = getAvailableRow(col);
    if (row == -1) return false;
    grid[row][col] = token;
    return true;
}

bool Board::canDrop(int col) const {
    return getAvailableRow(col) != -1;
}

bool Board::isFull() const {
    for (int c = 0; c < COLS; ++c) {
        if (canDrop(c)) return false;
    }
    return true;
}

char Board::getCell(int r, int c) const {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return '?';
    return grid[r][c];
}

int Board::countDirectionRecursive(int r, int c, int dr, int dc, char token) const {
    // out of bounds or not matching token
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return 0;
    if (grid[r][c] != token) return 0;
    // count current cell recursive count in the same direction
    return 1 + countDirectionRecursive(r + dr, c + dc, dr, dc, token);
}

bool Board::checkWinForToken(char token) const {
    // for each cell that matches token it will checks 4 directions right, down, diag down-right, diag up-right
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c] != token) continue;
            // check right and left 
            int count = countDirectionRecursive(r, c, 0, 1, token) + countDirectionRecursive(r, c, 0, -1, token) - 1;
            if (count >= 4) return true;
            // check down & up
            count = countDirectionRecursive(r, c, 1, 0, token) + countDirectionRecursive(r, c, -1, 0, token) - 1;
            if (count >= 4) return true;
            // diagonal down-right & up-left
            count = countDirectionRecursive(r, c, 1, 1, token) + countDirectionRecursive(r, c, -1, -1, token) - 1;
            if (count >= 4) return true;
            // diagonal up-right & down-left
            count = countDirectionRecursive(r, c, -1, 1, token) + countDirectionRecursive(r, c, 1, -1, token) - 1;
            if (count >= 4) return true;
        }
    }
    return false;
}
