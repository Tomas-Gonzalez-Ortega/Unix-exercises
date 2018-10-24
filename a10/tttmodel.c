
/**
 *  \author    Jeremy at Langara
 *  \date      2018-04-01
 *  \copyright For use in Langara CPSC 1280 201810 only
 */

#include <string.h>

#include "tttmodel.h"

void ttt_init(TTT* model) {
	memset(model->board, ' ', sizeof(model->board));
	model->state = 'x';
}

bool ttt_isGameOver(TTT* model) {
	switch(model->state) {
		case 'X':
		case 'O':
		case 'C':
			return true;
	}
	static const int allWins[8][3] = {
		{0,1,2},{3,4,5},{6,7,8},
		{0,3,6},{1,4,7},{2,5,8},
		{0,4,8},{6,4,2} };

	const char* board = model->board;

	// seeking winner
	for(int i = 0; i < 8; i++) {
		const int *winRow = allWins[i];
		const char line[] = {
			board[winRow[0]], board[winRow[1]], board[winRow[2]] };
		if(line[0] != ' ' && line[0] == line[1] && line[0] == line[2]) {
			model->state = line[0] - 'a' + 'A';
			return true;
		}
	}

	// seeking cat/draw
	for(int i = 0; i < (int) sizeof(board); i++) {
		if(board[i] == ' ') { return false; }
	}
	model->state = 'C';
	return true;
}

void ttt_nextTurn(TTT* model) {
	ttt_isGameOver(model);
	switch(model->state) {
		case 'x':
			model->state = 'o';
			break;
		case 'o':
			model->state = 'x';
			break;
	}
}

bool ttt_isLegalMove(const TTT* model, char player, int pos) {
	return (player == 'x' || player == 'o')
		&& player == model->state
		&& pos >= 0
		&& pos < ((int) sizeof(model->board))
		&& model->board[pos] == ' ';
}

bool ttt_playMove(TTT *model, char player, int pos) {
	if(!ttt_isLegalMove(model, player, pos)) return false;
	model->board[pos] = player;
	ttt_nextTurn(model);
	return true;
}

char* ttt_stateMsg(const TTT* model) {
	switch(model->state) {
		case 'x': return "X's move.";
		case 'o': return "O's move.";
		case 'X': return "X wins!";
		case 'O': return "O wins!";
		case 'C': return "Cat's game.";
		default: return "Unknown game state.";
	}
}

void ttt_display(const TTT* model, FILE* out) {
	const char* board = model->board;

	fprintf(out,
		"\t╔═╤═╤═╗\n"
		"\t║%c│%c│%c║\n"
		"\t╟─┼─┼─╢\n"
		"\t║%c│%c│%c║   %s\n"
		"\t╟─┼─┼─╢\n"
		"\t║%c│%c│%c║\n"
		"\t╚═╧═╧═╝\n",
		board[0], board[1], board[2],
		board[3], board[4], board[5], ttt_stateMsg(model),
		board[6], board[7], board[8]);
}
