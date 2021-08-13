#include "minimax.h"

#include <vector>

struct node {
	~node() {
		for (node* c : children) {
			c->~node();
			delete c;
		}
	}
	const Tic_tac_toe game;
	std::vector<node*> children;
	std::pair<int, int> move;
	int score;
};

bool maximizing(const Tic_tac_toe& game) {
	return game.cur_player() == field::X;
}

int score(const Tic_tac_toe& game) {
	switch (game.get_status()) {
	case state::o_won: return std::numeric_limits<int>::min(); //minimizing
	case state::x_won: return std::numeric_limits<int>::max(); //maximizing
	case state::tied: return 0;
	}
	int ans = 0;
	for (int i = 0; i < 3; i += 2)//give 50 points for corner points.
		for (int j = 0; j < 3; j += 2)
			if (game.board[i][j] != field::empty)
				ans += (game.board[i][j] == field::X ? 50 : -50);
	for (int i = 0; i < 3; ++i)
		for(int j = (i % 2 == 0 ? 1 : 0); j < 3; j += 2)
			if (game.board[i][j] != field::empty)
				ans += (game.board[i][j] == field::X ? 30 : -30); //30 points for corners
	if (game.board[1][1] != field::empty) 
		ans += (game.board[1][1] == field::X ? 100 : -100); //100 points for middle.
	return ans;
}

void build_tree(node* parent, int depth, const int max_depth) {
	parent->children = {};
	if (parent->game.get_status() != state::in_game || depth > max_depth)
		return;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Tic_tac_toe copy{ parent->game };
			try {
				copy.move(i, j);
				copy.switch_players();
				parent->children.push_back(new node{ copy, {}, {i, j} });
			}
			catch (illegal_move) {
				//do nothing
			}
		}
	}
	for (node* n : parent->children)
		build_tree(n, depth + 1, max_depth);
}

void add_scores(node* tree) {
	if (tree->children.empty())
		tree->score = score(tree->game);
	else {
		for (node* n : tree->children)
			add_scores(n);
		if (maximizing(tree->game)) {
			tree->score = std::numeric_limits<int>::min();
			for (node* n : tree->children)
				if (n->score > tree->score)
					tree->score = n->score;
		}
		else {
			tree->score = std::numeric_limits<int>::max();
			for (node* n : tree->children)
				if (n->score < tree->score)
					tree->score = n->score;
		}
	}
}

//returns indexes i, j which represent a move. 
std::pair<int, int> best_move(const Tic_tac_toe& game) {
	node* tree = new node{ game, {} };
	build_tree(tree, 0, 3);
	add_scores(tree);
	std::pair<int, int> x;
	if (maximizing(game)) {
		node* best = tree->children[0];
		for (node* n : tree->children) {
			if (best->score < n->score)
				best = n;
		}
		x = best->move;
	}
	else {
		node* best = tree->children[0];
		for (node* n : tree->children)
			if (best->score > n->score)
				best = n;
		x = best->move;
	}
	delete tree;
	return x;
}
