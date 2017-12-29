//  Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef BOARD_H_
#define BOARD_H_

#include <cstring>
#include <tuple>
#include <vector>

extern int counter;


/*!
 * \enum class STATE
 * \brief represent the board's states.
 */
enum class STATE{ S, O, EMPTY , ERROR };

/*!
 * \enum class GAME_STATUS
 * \brief represent the game's state.
 */
enum class GAME_STATUS{ PLAY, END };

/*!
 * \struct Move
 * \brief Convenient struct to simplify the handling of movements in the board.
 */
struct Move {
  int row, col;
  STATE state;
  int dValue, hValue;

  Move(int r, int c, STATE s, int d = 0, int h = 0): row(r) , col(c),
    state(s), dValue(d), hValue(h) {}

  Move(int r, int c): row(r) , col(c) {}
  Move() : row(0), col(0), state(STATE::EMPTY) {}

  bool operator>(const Move &rhs) const {
    return dValue > rhs.dValue;
  }

  bool operator<(const Move &rhs) const {
    return dValue < rhs.dValue;
  }
};

/*!
 * \class Board
 * \brief class to represent the board , this is a multidimetional matrix of STATE
 * type .
 */

class Board {
 public:
  /*!
  * \brief constuct by defect.
  */
  Board();

  /*!
  * \brief construct parameterized.
  * \param row , integer representing the row in the matrix.
  * \param col , integer representing the column in the matrix.
  */
  Board(int row, int col);

  /*!
  * \brief construct by copy
  * \param conste reference to another board.
  */
  Board(const Board &);

  /*!
  * \brief destructor , free the matrix in the memory.
  */
  ~Board();

  /*!
  * \brief return the board's current state CONST function.
  * \return board's state.
  */
  GAME_STATUS statusGame() const;

  /*!
  * \brief consult some square on the board.
  * \param First , size_t representing the row on the board.
  * \param Second , size_t representing the column on the board.
  * \return given a position (row ,col) , return the state.
  */
  STATE statusSquare(int, int) const;

  /*!
  * \brief amount of OSO words around of square given..
  * \param First , size_t representing the row on the board.
  * \param Second , size_t representing the column on the board.
  * \return integer , count of OSO words around if square given.
  */
  int osoWordAround(int, int);
  int osoWordAround(const Move &mv);

  /*!
  * \brief method to assing a state to a position given.
  * \param First , size_t representing the row on the board.
  * \param Second , size_t representing the column on the board.
  * \param Third , const reference to a STATE instance.
  * \return int , 0 success case , 1 otherwise.
  */
  int setState(const int &, const int &, const STATE &);

  /*!
  * \brief method to assign a state from movement given.
  * \param Unique , const reference to a Move object .
  * \return int , 0 success case , 1 otherwise.
  */
  int setState(const Move&);

  /*!
  * \brief method to unassing a square on the board from Move given.
  * \param Unique , const reference to a Move object .
  * \return int , 0 success case , 1 otherwise.
  */
  int unSetState(const Move&);

  /*!
  * \brief method to unassing a square from a position given.
  * \param r , integer representing the row on the board.
  * \param c , integer representing the column on the board.
  * \retuen int , 0 success case , 1 otherwise.
  */
  int unSetState(int r, int c);

  /*!
  * \brief resetBoard , function to reset all board squares.
  */
  int resetBoard();

  /*!
  * \brief squaresOsoAround , function which search and return squares that generates
  * oso word , it squares are around if row and col param.
  * \param r , row index.
  * \param c , column index.
  * \return std::vector<Move> , list of squares found.
  */
  std::vector<Move> squaresOsoAround(int r, int c);

  /*!
  * \brief inline mothod to get the board's tam rows.
  * \return int , rows tam.
  */
  inline  int rowsTam() const {return rows;}

  /*!
  * \brief inline method to get the board's tam columns.
  * \return int , columns tam.
  */
  inline  int colsTam() const {return cols;}

  /*!
  * \brief inline method to get the squares filled on the board.
  * \return int , counts of squares filled.
  */
  inline int squaresFilled() const { return squaresCount;}
  inline int empty_squares() const { return (rows * cols) - squaresCount;}


 private:
  STATE **board;
  int rows , cols;
  int squaresCount;
};

#endif  // BOARD_H_
