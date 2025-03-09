#include <iostream>
#include <vector>
#include <unordered_map>

const int WIN = 1, LOSE = 2, DRAW = 3, UNDECIDED = 4;

struct TicTacToe {
    int N;
    std::vector<std::vector<char>> board;

    TicTacToe(int n) : N(n), board(n, std::vector<char>(n, 0)) {}

    bool operator==(const TicTacToe& oth) const {
        if (N != oth.N)
            return false;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] != oth.board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    int game_status() const {
        for (int i = 0; i < N; i++) {
            bool is_hsame = board[i][0] != 0;
            bool is_vsame = board[0][i] != 0;
            for (int j = 1; j < N; j++) {
                if (board[i][0] != board[i][j])
                    is_hsame = false;
                if (board[0][i] != board[j][i])
                    is_vsame = false;
            }
            if (is_hsame || is_vsame)
                return WIN;
        }

        bool is_dsame = board[0][0];
        bool is_rdsame = board[0][N - 1];
        for (int i = 0; i < N; i++) {
            if (board[0][0] != board[i][i])
                is_dsame = false;
            if (board[0][N - 1] != board[i][N - 1 - i])
                is_rdsame = false;
        }
        if (is_dsame || is_rdsame)
            return WIN;
        
        bool is_full = true;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!board[i][j]) {
                    is_full = false;
                }
            }
        }
        if (!is_full)
            return UNDECIDED;
        
        return DRAW;
    }
};

struct TicTacToeHasher {
    size_t operator()(const TicTacToe& t) const {
        size_t h = 0;
        size_t pw = 1;
        for (int i = 0; i < t.N; i++) {
            for (int j = 0; j < t.N; j++) {
                if (t.board[i][j] == 'X')
                    h += pw;
                else if (t.board[i][j] == 'O')
                    h += 2 * pw;
                pw *= 3;
            }
        }
        return h;
    }
};

std::unordered_map<TicTacToe, int, TicTacToeHasher> cache;

int simulate(TicTacToe& game, char turn) {
    if (cache[game] != 0)
        return cache[game];
    
    char next_turn = (turn == 'X' ? 'O': 'X');
    int status = game.game_status();
    if (status != UNDECIDED) {
        return cache[game] = status == DRAW ? DRAW: LOSE;
    }

    bool is_drawable = false;
    bool is_winable = false;
    for (int i = 0; i < game.N; i++) {
        for (int j = 0; j < game.N; j++) {
            if (game.board[i][j]) {
                continue;
            }
            game.board[i][j] = turn;
            int best_case = simulate(game, next_turn);
            game.board[i][j] = 0;
            if (best_case == LOSE) {
                is_winable = true;
                break;
            }
            else if (best_case == DRAW) {
                is_drawable = true;
            }
        }
        if (is_winable)
            break;
    }

    if (is_winable) {
        return cache[game] = WIN;
    }
    else if (is_drawable) {
        return cache[game] = DRAW;
    }
    else {
        return cache[game] = LOSE;
    }
}

int main() {
    int N;
    std::cout << "Enter the size of gameboard (NxN, with N in-row for win): ";
    std::cin >> N;

    if (N > 4) {
        std::cout << "N larger than 4 is unsupported for now" << std::endl;
        return 0;
    }
    
    std::cout << "Solving, please wait..." << std::endl;
    
    TicTacToe game(N);
    int best_case = simulate(game, 'X');
    std::cout << "TicTacToe with " << N << "x" << N << " gameboard always ends with ";
    if (best_case == WIN)
        std::cout << "WIN";
    else if (best_case == LOSE)
        std::cout << "LOSE";
    else
        std::cout << "DRAW";
    std::cout << " for the first player." << std::endl;
}
