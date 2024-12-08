#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>
using namespace std;

// Structure to hold stats for each player
struct PlayerStats {
    int wins = 0;
    int losses = 0;
    int draws = 0;
};

// Global variables
const vector<char> Symbols = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
map<string, PlayerStats> stats;

// Function to validate player names
string getPlayerName(int playerNumber) {
    string name;
    while (true) {
        cout << "Enter name for Player " << playerNumber << " (letters only): ";
        getline(cin, name);

        // Check if the name contains only letters and spaces
        bool valid = true;
        for (char ch : name) {
            if (!isalpha(ch) && !isspace(ch)) {
                valid = false;
                break;
            }
        }

        if (valid && !name.empty()) {
            return name;
        } else {
            cout << "Invalid name. Please enter letters only." << endl;
        }
    }
}

// Function to initialize the game
void initializeGame(int& rows, int& cols, int& players, vector<string>& playerNames) {
    cout << "Board size will stay the same throughout the game" << endl;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    cout << "Enter the number of players (2-7): ";
    cin >> players;

    while (players < 2 || players > 7) {
        cout << "Invalid number of players. Enter a value between 2 and 7: ";
        cin >> players;
    }

    // Clear input buffer
    cin.ignore();

    // Get player names
    playerNames.clear();
    for (int i = 1; i <= players; ++i) {
        playerNames.push_back(getPlayerName(i));
    }
}

// Function to create a new board
vector<vector<char>> createBoard(int rows, int cols) {
    return vector<vector<char>>(rows, vector<char>(cols, ' '));
}

// Function to display the game board
void displayBoard(const vector<vector<char>>& board, const vector<string>& playerNames) {
    int rows = board.size();
    int cols = board[0].size();

    cout << "Players:" << endl;
    for (size_t i = 0; i < playerNames.size(); ++i) {
        cout << Symbols[i] << " - " << playerNames[i] << endl;
    }
    cout << endl;

    // Print column numbers (top)
    cout << "   ";
    for (int col = 1; col <= cols; ++col) {
        cout << col << "   ";
    }
    cout << endl;

    // Print rows with letter labels and grid
    char rowLabel = 'A';
    for (int r = 0; r < rows; ++r) {
        cout << "   ";
        for (int c = 0; c < cols; ++c) {
            cout << "----";
        }
        cout << "-" << endl;

        cout << rowLabel++ << " ";
        for (int c = 0; c < cols; ++c) {
            cout << "| " << board[r][c] << " ";
        }
        cout << "| " << rowLabel - 1 << endl;
    }

    // Bottom border
    cout << "   ";
    for (int c = 0; c < cols; ++c) {
        cout << "----";
    }
    cout << "-" << endl;

    // Print column numbers (bottom)
    cout << "   ";
    for (int col = 1; col <= cols; ++col) {
        cout << col << "   ";
    }
    cout << endl;
}

// Function to check for a winner (win condition fixed at 3 in a row)
bool checkWinner(const vector<vector<char>>& board, char symbol) {
    int rows = board.size();
    int cols = board[0].size();
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    const int winCondition = 3;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] == symbol) {
                for (auto [dr, dc] : directions) {
                    int count = 1;
                    int nr = r + dr, nc = c + dc;

                    while (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == symbol) {
                        count++;
                        if (count == winCondition) {
                            return true;
                        }
                        nr += dr;
                        nc += dc;
                    }
                }
            }
        }
    }
    return false;
}

// Function to check for a draw
bool checkDraw(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return false;
        }
    }
    return true;
}

// Function to handle a player's move
void playerMove(vector<vector<char>>& board, char symbol) {
    while (true) {
        char rowLabel;
        int col;
        cout << "Enter your move (e.g., A1): ";
        cin >> rowLabel >> col;

        int row = rowLabel - 'A';
        col -= 1;

        if (row >= 0 && row < board.size() && col >= 0 && col < board[0].size() && board[row][col] == ' ') {
            board[row][col] = symbol;
            break;
        } else {
            cout << "Invalid move. Try again." << endl;
        }
    }
}

// Function to update stats
void updateStats(map<string, PlayerStats>& stats, const vector<string>& playerNames, int winnerIndex, bool isDraw) {
    if (isDraw) {
        for (const auto& name : playerNames) {
            stats[name].draws++;
        }
    } else {
        for (int i = 0; i < playerNames.size(); ++i) {
            if (i == winnerIndex) {
                stats[playerNames[i]].wins++;
            } else {
                stats[playerNames[i]].losses++;
            }
        }
    }
}

// Function to display stats
void displayStats(const map<string, PlayerStats>& stats, int totalGames) {
    cout << "Total games played = " << totalGames << endl;
    cout << "------------------------------------------" << endl;
    cout << "| Player Name            | WIN | LOSS | DRAW |" << endl;
    cout << "------------------------------------------" << endl;

    for (const auto& [name, stat] : stats) {
        cout << "| " << name;
        cout << string(22 - name.length(), ' ');
        cout << "| " << stat.wins << "   | " << stat.losses << "    | " << stat.draws << "    |" << endl;
    }

    cout << "------------------------------------------" << endl;
}

// Main game loop
int main() {
    int totalGames = 0;
    vector<string> playerNames;
    int rows, cols, players;

    initializeGame(rows, cols, players, playerNames);

    while (true) {
        vector<vector<char>> board = createBoard(rows, cols);

        int currentPlayer = 0;
        bool gameWon = false, isDraw = false;

        while (!gameWon && !isDraw) {
            displayBoard(board, playerNames);
            cout << playerNames[currentPlayer] << "'s turn (" << Symbols[currentPlayer] << "):" << endl;
            playerMove(board, Symbols[currentPlayer]);

            gameWon = checkWinner(board, Symbols[currentPlayer]);
            isDraw = checkDraw(board);

            if (!gameWon && !isDraw) {
                currentPlayer = (currentPlayer + 1) % players;
            }
        }

        displayBoard(board, playerNames);

        if (gameWon) {
            cout << playerNames[currentPlayer] << " wins!" << endl;
            updateStats(stats, playerNames, currentPlayer, false);
        } else {
            cout << "It's a draw!" << endl;
            updateStats(stats, playerNames, -1, true);
        }

        totalGames++;
        displayStats(stats, totalGames);

        char playAgain;
        cout << "Do you want to play another game? (y/n): ";
        cin >> playAgain;

        if (tolower(playAgain) != 'y') {
            break;
        }
    }

    return 0;
}
