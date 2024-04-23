#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 10;
const int NUM_MINES = 10;

enum class CellStatus {
    Hidden,
    Revealed,
    Marked
};

enum class CellContent {
    Empty,
    Mine,
    Number
};

struct Cell {
    CellStatus status;
    CellContent content;
    int surroundingMines;
};

class Minesweeper {
private:
    vector<vector<Cell> > board;

    void placeMines() {
        srand(time(NULL));
        for (int i = 0; i < NUM_MINES; ++i) {
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            if (board[x][y].content != CellContent::Mine) {
                board[x][y].content = CellContent::Mine;
                incrementSurroundingMines(x, y);
            }
            else {
                --i;
            }
        }
    }

    void incrementSurroundingMines(int x, int y) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
                    board[nx][ny].content != CellContent::Mine) {
                    ++board[nx][ny].surroundingMines;
                    board[nx][ny].content = CellContent::Number;
                }
            }
        }
    }

    void reveal(int x, int y) {
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
            return;
        if (board[x][y].status == CellStatus::Revealed)
            return;
        board[x][y].status = CellStatus::Revealed;
        if (board[x][y].content == CellContent::Empty) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    reveal(x + dx, y + dy);
                }
            }
        }
    }

public:
    Minesweeper() {
        board.resize(BOARD_SIZE, vector<Cell>(BOARD_SIZE, { CellStatus::Hidden, CellContent::Empty, 0 }));
        placeMines();
    }

    void displayBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j].status == CellStatus::Hidden) {
                    cout << "-";
                }
                else if (board[i][j].status == CellStatus::Marked) {
                    cout << "F";
                }
                else {
                    if (board[i][j].content == CellContent::Mine) {
                        cout << "*";
                    }
                    else if (board[i][j].content == CellContent::Number) {
                        cout << board[i][j].surroundingMines;
                    }
                }
                cout << " ";
            }
            cout << endl;
        }
    }

    void revealCell(int x, int y) {
        if (board[x][y].status == CellStatus::Hidden) {
            reveal(x, y);
        }
    }

    void markCell(int x, int y) {
        if (board[x][y].status == CellStatus::Hidden) {
            board[x][y].status = CellStatus::Marked;
        }
        else if (board[x][y].status == CellStatus::Marked) {
            board[x][y].status = CellStatus::Hidden;
        }
    }
};

int main() {
    Minesweeper game;
    cout << "Welcome to Minesweeper!" << endl;
    cout << "Enter coordinates to reveal (x y):" << endl;

    while (true) {
        game.displayBoard();
        int x, y;
        cin >> x >> y;
        if (cin.fail()) {
            cout << "Invalid input. Please enter two integers." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            cout << "Invalid coordinates. Please enter coordinates within the board size." << endl;
            continue;
        }
        char action;
        cout << "Enter 'r' to reveal or 'm' to mark/unmark:" << endl;
        cin >> action;
        if (action == 'r') {
            game.revealCell(x, y);
        }
        else if (action == 'm') {
            game.markCell(x, y);
        }
        else {
            cout << "Invalid action. Please enter 'r' to reveal or 'm' to mark/unmark." << endl;
        }
    }

    return 0;
}