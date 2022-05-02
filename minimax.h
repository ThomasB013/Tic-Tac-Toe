#pragma once
#include <vector>
#include <limits>

struct missing_method {};
struct missing_score_method : missing_method {};
struct missing_maximizing_method : missing_method {};
struct missing_transition_method : missing_method {};
struct missing_stop_condition_method : missing_method {};
struct missing_possible_moves_method : missing_method {};

struct score_depth {
	int score;
	unsigned int depth;
};

bool better_score(score_depth new_score, score_depth old_score, bool maximizing);

template<typename State, typename Move>
class minimax {
public:
	minimax() = delete;
	minimax(const State& starting_state, unsigned int depth);
	Move next_move();
	void set_max_depth(unsigned int depth) { max_depth = depth; }
	void set_score_method(int (*f)(const State& state)) { score = f; }
	void set_maximizing_method(bool (*f)(const State& state)) { maximizing = f; }
	void set_transition_method(void (*f)(State& state, Move move)) { transition = f; }
	void set_possible_moves_method(std::vector<Move>(*f)(const State& state)) { possible_moves = f; }
	void set_stop_condition_method(bool (*f)(const State& state)) { stop_condition = f; }
private:
	struct node {
		~node();
		const State state;
		std::vector<node*> children;
		Move move;
		score_depth score;
	};
	void check_functions_present();
	void add_scores(node* tree, unsigned int depth);
	void build_tree(node* parent, unsigned int depth);
	const State& starting_state;
	unsigned int max_depth;
	int (*score)(const State& state) = nullptr;	//score the states
	bool (*maximizing)(const State& state) = nullptr;	//determine if a state is maximizing or not.
	bool (*stop_condition)(const State& state) = nullptr; //determine if the stopping condition for a state is met.
	void (*transition)(State& state, Move move) = nullptr; //execute a transition based on a move.
	std::vector<Move>(*possible_moves)(const State& state) = nullptr; //extract all possible moves based on a current state.
};

//I would normally place all of this in minimax.cpp, but I get errors when compiling on visual studio,
//so I placed it here:

template<typename State, typename Move>
minimax<State, Move>::minimax(const State& state, unsigned int depth) : starting_state(state), max_depth(depth) {}

template<typename State, typename Move>
minimax<State, Move>::node::~node() {
	for (node* n : children)
		delete n;
}

template<typename State, typename Move>
void minimax<State, Move>::check_functions_present() {
	if (score == nullptr)
		throw missing_score_method{};
	if (maximizing == nullptr)
		throw missing_maximizing_method{};
	if (transition == nullptr)
		throw missing_transition_method{};
	if (stop_condition == nullptr)
		throw missing_stop_condition_method{};
	if (possible_moves == nullptr)
		throw missing_possible_moves_method{};
}

template<typename State, typename Move>
void minimax<State, Move>::add_scores(node* tree, unsigned int depth) {
	if (tree->children.empty())
		tree->score = { score(tree->state), depth };
	else {
		for (node* n : tree->children)
			add_scores(n, depth + 1);
		const bool max = maximizing(tree->state);
		tree->score = { max ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max(), std::numeric_limits<unsigned int>::max() };
		for (node* n : tree->children)
			if (better_score(n->score, tree->score, max))
				tree->score = n->score;
	}
}

template<typename State, typename Move>
void minimax<State, Move>::build_tree(node* parent, unsigned int depth) {
	parent->children = {};
	if (stop_condition(parent->state) || depth > max_depth)
		return;
	for (Move m : possible_moves(parent->state)) {
		State copy{ parent->state };
		transition(copy, m);
		parent->children.push_back(new node{ copy, {}, m });
	}
	for (node* n : parent->children)
		build_tree(n, depth + 1);
}

template<typename State, typename Move>
Move minimax<State, Move>::next_move() {
	check_functions_present();
	node* tree = new node{ starting_state, {} };
	build_tree(tree, 0);
	add_scores(tree, 0);
	node* best = tree->children[0]; //Safely assume that there is at least one child. next_move() should not be called on a state without transitions.
	for (node* n : tree->children)
		if (better_score(n->score, best->score, maximizing(starting_state)))
			best = n;
	const Move x{ best->move }; //Copy so that we can delete tree.
	delete tree;
	return x;
}
