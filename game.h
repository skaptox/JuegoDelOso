// Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef GAME_H_
#define GAME_H_

#include <QFuture>
#include <QWidget>


#include <utility>
#include <vector>

#include "./agent.h"
#include "./board.h"

class QLabel;
class QMouseEvent;
class QPainter;
class QPaintEvent;
class QProgressBar;
class QPushButton;
class QRect;
class QSize;
class QTimer;
class QElapsedTimer;

struct Box {
  QRect rect;
  bool turn;
  int status;
};

enum Status { Over, Paused, Start, PC};

struct DataGame {
  Board *board;
  bool turn;
  int score1, score2;
  int level;
  Status status;
  Move lastMove;
  std::vector<std::vector<Box>> gBoard;  // Graphic board

  void reset() {
     status = Status::Start;
     score1 = score2 = 0;
     turn = true;
  }
};

//!  Game class.

class Game : public QWidget {
  Q_OBJECT

 public:
  Game(const QSize &size, int level, int format, int playerl, int player2,
    QWidget *parent = nullptr);
  ~Game();
  static void markBoardSquares(const std::vector<Move> &, DataGame &);

 signals:
  void closedGameMatch();
  void gameOver(std::pair<int, int> score);

 private:
  typedef QPushButton Button;
  Agent * _agent;
  DataGame _data;

  void mousePressEvent(QMouseEvent *e) override;
  void paintEvent(QPaintEvent *e) override;

  bool _playAI, _playAIvsAI;
  int _format;

  Move _wildCard;
  QFuture<void> _future;
  QLabel *_display1, *_display2, *_letter, *_player1Image, *_player2Image;
  QPainter *_painter;
  QProgressBar *_timeProgressBar;
  QSize *_squareSize;
  QTimer *_timer;
  QElapsedTimer *_elapsedTimer;

 private slots:
  void changeLetter();
  void closeGame();
  void findMoveButtonClicked();
  void pauseButtonClicked();
  void update();
  void gameIsOver();
  void updateProgressBar();
 public slots:
  void resetButtonClicked();
};

#endif  //  GAME_H_
