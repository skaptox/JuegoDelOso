// Copyright (c) 2017 Richard Games. All rights reserved.

#include <QDesktopWidget>
#include <QDir>
#include <QEvent>
#include <QFontDatabase>
#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QLayout>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QWidget>

#include <iostream>

#include "./game.h"
#include "./gamewindow.h"
#include "./qt_functions.h"

std::pair<int, int> score;

using sk::make;
using std::cout;
using std::endl;

GameWindow::GameWindow(QWidget *parent) : QWidget(parent), _difficulty(4),
_format(7), _volume(50) {
  //! Detect screen resolution

  QDesktopWidget widget;
  QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
  int h = mainScreenSize.height();

  //! Aspect Radio 3:4

  float w = h * 0.75;
  float x = (mainScreenSize.width() / 2) - (w / 2);  //! Center widget in screen

  setWindowFlags(Qt::FramelessWindowHint);  //! Disable Window Buttons.
  setGeometry(x, 0, w, h);
  setFixedSize(size());

  //! Central Widget

  _centralWidget = make<QWidget>("centralWidget");

  //! Main Buttons

  Button *playButton = make<Button>("playButton",
    ":/Icons/play-button-1.png");

  connect(playButton, SIGNAL(clicked()), this, SLOT(playButtonClicked()));

  Button *settingsButton = make<Button>("settingsButton",
    ":/Icons/settings.png");
  connect(settingsButton, SIGNAL(clicked()), this,
    SLOT(settingsButtonClicked()));

  Button *audioButton = make<Button>("audioButton", ":/Icons/mute.png");
  connect(audioButton, SIGNAL(clicked()), this, SLOT(audioButtonClicked()));

  //! Player Buttons

  Button *player1Button = make<Button>("player1Button", ":/Icons/user-2.png");
  _player1State = 2;

  Button *player2Button = make<Button>("player2Button", ":/Icons/user-1.png");
  _player2State = 1;

  for (const auto &item : {player1Button, player2Button}) {
    connect(item, SIGNAL(clicked()), this, SLOT(playerButtonClicked()));
    item->hide();
  }

  //! Layouts

  QHBoxLayout *buttonsLayout = make<QHBoxLayout>("buttonsLayout",
    Qt::AlignHCenter);

  //! Adding buttons to its layout

  for (auto &item : {player1Button, settingsButton, playButton, audioButton,
  player2Button}) {
    buttonsLayout->addWidget(item);
  }

  //! Central Layout

  _centralLayout = make<QVBoxLayout>("centralLayout", Qt::AlignBottom);
  _centralLayout->addLayout(buttonsLayout);

  _centralWidget->setLayout(_centralLayout);

  //! Window Buttons

  Button *exitButton = make<Button>("exitButton", ":/Icons/add-1.png");
  connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

  Button *minimizeButton = make<Button>("minimizeButton",
    ":/Icons/minus-1.png");
  connect(minimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));

  QHBoxLayout *windowButtonsLayout = make<QHBoxLayout>("windowButtonsLayout",
    Qt::AlignRight);

  for (const auto &item : {minimizeButton, exitButton})
    windowButtonsLayout->addWidget(item);

  //! Layout of 'This'

  QVBoxLayout *mainLayout = make<QVBoxLayout>("mainLayout");

  mainLayout->addLayout(windowButtonsLayout);
  mainLayout->addWidget(_centralWidget);
  mainLayout->setContentsMargins(10, 0, 10, 0);

  setLayout(mainLayout);

  //! StyleSheet

  setObjectName("this");
  setStyleSheet(customStyleSheet());

  //! Soundtracks


  QString dir = QDir::currentPath() + "/Audios/[NCM] Island - MBB.aac";
  playlist = new QMediaPlaylist(this);
  playlist->addMedia(QUrl::fromLocalFile(dir));
  playlist->setPlaybackMode(QMediaPlaylist::Loop);

  _music.reset(new QMediaPlayer(this));
  _music->setPlaylist(playlist);
  _music->setVolume(_volume);
  _music->play();

}

void GameWindow::GameMatch() {
  QWidget *gameWidget = make<QWidget>("gameWidget");

  QVBoxLayout *gameLayout = new QVBoxLayout();
  QSize availableSize = _centralWidget->size();

  _game = new Game(availableSize, _difficulty, _format, _player1State,
  _player2State, this);

  connect(_game, &Game::closedGameMatch, this, &GameWindow::showMenu);
  connect(_game, &Game::gameOver, this, &GameWindow::showFinalScore);

  gameLayout->addWidget(_game);
  gameLayout->setMargin(0);
  gameWidget->setLayout(gameLayout);

  _centralLayout->addWidget(gameWidget);
}

