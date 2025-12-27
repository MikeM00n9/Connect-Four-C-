#include "connectFour.h"
#include <algorithm>
#include <limits>
//constructor
Game::Game() {
    board = new Board();
    curToken = 'R';
    winToken = ' ';
}
//destructor
Game::~Game() {
    delete board;
}

char Game::currentToken() const {
    return curToken;
}

const Board& Game::getBoard() const {
    return *board;
}

void Game::switchTurn() {
    curToken = (curToken == 'R') ? 'B' : 'R';
}
//player's moves
bool Game::playerMove(int col) {
    if (!board->validColumn(col) || !board->canDrop(col)) return false;
    bool ok = board->dropPiece(col, 'R');
    if (!ok) return false;
    checkAndSetWinner();
    if (winToken == ' ') switchTurn();
    return true;
}
//computer Moves
//can look for diangal moves but the favor
int Game::computerMove() {
    // the computer wins if it's possible
    int col = findWinningMove('B');
    if (col != -1) {
        board->dropPiece(col, 'B');
        checkAndSetWinner();
        if (winToken == ' ') switchTurn();
        return col;
    }
    // block the opponent if they can win by the next move not worklinf effecient 
    col = findWinningMove('R');
    if (col != -1) {
        board->dropPiece(col, 'B');
        checkAndSetWinner();
        if (winToken == ' ') switchTurn();
        return col;
    }
    // picks center if available
    int center = COLS / 2;
    if (board->canDrop(center)) {
        board->dropPiece(center, 'B');
        checkAndSetWinner();
        if (winToken == ' ') switchTurn();
        return center;
    }
    // simple pick
    col = simplePick();
    if (col != -1) {
        board->dropPiece(col, 'B');
        checkAndSetWinner();
        if (winToken == ' ') switchTurn();
    }
    return col;
}
//game over check
bool Game::isGameOver() const {
    return winToken != ' ';
}

char Game::winner() const {
    return winToken;
}

void Game::checkAndSetWinner() {
    if (board->checkWinForToken('R')) {
        winToken = 'R';
        return;
    }
    if (board->checkWinForToken('B')) {
        winToken = 'B';
        return;
    }
    if (board->isFull()) {
        winToken = 'T';
        return;
    }
    winToken = ' ';
}

int Game::findWinningMove(char token) const {
    for (int c = 0; c < COLS; ++c) {
        if (!board->canDrop(c)) continue;
        // simulate by copying board state to temp board
        int r;
        // find available row copy of getAvailableRow 
        for (r = ROWS - 1; r >= 0; --r) {
            if (board->getCell(r, c) == ' ') break;
        }
        if (r < 0) continue; // full

        // temp place token
        Board temp;
        // copy current board into temp
        for (int rr = 0; rr < ROWS; ++rr) {
            for (int cc = 0; cc < COLS; ++cc) {
                char ch = board->getCell(rr, cc);
                // If ch is space but rr==r and cc==c set token
                if (rr == r && cc == c) ch = token;
                // drop into temp find the available row in temp col and set directly by simulating drop
            }
        }
        
        for (int cc = 0; cc < COLS; ++cc) {
            // collect tokens top-to-bottom from original board into a vector
            vector<char> tokensInCol;
            for (int rr = 0; rr < ROWS; ++rr) {
                char ch = board->getCell(rr, cc);
                if (ch != ' ') tokensInCol.push_back(ch);
            }
            // clear column in temp by dropping nothing 
          
            for (char tkn : tokensInCol) {
                temp.dropPiece(cc, tkn);
            }
        }
        temp.dropPiece(c, token);
        // check if this move gives token a win
        if (temp.checkWinForToken(token)) return c;
    }
    return -1;
}

int Game::scoreColumn(int col, char token) const {
    if (!board->validColumn(col) || !board->canDrop(col)) return numeric_limits<int>::min();
    // create temp again 
    Board temp;
    for (int cc = 0; cc < COLS; ++cc) {
        vector<char> tokensInCol;
        for (int rr = 0; rr < ROWS; ++rr) {
            char ch = board->getCell(rr, cc);
            if (ch != ' ') tokensInCol.push_back(ch);
        }
        for (char tkn : tokensInCol) temp.dropPiece(cc, tkn);
    }
    // drop a token in  column
    temp.dropPiece(col, token);

    //number of 2 in a rows and 3 in a row for token minus opponent's potential
    int score = 0;
    char opp = (token == 'R') ? 'B' : 'R';

    // scans for every cell with token
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (temp.getCell(r, c) != token) continue;
            // check for small runs 
            // horizontal
            int run = 1;
            for (int cc = c + 1; cc < COLS && temp.getCell(r, cc) == token; ++cc) ++run;
            if (run >= 2) score += (run == 2 ? 10 : 50);
            // vertical
            run = 1;
            for (int rr = r + 1; rr < ROWS && temp.getCell(rr, c) == token; ++rr) ++run;
            if (run >= 2) score += (run == 2 ? 12 : 60);
            // diag down right
            run = 1;
            for (int k = 1; r + k < ROWS && c + k < COLS && temp.getCell(r + k, c + k) == token; ++k) ++run;
            if (run >= 2) score += (run == 2 ? 11 : 55);
            // diag up right
            run = 1;
            for (int k = 1; r - k >= 0 && c + k < COLS && temp.getCell(r - k, c + k) == token; ++k) ++run;
            if (run >= 2) score += (run == 2 ? 11 : 55);
        }
    }

    // penalize if opponent could win immediately after 
    int oppWin = -1;
    for (int cc = 0; cc < COLS; ++cc) {
        if (!temp.canDrop(cc)) continue;
        // replicate temp to temp2 to test player's immediate win
        Board temp2;
        for (int ccc = 0; ccc < COLS; ++ccc) {
            vector<char> tokensInCol2;
            for (int rr = 0; rr < ROWS; ++rr) {
                char ch = temp.getCell(rr, ccc);
                if (ch != ' ') tokensInCol2.push_back(ch);
            }
            for (char tkn : tokensInCol2) temp2.dropPiece(ccc, tkn);
        }
        temp2.dropPiece(cc, opp);
        if (temp2.checkWinForToken(opp)) {
            // opponent can win in response
            score -= 1000;
        }
    }

    // semi favor center columns
    int center = COLS / 2;
    int centerDist = abs(center - col);
    score -= centerDist * 2;

    return score;
}

int Game::simplePick() const {
    int bestCol = -1;
    int bestScore = numeric_limits<int>::min();
    for (int c = 0; c < COLS; ++c) {
        if (!board->canDrop(c)) continue;
        int sc = scoreColumn(c, 'B');
        if (sc > bestScore) {
            bestScore = sc;
            bestCol = c;
        }
    }
    // fallback to first available if none chosen
    if (bestCol == -1) {
        for (int c = 0; c < COLS; ++c) if (board->canDrop(c)) return c;
    }
    return bestCol;
}
