
/**
 * \author    Jeremy at Langara
 * \date      2018-04-01
 * \copyright For use in Langara CPSC 1280 201810 only
 */

#ifndef ___TTTmodel_h___
#define ___TTTmodel_h___

#include <stdio.h>
#include <stdbool.h>

/** The game state of TTT */
typedef struct {
	char board[9];	/**< the game board: [ xo] */
	char state;   	/**<
	              		[xo] indicates who's turn;
	              		[XO] indicates the winner;
	              		 [C] indicates cat's game. */
} TTT;


/** Initializes a game of Tic-Tac-Toe.
 * @param[in/out] model the game model
 */
void ttt_init(TTT* model);

/** Determines if a game is over.
 * @param[in/out] model the game model
 * @return true if the game is over, false otherwise
 */
bool ttt_isGameOver(TTT* model);

/** Determines if a move is legal.
 * @param[in] model the game model
 * @param[in] player [xo]
 * @param[in] pos [0-8] the board position to occupy
 * @return true if the move is legal, false otherwise
 */
bool ttt_isLegalMove(const TTT* model, char player, int pos);

/** The given player takes the given position. Only legal moves are executed.
 * @param[in/out] model the game model
 * @param[in] player [xo]
 * @param[in] pos [0-8] the board position to occupy
 * @return true if the move is legal, false otherwise
 */
bool ttt_playMove(TTT *model, char player, int pos);

/** Displays the game state on the given stream.
 * @param[in] model the game model
 * @param[in] out the stream on which to display state
 */
void ttt_display(const TTT* model, FILE* out);

#endif // ___TTTmodel_h___