void GameWindow::audioButtonClicked() {
  Button *button = qobject_cast<Button*> (sender());
  button->clearFocus();

  if (_music->state() == QMediaPlayer::PausedState) {
    _music->play();
    button->setIcon(QIcon(":/Icons/mute.png"));
  } else {
    _music->pause();
    button->setIcon(QIcon(":/Icons/music-player-1.png"));
  }
}

void GameWindow::backButtonClicked() {
    _centralWidget->setGraphicsEffect(Q_NULLPTR);  //! Disable blur effect
    Button *button = qobject_cast<Button*>(sender());

    QWidget *widget;
    QLabel *label = findChild<QLabel*>("settingLabel");

    if (button->objectName() == "backButton2") {
        widget = findChild<QWidget*>("scoreWidget");
        label = findChild<QLabel*>("scoreLabel");
        _game->resetButtonClicked();
    } else {
        widget = findChild<QWidget*>("settingWidget");
        label = findChild<QLabel*>("settingLabel");
        Button *settingsButton = findChild<Button*>("settingsButton");
        settingsButton->setEnabled(true);
    }

    if (widget != Q_NULLPTR) {
      delete widget;
      delete label;
    }
}

void GameWindow::playButtonClicked() {
  static bool playersChoosed;

  Button *button = qobject_cast<Button*>(sender());
  if (button != Q_NULLPTR) button->clearFocus();

  backButtonClicked();  //! If that were pressed being on setting panel

  /* layout = [player1Button, settingsButton, playButton, audioButton,
    player2Button]  */

  QHBoxLayout *layout = _centralLayout->findChild<QHBoxLayout*>
    ("buttonsLayout");
  sk::setVisible(layout, {1, 3}, false);

  if (!playersChoosed) {
    sk::setVisible(layout, {0, 4}, true);  // To choose players
  } else {
    sk::setVisible(layout, {0, 2, 4}, false);
    GameMatch();
  }
  playersChoosed = !playersChoosed;
}

void GameWindow::playerButtonClicked() {
  Button *button = qobject_cast<Button*>(sender());
  if (button != Q_NULLPTR) button->clearFocus();

  int &player = (button->objectName() == "player1Button") ?
    _player1State : _player2State;

  if (player < 3)
      player += 1;
  else
      player = 1;

  button->setIcon(QIcon(":/Icons/user-"+ QString::number(player)+".png"));
}

void GameWindow::settingsButtonClicked() {
    Button *button = qobject_cast<Button*>(sender());
    if (button != Q_NULLPTR) {
      button->clearFocus();
      button->setEnabled(false);
    }

    //! Panel Widget

    QWidget *settingWidget = new QWidget(this);
    settingWidget->setObjectName("settingWidget");

    //! To center the panel on the screen

    int x = width()/20;
    int h = height()/2.5;
    int y = (height() - h) / 2;
    int w = width() - 2*x;

    settingWidget->setGeometry(x, y, w, h);

    //! SETTINGS

    //! Header

    QLabel *settingLabel = new QLabel("Configuración", this);
    settingLabel->setObjectName("settingLabel");

    settingLabel->setAlignment(Qt::AlignCenter);
    settingLabel->setGeometry(x, y-100, w, 100);
    settingLabel->show();

    QString f = QString::number(_format);
    QString d = QString::number(_difficulty);

    QLabel *customsLabel = new QLabel("Tam: " + f + "x" + f + " Dif: " + d);
    customsLabel->setObjectName("customsLabel");

    //! Board Size

    //!  Board Size Label

    QLabel *boardSizeLabel = new QLabel("Tamaño");

    //! _difficulty Slider

    QSlider *boardSizeSlider = new QSlider(Qt::Horizontal);
    boardSizeSlider->setObjectName("boardSizeSlider");
    boardSizeSlider->setRange(300, 1200);
    boardSizeSlider->setValue(_format*100);
    connect(boardSizeSlider, SIGNAL(valueChanged(int)), this,
      SLOT(sliderChanged()));

    //!  _difficulty Label

    QLabel *_difficultyLabel = new QLabel("Dificultad");

    //! _difficulty Slider

    QSlider *_difficultySlider = new QSlider(Qt::Horizontal);
    _difficultySlider->setObjectName("_difficultySlider");

    _difficultySlider->setRange(100, 400);
    _difficultySlider->setValue(_difficulty*100);
    connect(_difficultySlider, SIGNAL(valueChanged(int)), this,
      SLOT(sliderChanged()));

    //! Music

    //! Music Label

    QLabel *musicLabel = new QLabel("Música");

    //! Music Slider

    QSlider *musicSlider = new QSlider(Qt::Horizontal);
    musicSlider->setObjectName("musicSlider");
    musicSlider->setValue(_volume);
    connect(musicSlider, SIGNAL(valueChanged(int)), this,
      SLOT(sliderChanged()));

    //! Back Button

    Button *backButton = new Button("ATRAS");
    backButton->setObjectName("backButton");
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));

    //! Layout to add all items

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(customsLabel, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(boardSizeLabel, 1, 0);
    layout->addWidget(boardSizeSlider, 1, 1);

    layout->addWidget(_difficultyLabel, 2, 0);
    layout->addWidget(_difficultySlider, 2, 1);

    layout->addWidget(musicLabel, 3, 0);
    layout->addWidget(musicSlider, 3, 1);

    layout->addWidget(backButton, 4, 1, Qt::AlignBottom | Qt::AlignRight);

    settingWidget->setLayout(layout);
    settingWidget->show();

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(10);

    _centralWidget->setGraphicsEffect(effect);
}

