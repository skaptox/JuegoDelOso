//  Copyright (c) 2017 Richard Games. All rights reserved.

#include <QVector>
#include "./board.h"

Board::Board() {
  rows = cols = 3;
  squaresCount = 0;
  board = new STATE * [rows];
  for (int i = 0; i < rows; ++i)
    board[i] = new STATE[cols];

  for (int i = 0; i < rows ; ++i)
    for (int j = 0; j < cols ; ++j)
      board[i][j] = STATE::EMPTY;
}

Board::Board(int row, int col): rows(row), cols(col) {
  squaresCount = 0;
  board = new STATE * [rows];
  for (int i = 0; i < rows; ++i)
    board[i] = new STATE[cols];

  for (int i = 0; i < rows ; ++i)
    for (int j = 0; j < cols ; ++j)
      board[i][j] = STATE::EMPTY;
}

Board::Board(const Board &b) {
  squaresCount = 0;
  rows = b.rowsTam();
  cols = b.colsTam();

  board = new STATE * [rows];
  for (int i = 0; i < rows; ++i)
    board[i] = new STATE[cols];

  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      board[i][j] = b.statusSquare(i, j);
}

Board::~Board() {
  for (int i = 0; i < rows; ++i)
    delete [] board[i];
  delete [] board;
}

GAME_STATUS Board::statusGame() const {
  return (squaresCount == rows * cols) ? GAME_STATUS::END : GAME_STATUS::PLAY;
}

STATE Board::statusSquare(int r, int c) const {
  if (r >= rows || c >= cols)
    return STATE::ERROR;
  return board[r][c];
}

int Board::osoWordAround(const Move &mv) const {
  return osoWordAround(mv.row, mv.col);
}

int Board::osoWordAround(int r, int c) const {
  if (r > rows || c > cols)
    return 0;

  int count = 0;
  if (statusSquare(r, c) == STATE::O) {
    if (r-2 >= 0)
      if (board[r-2][c] == STATE::O && board[r-1][c] == STATE::S)
        count++;
    if (r+2 < rows)
      if (board[r+2][c] == STATE::O && board[r+1][c] == STATE::S)
        count++;
    if (c-2 >= 0)
      if (board[r][c-2] == STATE::O && board[r][c-1] == STATE::S)
        count++;
    if (c+2 < cols)
      if (board[r][c+2] == STATE::O && board[r][c+1] == STATE::S)
        count++;
    if (r-2 >= 0 && c-2 >= 0)
      if (board[r-2][c-2] == STATE::O && board[r-1][c-1] == STATE::S)
        count++;
    if (r+2 < rows && c-2 >= 0)
      if (board[r+2][c-2] == STATE::O && board[r+1][c-1] == STATE::S)
        count++;
    if (r+2 < rows  && c+2 < cols)
      if (board[r+2][c+2] == STATE::O && board[r+1][c+1] == STATE::S)
        count++;
    if (r-2 >= 0 && c+2 < cols)
      if (board[r-2][c+2] == STATE::O && board[r-1][c+1] == STATE::S)
        count++;
  }

  if (statusSquare(r, c) == STATE::S) {
    if (r-1 >= 0 && r+1 < rows)
      if (board[r-1][c] == STATE::O && board[r+1][c] == STATE::O)
        count++;
    if (c-1 >= 0 && c+1 < cols)
      if (board[r][c-1] == STATE::O && board[r][c+1] == STATE::O)
        count++;
    if (r-1 >= 0 && r+1 < rows && c-1 >= 0 && c+1 <cols) {
      if (board[r-1][c-1] == STATE::O && board[r+1][c+1] == STATE::O)
        count++;
      if (board[r+1][c-1] == STATE::O && board[r-1][c+1] == STATE::O)
        count++;
    }
  }
  return count;
}

int Board::setState(const int &r, const int &c, const STATE &s) {
  if (board[r][c] != STATE::EMPTY)
    return 1;
  board[r][c] = s;
  squaresCount++;
  return 0;
}

int Board::setState(const Move &m) {
  if (board[m.row][m.col] != STATE::EMPTY)
    return 2;
  board[m.row][m.col] = m.state;
  squaresCount++;
  return 0;
}

int Board::unSetState(const Move &m) {
  if (board[m.row][m.col] == STATE::EMPTY)
    return 1;
  board[m.row][m.col] = STATE::EMPTY;
  squaresCount--;
  return 0;
}

int Board::unSetState(int r, int c) {
  if (board[r][c] == STATE::EMPTY)
    return 1;
  board[r][c] = STATE::EMPTY;
  squaresCount--;
  return 0;
}

int Board::resetBoard() {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      board[i][j] = STATE::EMPTY;

  squaresCount = 0;
  return 0;
}

std::vector<Move> Board::squaresOsoAround(int r, int c) const {
  if (r > rows || c > cols)
    return std::vector<Move>();

  QVector<Move> squares;
  if (statusSquare(r, c) == STATE::O) {
    if (r-2 >= 0)
      if (board[r-2][c] == STATE::O && board[r-1][c] == STATE::S)
        squares << Move(r-2, c) << Move(r-1, c);
    if (r+2 < rows)
      if (board[r+2][c] == STATE::O && board[r+1][c] == STATE::S)
        squares << Move(r+2, c) << Move(r+1, c);
    if (c-2 >= 0)
      if (board[r][c-2] == STATE::O && board[r][c-1] == STATE::S)
        squares << Move(r, c-2) << Move(r, c-1);
    if (c+2 < cols)
      if (board[r][c+2] == STATE::O && board[r][c+1] == STATE::S)
        squares << Move(r, c+2) << Move(r, c+1);
    if (r-2 >= 0 && c-2 >= 0)
      if (board[r-2][c-2] == STATE::O && board[r-1][c-1] == STATE::S)
        squares << Move(r-2, c-2) << Move(r-1, c-1);
    if (r+2 < rows && c-2 >= 0)
      if (board[r+2][c-2] == STATE::O && board[r+1][c-1] == STATE::S)
        squares << Move(r+2, c-2) << Move(r+1, c-1);
    if (r+2 < rows  && c+2 < cols)
      if (board[r+2][c+2] == STATE::O && board[r+1][c+1] == STATE::S)
        squares << Move(r+2, c+2) << Move(r+1, c+1);
    if (r-2 >= 0 && c+2 < cols)
      if (board[r-2][c+2] == STATE::O && board[r-1][c+1] == STATE::S)
        squares << Move(r-2, c+2) << Move(r-1, c+1);
  }

  if (statusSquare(r, c) == STATE::S) {
    if (r-1 >= 0 && r+1 < rows)
      if (board[r-1][c] == STATE::O && board[r+1][c] == STATE::O)
        squares << Move(r-1, c) << Move(r+1, c);
    if (c-1 >= 0 && c+1 < cols)
      if (board[r][c-1] == STATE::O && board[r][c+1] == STATE::O)
        squares << Move(r, c-1) << Move(r, c+1);
    if (r-1 >= 0 && r+1 < rows && c-1 >= 0 && c+1 <cols) {
      if (board[r-1][c-1] == STATE::O && board[r+1][c+1] == STATE::O)
        squares << Move(r-1, c-1) << Move(r+1, c+1);
      if (board[r+1][c-1] == STATE::O && board[r-1][c+1] == STATE::O)
        squares << Move(r+1, c-1) << Move(r-1, c+1);
    }
  }
  if (squares.isEmpty())
      return {};
  squares << Move(r, c);
  std::vector<Move> vec(squares.begin(), squares.end());
  return vec;
}

