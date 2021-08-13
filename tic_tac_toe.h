#pragma once
#include <iostream>

struct illegal_move {};
struct out_of_range_move : illegal_move {};
struct non_empty_field_move : illegal_move {};

enum class field : char { empty = ' ', X = 'x', O = 'o' };
enum class state { in_game, tied, x_won, o_won };
enum class gamemode {multiplayer, singleplayer};

class Tic_tac_toe {
public:
    Tic_tac_toe();
    Tic_tac_toe(const Tic_tac_toe& other);
    state get_status() const { return status; }
    field cur_player() const { return current_player; }
    void move(int i, int j);
    void switch_players() { current_player = (current_player == field::X ? field::O : field::X); }
    friend std::ostream& operator<<(std::ostream& os, const Tic_tac_toe& game);
    friend int score(const Tic_tac_toe& game);
private:
    state status;
    field current_player;
    field board[3][3];
    bool whole_row(int row) const;
    bool whole_col(int col) const;
    bool diagonal(int i, int j) const;
    bool winning_move(int i, int j) const;
    bool is_full() const;
};

void play_tic_tac_toe(gamemode mode);