void GameWindow::sliderChanged() {
    QSlider *slider = qobject_cast<QSlider*>(sender());

    if (slider->objectName() == "musicSlider") {
        _music->setVolume(slider->value());
    } else if (slider->objectName() == "boardSizeSlider") {
        _format = slider->value() / 100;
    } else if (slider->objectName() == "_difficultySlider") {
        _difficulty =  slider->value() / 100;
    } else {}

    QLabel* customsLabel = findChild<QLabel*>("customsLabel");
    QString f = QString::number(_format);
    QString d = QString::number(_difficulty);

    customsLabel->setText("Tam: " + f + "x" + f + " Dif: " + d);
}

void GameWindow::showMenu() {
    QWidget *gameWidget = _centralWidget->findChild<QWidget*>("gameWidget");
    if (gameWidget != Q_NULLPTR)
        delete gameWidget;

    QHBoxLayout *layout =
      _centralLayout->findChild<QHBoxLayout*>("buttonsLayout");

  /* layout = [player1Button, settingsButton, playButton, audioButton,
   player2Button] */

    sk::setVisible(layout, {0, 4}, false);
    sk::setVisible(layout, {1, 2, 3}, true);
}

void GameWindow::showFinalScore(std::pair<int, int> score) {
    //! Panel Widget

    QWidget *finalScoreWidget = new QWidget(this);
    finalScoreWidget->setObjectName("scoreWidget");

    //! To center the panel on the screen

    int x = width() / 20;
    int h = height() / 2.5;
    int y = (height() - h) / 2;
    int w = width() - 2 * x;

    finalScoreWidget->setGeometry(x, y, w, h);

    //! SETTINGS

    //! Header

    QLabel *scoreLabel = new QLabel("Juego Terminado", this);
    scoreLabel->setObjectName("scoreLabel");

    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setGeometry(x, y-100, w, 100);
    scoreLabel->show();

    QLabel *player1Image = new QLabel();
    player1Image->setPixmap(QPixmap(":/Icons/user-" +
      QString::number(_player1State)+".png"));


    QLabel * player2Image = new QLabel();
    player2Image->setPixmap(QPixmap(":/Icons/user-" +
      QString::number(_player2State)+".png"));

    QLabel *score1 = new QLabel(QString::number(score.first));
    QLabel *score2 = new QLabel(QString::number(score.second));

    //! Back Button

    Button *backButton = new Button("ATRAS");
    backButton->setObjectName("backButton2");
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonClicked()));

    //! Layout to add all items

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(player1Image, 0, 0, Qt::AlignCenter);
    layout->addWidget(player2Image, 0, 1, Qt::AlignCenter);
    layout->addWidget(score1, 1, 0, Qt::AlignCenter);
    layout->addWidget(score2, 1, 1, Qt::AlignCenter);
    layout->addWidget(backButton, 2, 1, Qt::AlignBottom | Qt::AlignRight);

    finalScoreWidget->setLayout(layout);
    finalScoreWidget->show();

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(10);

    _centralWidget->setGraphicsEffect(effect);
}

QString GameWindow::customStyleSheet() {
    QFile styleFile(":/Css/Style.css");
    if (styleFile.open(QIODevice::ReadOnly)) {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        return styleSheet;
    }
    return "";
}

