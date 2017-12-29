//  Copyright (c) 2017 Richard Games. All rights reserved.

#include <time.h>

#include <algorithm>

#include <cstdlib>
#include <functional>
#include <string>

#include "./ai.h"

using std::cout;
using std::endl;

int counter = 0;

std::string printState(const STATE &s) {
  if (s == STATE::EMPTY)
    return "EMPTY";
  if (s == STATE::ERROR)
    return "ERROR";
  if (s == STATE::O)
    return "O";
  if (s == STATE::S)
    return "S";
  return "UNDEFINED";
}

std::string printMove(const Move &mv) {
  std::string out = "Move :";

  out += std::to_string(mv.row) + " - ";
  out += std::to_string(mv.col) + ", ";
  out += "L: " + printState(mv.state) + ", ";
  out += "D: " + std::to_string(mv.dValue) + ", ";
  out += "H: " + std::to_string(mv.hValue) + "\n";

  return out;
}

void showBoard(const Board &board) {
  for (int i = 0; i < board.rowsTam(); ++i) {
    for (int j = 0; j < board.colsTam(); ++j) {
      if ( board.statusSquare(j, i) == STATE::S)
        std::cout << "| S ";
      else if (board.statusSquare(j, i) == STATE::O)
        std::cout << "| O ";
      else
        std::cout << "|   ";
    }
    std::cout << std::endl;
  }
}

int playerToggle(int player) {
  return player*-1;
}

int evalBoard(const Board &board , int player1, int player2) {
  return (player1 - player2);
}

std::vector<Move> generateMoves(const Board &board) {
  std::vector<Move> moves;
  int rows = board.rowsTam();
  int cols = board.colsTam();

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (board.statusSquare(i, j) == STATE::EMPTY) {
        moves.push_back(Move(i, j, STATE::O));
        moves.push_back(Move(i, j, STATE::S));
      }
    }
  }
  return moves;
}

void deterministValue(std::vector<Move> &mvs, Board &board) {
  for (Move &mv : mvs) {
    board.setState(mv);
    mv.dValue = board.osoWordAround(mv);
    board.unSetState(mv);
  }
}

void sortMoves(std::vector<Move> &mvs) {
  std::sort(mvs.rbegin(), mvs.rend(), std::less<Move>());
}

/*

function alphabeta(node, depth, α, β, Player)
    if  depth = 0 or node is a terminal node
        return the heuristic dValue of node
    if  Player = MaxPlayer
        for each child of node
            α := max(α, alphabeta(child, depth-1, α, β, not(Player) ))
            if β ≤ α
                break                             (* Beta cut-off *)
        return α
    else
        for each child of node
            β := min(β, alphabeta(child, depth-1, α, β, not(Player) ))
            if β ≤ α
                break                             (* Alpha cut-off *)
        return β


(* Llamada INicial: *)
alphabeta(ORIGEN NODO, depth, -infinity, +infinity, MaxPlayer
*/

int alphaBeta(int alpha, int beta, int depth, Board &board, bool turn,
int player1, int player2) {
  counter++;

  if (depth == 0 || board.statusGame() == GAME_STATUS::END)
      return evalBoard(board, player1, player2);

  std::vector<Move> movements(generateMoves(board));

  for (Move &move : movements) {
    board.setState(move);
    move.dValue = board.osoWordAround(move.row, move.col);
    int points = move.dValue;

    if (points > 0) {
      if (turn)
        alpha = std::max(alpha, alphaBeta(alpha, beta, depth - 1, board, turn,
          player1 + points , player2));
      else
        beta = std::min(beta, alphaBeta(alpha, beta, depth - 1, board, turn,
          player1, player2 + points));
    } else {  // points == 0
      if (turn)
        alpha = std::max(alpha, alphaBeta(alpha , beta, depth - 1, board, !turn,
          player1 , player2));
      else
        beta = std::min(beta, alphaBeta(alpha, beta, depth - 1, board, !turn,
          player1, player2));
    }

    if (turn) {
      if (beta <= alpha) {
        board.unSetState(move);
        break;
      }
    } else {
      if (beta <= alpha) {
        board.unSetState(move);
        break;
      }
    }
    board.unSetState(move);
  }
  return turn ? alpha : beta;
}

Move playAI(Board &board, int d, bool turn, int P1G, int P2G) {
  if (board.statusGame() == GAME_STATUS::END) {
      return Move();
  }
  std::vector<Move> movements = generateMoves(board);
  deterministValue(movements, board);
  sortMoves(movements);

  /*

  int maxDValue = (*std::max_element(movements.begin(),
    movements.end())).dValue;

  movements.erase(std::remove_if(movements.begin(), movements.end(),
    [&] (const Move& mv) {
      return mv.dValue < maxDValue;
    }), movements.end());

  */

  int hValue, dValue;
  int maxHValue = -1000;
  int minHValue = 1000;

  for (Move &move : movements) {
    board.setState(move);
    dValue = move.dValue;

    if (dValue > 0) {
      if (turn)  // maximize
        hValue = alphaBeta(-500, 500, d - 1, board, turn, P1G + dValue, P2G);
      else    // minimize
        hValue = alphaBeta(-500, 500, d - 1, board, turn, P1G, P2G + dValue);
    } else {  // move.dValue == 0
      hValue = alphaBeta(-500, 500, d - 1, board, !turn, P1G, P2G);
    }

    if (hValue < minHValue) {
      minHValue = hValue;
    }

    if (hValue > maxHValue) {
      maxHValue = hValue;
    }

    move.hValue = hValue;
    board.unSetState(move);
  }

  std::function<bool(const Move& mv)> lambda;

  if (turn) {
    lambda = [&] (const Move& mv) { return mv.hValue < maxHValue; };
  } else {
    lambda = [&] (const Move& mv) { return mv.hValue > minHValue; };
  }

  movements.erase(std::remove_if(movements.begin(), movements.end(), lambda),
    movements.end());

  /*
  cout << "\nMejores movimientos\n" << endl;

  int j = 1;
  for (auto i : movements) {
    cout << j++ << " " << printMove(i);
  }
  */

  srand(time(0));
  int mod = movements.empty() ? 1 : movements.size();
  return movements[rand() % mod];
}

