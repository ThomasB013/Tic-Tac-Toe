#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct illegal_move{};
struct out_of_range_move : illegal_move{};
struct non_empty_field_move : illegal_move {};

enum class field : char { empty = ' ', X = 'x', O = 'o' };

class Tic_tac_toe {
public:
    Tic_tac_toe();
    void move(int i, int j);
    bool winning_move(int i, int j) const;
    bool is_full() const;
    
    friend ostream& operator<<(ostream& os, const Tic_tac_toe& game);
    field cur_player() const { return current_player; }
    void switch_players() { current_player = (current_player == field::X ? field::O : field::X); }
    

private:
    bool game_over;
    field current_player;
    field board[3][3];
    bool whole_row(int row) const;
    bool whole_col(int col) const;
    bool diagonal(int i, int j) const;
};

Tic_tac_toe::Tic_tac_toe() :game_over(false), current_player(field::X) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = field::empty;
}

void Tic_tac_toe::move(int i, int j) {
    if (i < 0 || i > 2 || j < 0 || j > 2)
        throw out_of_range_move{};
    if (board[i][j] != field::empty)
        throw non_empty_field_move{};
    board[i][j] = current_player;
}

ostream& operator<<(ostream& os, const Tic_tac_toe& game) {
    os << (char)-38 << (char)-60 << (char)-62 << (char)-60 << (char)-62 << (char)-60 << (char)-65 << '\n'
        << (char)-77;
    for (int i = 0; i < 3; ++i)
        os << static_cast<char>(game.board[0][i]) << (char)-77;
    for (int j = 1; j < 3; ++j) {
        os << '\n' << (char)-61 << (char)-60 << (char)-59 << (char)-60 << (char)-59 << (char)-60 << (char)-76 << '\n'
            << (char)-77;
        for (int i = 0; i < 3; ++i)
            os << static_cast<char>(game.board[j][i]) << (char)-77;
    }
    os << '\n' << (char)-64 << (char)-60 << (char)-63 << (char)-60 << (char)-63 << (char)-60 << (char)-39;
    return os;
}

bool Tic_tac_toe::whole_row(int row) const {
    for (int j = 0; j < 3; ++j)
        if (board[row][j] != current_player)
            return false;
    return true;
}

bool Tic_tac_toe::whole_col(int col) const {
    for (int i = 0; i < 3; ++i)
        if (board[i][col] != current_player)
            return false;
    return true;
}

bool Tic_tac_toe::diagonal(int i, int j) const {
    return (board[0][0] == current_player && board[1][1] == current_player && board[2][2] == current_player
        || board[2][0] == current_player && board[1][1] == current_player && board[0][2] == current_player);
}

bool Tic_tac_toe::winning_move(int i, int j) const {
    return whole_row(i) || whole_col(j) || diagonal(i, j);
}

bool Tic_tac_toe::is_full() const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == field::empty)
                return false;
    return true;
}

void play_tic_tac_toe() {
    Tic_tac_toe game;
    int i = 0, j = 0;
    while (!game.winning_move(i, j) && !game.is_full()) {
        game.switch_players();
        cout << game << '\n';
        cout << "Player: " << static_cast<char>(game.cur_player()) << ", Enter a row and a column to make a move\n";
        while (1) {
            cin >> i >> j;
            try {
                game.move(i, j);
                break;
            }
            catch (out_of_range_move) {
                cout << "Both indices should be in [0, 2]\n";
            }
            catch (non_empty_field_move) {
                cout << "Field is already taken, enter a new pair on indices\n";
            }
        }
    }
    if (game.winning_move(i, j))
        cout << game << "\n\nCongratulatoins: Player " << static_cast<char>(game.cur_player()) << " won the game!\n";
    else
        cout << game << "\n\nTied game.\n";
}


int main() {
    play_tic_tac_toe();
}
