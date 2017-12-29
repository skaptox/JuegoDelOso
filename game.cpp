//  Copyright (c) 2017 Richard Games. All rights reserved.

#include <QDebug>
#include <QFuture>
#include <QGraphicsEffect>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QProgressBar>
#include <QPushButton>
#include <QRect>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QWidget>

#include <math.h>
#include <functional>
#include <iostream>

#include "./ai.h"
#include "./game.h"
#include "./gamewindow.h"
#include "./qt_functions.h"

using sk::make;

using std::cout;
using std::endl;
using std::vector;

Game::Game(const QSize &size, int level, int format, int p1, int p2,
QWidget *parent)  : QWidget(parent), _format(format) {
  _playAI = (p1 == 1 || p2 == 1) ? true : false;  //!  Does AI play?
  _playAIvsAI = (p1 == 1 && p2 == 1) ? true : false;

  _data = {new Board(format, format), true, 0, 0, level, Status::Start};

  //! HEADER PANEL

  //! Players Images

  _player1Image = new QLabel();

  _player1Image->setPixmap(QPixmap(":/Icons/user-" +
    QString::number(p1) + ".svg"));

  _player2Image = new QLabel();

  _player2Image->setPixmap(QPixmap(":/Icons/user-" +
    QString::number(p2) + ".svg"));

  _player2Image->setEnabled(false);

  Button *selectLetterButton = make<Button>("selectLetterButton",
    ":/Icons/compose.svg");
  connect(selectLetterButton, SIGNAL(clicked()), this, SLOT(changeLetter()));

  //! Score boards

  _display1 = new QLabel("0");
  _display2 = new QLabel("0");
  _display2->setDisabled(true);

  for (const auto & item : {_display1, _display2}) {
    item->setAlignment(Qt::AlignCenter);
    item->setFixedWidth(65);
  }

  //! Adding header widgets to a layout

  vector<QWidget*> headWidgets {_player1Image, _display1, selectLetterButton,
    _display2, _player2Image};

  QHBoxLayout *topLayout = make<QHBoxLayout>("topLayout", Qt::AlignHCenter);

  for (const auto & item : headWidgets) {
    topLayout->addWidget(item);
  }

  //! SUB HEADER PANEL

  //! Utility Buttons

  Button *findMoveButton = make<Button>("findMoveButton", ":/Icons/search.svg");
  connect(findMoveButton, SIGNAL(clicked()), this,
    SLOT(findMoveButtonClicked()));

  Button *pauseButton = make<Button>("pauseButton", ":/Icons/pause-1.svg");
  connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));

  Button *resetButton = make<Button>("resetButton", ":/Icons/repeat-1.svg");
  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
  resetButton->hide();

  Button *closeButton = make<Button>("closeButton", ":/Icons/error.svg");
  connect(closeButton, SIGNAL(clicked()), this, SLOT(closeGame()));
  closeButton->hide();

  //! Adding previous buttons to a layout

  _letter = new QLabel("S");
  _letter->setAlignment(Qt::AlignCenter);
  _letter->setStyleSheet("QLabel{color:rgb(76, 76, 76);font-size:48px;}");

  QHBoxLayout *buttonsLayout = make<QHBoxLayout> ("buttonsLayout");

  vector<QWidget*> buttons {pauseButton, _letter, findMoveButton, resetButton,
    closeButton};

  for (const auto &item : buttons) {
    buttonsLayout->addWidget(item);
  }

  //! Adding previous layouts to a widget's layout

  QVBoxLayout *headerLayout = make<QVBoxLayout>("headerLayout",
    Qt::AlignHCenter);

  headerLayout->addLayout(topLayout);
  headerLayout->addLayout(buttonsLayout);

  QWidget *headerWidget = make<QWidget>("headerWidget", this);

  headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  headerWidget->setLayout(headerLayout);
  headerWidget->show();  //! To know really size

  //! Main Layout

  QVBoxLayout *mainLayout = make<QVBoxLayout>("mainLayout",
    Qt::AlignTop | Qt::AlignCenter);

  mainLayout->addWidget(headerWidget);

  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->setLayout(mainLayout);

  //! GAME BOARD

  //! To know size of each box
  int availableSize = std::min(size.width(), (size.height() -
    headerWidget->height()));

  int squareSide = availableSize / _format;
  _squareSize = new QSize(squareSide, squareSide);

  int initialX = (size.width() - (squareSide * _format)) / 2;
  int initialY = headerWidget->height();

  //! Building the matrix board
  _data.gBoard.resize(_format);

  for (int i = 0; i < _format; ++i) {
    _data.gBoard[i].resize(_format);
    for (int j = 0; j < _format; ++j) {
      _data.gBoard[i][j].rect = QRect(QPoint(squareSide * i + initialX,
        squareSide * j + initialY), *_squareSize);
      _data.gBoard[i][j].status = 0;
    }
  }

  _data.board = new Board(_format, _format);
  _agent = new Agent(this);

  connect(_agent, &Agent::repaint, this, &Game::update);
  connect(_agent, &Agent::gameOver, this, &Game::gameIsOver);
}

