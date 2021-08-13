#include "tic_tac_toe.h"
#include "minimax.h"
#include <string>

Tic_tac_toe::Tic_tac_toe() : status(state::in_game), current_player(field::X) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = field::empty;
}

Tic_tac_toe::Tic_tac_toe(const Tic_tac_toe& other) :status(other.status), current_player(other.current_player) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = other.board[i][j];
}

void Tic_tac_toe::move(int i, int j) {
    if (i < 0 || i > 2 || j < 0 || j > 2)
        throw out_of_range_move{};
    if (board[i][j] != field::empty)
        throw non_empty_field_move{};
    board[i][j] = current_player;
    if (winning_move(i, j))
        status = current_player == field::X ? state::x_won : state::o_won;
    else if (is_full())
        status = state::tied;
}

std::ostream& operator<<(std::ostream& os, const Tic_tac_toe& game) {
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

void human_move(Tic_tac_toe& game, std::ostream& os = std::cout, std::istream& is = std::cin) {
    int i = 0, j = 0;
    while (1) {
        is >> i >> j;
        try {
            game.move(i, j);
            break;
        }
        catch (out_of_range_move) {
            os << "Both indices should be in [0, 2]\n";
        }
        catch (non_empty_field_move) {
            os << "Field is already taken, enter a new pair of indices\n";
        }
    }
}

void computer_move(Tic_tac_toe& game) {
    std::pair<int, int> x = best_move(game);
    game.move(x.first, x.second);
}

void play_game(Tic_tac_toe& game, gamemode mode, std::ostream& os = std::cout, std::istream& is = std::cin) {
    bool human_turn = true;
    while (game.get_status() == state::in_game) {
        os << game << "\nPlayer: " << static_cast<char>(game.cur_player()) << ", Enter a row and a column to make a move\n";
        if (human_turn)
            human_move(game, os, is);
        else
            computer_move(game);
        if (mode == gamemode::singleplayer)
            human_turn = !human_turn;
        game.switch_players();
    }
}

void report_ending(const Tic_tac_toe& game, std::ostream& os = std::cout) {
    os << game << '\n';
    switch (game.get_status()) {
    case state::o_won:
        os << "\nCongratulations: Player " << static_cast<char>(field::O) << " won the game!\n";
        break;
    case state::x_won:
        os << "\nCongratulations: Player " << static_cast<char>(field::X) << " won the game!\n";
        break;
    case state::tied:
        os << "\nTied game.\n";
        break;
    default:
        os << "\nUnexpected ending.\n";
    }
}

void play_tic_tac_toe(gamemode mode) {
    Tic_tac_toe game;
    play_game(game, mode);
    report_ending(game);
}
