//  Copyright (c) 2017 Richard Games. All rights reserved.

#include <math.h>

#include <iostream>
#include <vector>

#include "./agent.h"
#include "./ai.h"
#include "./board.h"
#include "./game.h"

Agent::Agent(QObject *parent) : QObject(parent) { }

size_t Agent::numOfNodes(int empty_squares, int depth) const {
  size_t accumulate = 1;
  size_t possibilities = 2 * empty_squares;

  // 8 * 6 * 4 * .. * n - 2 for n > 2

  for (int i = 0; i < depth; ++i) {
    accumulate *= possibilities;
    possibilities -= 2;

    if (possibilities == 0)
      break;
  }
  return accumulate;
}

int Agent::recommendedDepth(const DataGame *data) const {
  int depth =  data->level;
  size_t bound = pow(2, 28);  // 268.435.456
  size_t num_of_nodes = numOfNodes(data->board->empty_squares(), depth);

  // Reduce depth while the future tree has many "nodes".

  if (num_of_nodes > bound) {
    while (numOfNodes(data->board->empty_squares(), depth) > bound) {
      depth--;
    }
  } else {
    if (data->level == 4) {
      size_t next;
      while ((next = numOfNodes(data->board->empty_squares(), depth + 1)) <
      bound) {
        size_t current = numOfNodes(data->board->empty_squares(), depth);

        // If it reached the tree's background (There is not more nodes)
        if (current == next)
          break;
        depth++;
      }
    }
  }
  return depth;
}

void Agent::aiVsAi(DataGame * data) const {
  int points;

  while (data->status != Status::Over) {
    Board copy = *data->board;
    data->lastMove = playAI(copy, data->level, data->turn, data->score1,
      data->score2);
    data->board->setState(data->lastMove);

    if (data->board->statusGame() == GAME_STATUS::END) {
      data->status = Status::Over;
    }

    emit(repaint());

    points = data->lastMove.dValue;

    if (points) {
      if (data->turn) {
        data->score1 += points;
      } else {
        data->score2 += points;
      }
    } else {
      data->turn = !data->turn;
    }

    if (points) {
        int row = data->lastMove.row;
        int col = data->lastMove.col;
        Game::markBoardSquares(data->board->squaresOsoAround(row, col), *data);
    }
    emit(repaint());
  }
  emit(gameOver());
}

void Agent::playAgent(DataGame * data) const {
  if (data->status == Status::Over)
    return;

  data->status = Status::PC;  // Reserve status

  int points = 1;
  while (points && data->status != Status::Over) {
    /* Use copy because main thread is painting original board and
       playAI() receives a reference and is modifying it.
    */
    Board copy = *data->board;
    data->lastMove = playAI(copy, recommendedDepth(data), data->turn,
      data->score1, data->score2);
    data->board->setState(data->lastMove);

    if (data->board->statusGame() == GAME_STATUS::END) {
      data->status = Status::Over;
      emit(gameOver());
    }
    emit(repaint());
    points = data->lastMove.dValue;
    data->score2 += points;

    if (points) {
      int row = data->lastMove.row;
      int col = data->lastMove.col;
      Game::markBoardSquares(data->board->squaresOsoAround(row, col), *data);
    } else {
      data->turn = !data->turn;
    }
    emit(repaint());
  }
  data->status = Status::Start;
}