void Game::paintEvent(QPaintEvent *e) {
  // Enable active player
  _player1Image->setEnabled(_data.turn);
  _player2Image->setEnabled(!_data.turn);

  // Scores
  _display1->setText(QString::number(_data.score1));
  _display2->setText(QString::number(_data.score2));


  _painter = new QPainter(this);

  QPen borderPen(QColor(255, 255, 255));
  borderPen.setWidth(2);
  _painter->setPen(borderPen);

  QFont font(this->font());
  font.setPointSizeF(_squareSize->width() / 1.5);
  _painter->setFont(font);

  int row = 0, col = 0;
  QPixmap letter;

  for (const auto & it : _data.gBoard) {
    col = 0;
    for (const auto & item : it) {
      STATE state(_data.board->statusSquare(row, col));
      letter.load(state == STATE::S ? ":/Images/s.svg" : ":/Images/o.svg");

      if (state == STATE::EMPTY) {
        _painter->fillRect(item.rect, QColor(33, 33, 33, 230));  // Gray
      } else {
        if (item.status > 0) {  // If _letter complete a word
          if (item.turn) {  // Player one
            if (item.status == 1)
              _painter->fillRect(item.rect, QColor(252, 57, 82, 230));
            else if (item.status == 2)
              _painter->fillRect(item.rect, QColor(222, 50, 72, 230));
            else if (item.status == 3)
              _painter->fillRect(item.rect, QColor(191, 43, 62, 230));
            else
              _painter->fillRect(item.rect, QColor(161, 36, 52, 230));

          } else {  // Player two
              if (item.status == 1)
                _painter->fillRect(item.rect, QColor(87, 99, 133, 230));
              else if (item.status == 2)
                _painter->fillRect(item.rect, QColor(67, 76, 102, 230));
              else if (item.status == 3)
                _painter->fillRect(item.rect, QColor(47, 53, 71, 230));
              else
                _painter->fillRect(item.rect, QColor(30, 34, 46, 230));
          }
        } else {
          if (row == _data.lastMove.row && col == _data.lastMove.col) {
            _painter->fillRect(item.rect, QColor(35, 162, 77, 230));
          } else {
            _painter->fillRect(item.rect, QColor(33, 33, 33, 230));
          }
        }
        _painter->drawPixmap(item.rect, letter.scaled(item.rect.size(),
          Qt::KeepAspectRatio, Qt::SmoothTransformation));
      }
      _painter->drawRoundRect(item.rect, 2, 2);
      col++;
    }
    row++;
  }

  if (_wildCard.state != STATE::EMPTY) {
    letter.load(_wildCard.state == STATE::S ?
      ":/Images/s.svg" : ":/Images/o.svg");

    int r = _wildCard.row;
    int c = _wildCard.col;

    _painter->fillRect(_data.gBoard[r][c].rect, QColor(235, 186, 22));

    _painter->drawPixmap(_data.gBoard[r][c].rect, letter.scaled(
      _data.gBoard[r][c].rect.size(), Qt::KeepAspectRatio,
      Qt::SmoothTransformation));
    _wildCard.state = STATE::EMPTY;
  }

  QWidget::paintEvent(e);
  delete _painter;
}

