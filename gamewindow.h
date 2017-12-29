// Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include <QMediaPlayer>
#include <QPushButton>
#include <QWidget>

#include <memory>
#include <utility>


class Game;
class QLineEdit;
class QMediaPlaylist;
class QVBoxLayout;

class GameWindow : public QWidget {
  Q_OBJECT

 public:
  explicit GameWindow(QWidget *parent = 0);

 private:
  typedef QPushButton Button;

  Game *_game;
  int _difficulty, _format, _player1State, _player2State, _volume;
  QLineEdit *_display1, *_display2;
  QMediaPlaylist *playlist;
  QVBoxLayout *_centralLayout;
  QWidget *_boardWidget, *_centralWidget;
  std::unique_ptr<QMediaPlayer> _music;

  QString customStyleSheet();
  void GameMatch();

 private slots:  // NOLINT
  void audioButtonClicked();
  void backButtonClicked();
  void playButtonClicked();
  void playerButtonClicked();
  void settingsButtonClicked();
  void sliderChanged();
  void showMenu();
  void showFinalScore(std::pair<int, int>);
};

#endif  // GAMEWINDOW_H_
