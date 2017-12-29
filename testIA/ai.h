//  Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef AI_H_
#define AI_H_

#include <iostream>
#include <string>
#include <vector>

#include "./board.h"

/*!
 * \brief converte to std::string some state.
 * \param s , the state to print.
 * \return string , the converted state.
 */
std::string printState(const STATE &s);

/*!
 * \brief converter to std::string some move.
 *param mv , the move to converter.
 *return string , move converted to std::string.
 */
std::string printMove(const Move &mv);

/*!
 * \brief show in console some board.
 * \param board, the board to show in console.
 */
void showBoard(const Board &board);

/*!
 * \brief function to toggle at current player
 * \param Integer representing the current player. ( 1 : player 1 , -1 : player 2)
 * \return the toggle player
 */
int playerToggle(int player);

/*!
 * \brief function to eval the board based on the players points.
 * \param 2 integers with values of the players points.
 * \return integer wiht difference between players . when the difference is long , return better points.
 */
int evalBoard(const Board &board , int player1 , int player2);

/*!
 * \brief function to generate all movement possibilities.
 * \param reference to board.
 * \return std::vector<Move> list containing all moves.
 */
std::vector<Move> generateMoves(const Board &board);

/*!
 * \brief Ordering function, prioritizing movements that generate points.
 * \param list of moves and the current board.
 * \return list prioritizing best moves. to generate more cuts on the search algorithm.
 */
void deterministValue(std::vector<Move> &mvs, Board &board);

void sortMoves(std::vector<Move>&mv);

/*!
 * \brief main function of the AI , based on the algorithm of cut alpha-beta.
 * \param a , integer representing the below limit search in alpa-beta algorithm.
 * \param b , integer representing the upper limit search in alpa-beta algorithm.
 * \param d , search deph.
 * \param board , the current to apply algorithm.
 * \param turn , the current player turn , 1 for player1 , -1 for player2.
 * \param PG1 , points of player1.
 * \param PG2 , points of player2.
 * \return integer representing the value of the board.
 */
int alphaBeta(int a , int b , int d , Board &board, bool turn, int P1G,
  int P2G);

Move playAI(Board &board, int d = 4, bool turn = 1, int P1G = 0, int P2G = 0);

#endif  // AI_H_
