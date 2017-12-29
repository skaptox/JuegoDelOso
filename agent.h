//  Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef AGENT_H_
#define AGENT_H_

#include <QObject>

class Board;
class Game;
class DataGame;

class Agent : public QObject {
Q_OBJECT
 public:
  explicit Agent(QObject *parent = 0);
  void aiVsAi(DataGame *) const;
  void playAgent(DataGame *) const;

 signals:
  void repaint() const;
  void gameOver() const;

 private:
  int recommendedDepth(const DataGame *) const;
  size_t numOfNodes(int, int) const;
};

#endif  // AGENT_H_