void Game::mousePressEvent(QMouseEvent *e) {
  if (_data.status == Status::Over) {
    return;
  }

  if (_data.status == Status::Paused) {
    pauseButtonClicked();
  }

  if (_playAIvsAI) {
    QFuture<void> future = QtConcurrent::run(_agent, &Agent::aiVsAi, &_data);
    return;
  }

  for (int i = 0; i < _format; ++i)
    for (int j = 0; j < _format; ++j) {
      bool positionIsValid = _data.gBoard[i][j].rect.contains(e->pos());
      bool squareIsEmpty = _data.board->statusSquare(i, j) == STATE::EMPTY;

      if (positionIsValid && squareIsEmpty && _data.status == Status::Start) {
        int points = 0;

        if (_data.turn) {  // If player one
          STATE state(_letter->text() == "S" ? STATE::S : STATE::O);
          _data.lastMove = Move(i, j, state);
          _data.board->setState(_data.lastMove);

          if (_data.board->statusGame() == GAME_STATUS::END)
            _data.status = Status::Over;

          points = _data.board->osoWordAround(i, j);
          _data.score1 += points;

          QWidget::repaint();

          //! If get points with that last movement
          if (points) {
            vector<Move> squares = _data.board->squaresOsoAround(i, j);
            markBoardSquares(squares, _data);
          } else {
            _data.turn = !_data.turn;
            if (_playAI) {
              QFuture<void> future = QtConcurrent::run(_agent,
                &Agent::playAgent, &_data);
            }
          }
          QWidget::repaint();
        } else {  // If player two
          STATE state = _letter->text() == "S"? STATE::S : STATE::O;
          _data.lastMove = Move(i, j, state);
          _data.board->setState(_data.lastMove);

          if (_data.board->statusGame() == GAME_STATUS::END)
            _data.status = Status::Over;

          if ((points = _data.board->osoWordAround(i, j))) {
            _data.score2 += points;
            vector<Move> squares = _data.board->squaresOsoAround(i, j);
            markBoardSquares(squares, _data);
          } else {
            _data.turn = !_data.turn;
          }
        }
        QWidget::repaint();
        break;
      }
    }

  if (_data.status == Status::Over) {
    gameIsOver();
    return;
  }

  QWidget::mousePressEvent(e);
}

void Game::findMoveButtonClicked() {
  Button *button(qobject_cast<Button*>(sender()));
  if (button) button->clearFocus();

  if (_data.board->statusGame() != GAME_STATUS::END) {
    _wildCard = playAI(*_data.board, 2, _data.turn, _display1->text().toInt(),
      _display2->text().toInt());
  }
  QWidget::repaint();
}

void Game::pauseButtonClicked() {
  Button *pauseButton(qobject_cast<Button*>(sender()));
  if (pauseButton) pauseButton->clearFocus();

  if (_data.status == Status::Paused)
    _data.status = Status::Start;
  else
    _data.status = Status::Paused;

  QHBoxLayout *layout = findChild<QHBoxLayout*>("buttonsLayout");
  if (layout) {
    pauseButton = qobject_cast<Button*>(layout->itemAt(0)->widget());

    //! layout = [pauseButton,letterLabel,findButton,resetButton,closedButton]
    if (_data.status == Status::Paused) {
       sk::setVisible(layout, {1, 2}, false);
       sk::setVisible(layout, {3, 4}, true);
      pauseButton->setIcon(QIcon(":/Icons/play-button-1.svg"));
    } else {
       sk::setVisible(layout, {1, 2}, true);
       sk::setVisible(layout, {3, 4}, false);
      pauseButton->setIcon(QIcon(":/Icons/pause-1.svg"));
    }
  }
}

void Game::update() {
    QWidget::update();
}

void Game::gameIsOver() {
  emit(gameOver(std::make_pair(_data.score1, _data.score2)));
}

void Game::resetButtonClicked() {
  Button *button(qobject_cast<Button*>(sender()));
  if (button) button->clearFocus();
  _data.board->resetBoard();
  _data.reset();

  for (int i = 0; i < _format; ++i)
    for (int j = 0; j < _format; ++j) {
      _data.gBoard[i][j].status = 0;
    }
  QWidget::repaint();
}

void Game::changeLetter() {
  Button *button(qobject_cast<Button*>(sender()));
  if (button) button->clearFocus();

  if (_data.status == Status::Paused) {
     pauseButtonClicked();
  }

  _letter->setText(_letter->text() == "S" ? "O":"S");
}

void Game::closeGame() {
  emit(closedGameMatch());
}

void Game::markBoardSquares(const vector<Move> &squaresList, DataGame &data) {
  for (const auto sq : squaresList) {
    int v = data.board->osoWordAround(sq.row, sq.col);
    data.gBoard[sq.row][sq.col].status = v;
    data.gBoard[sq.row][sq.col].turn = data.turn;
  }
}

Game::~Game() {
  delete _data.board;
  delete _squareSize;
  qDebug() << "Destructor de game";
}

