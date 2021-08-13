#include "minimax.h"


bool better_score(score_depth new_score, score_depth old_score, bool maximizing) {
	if (maximizing)
		return (new_score.score > old_score.score || old_score.score == new_score.score && old_score.depth > new_score.depth);
	else
		return (new_score.score < old_score.score || old_score.score == new_score.score && old_score.depth > new_score.depth);
}