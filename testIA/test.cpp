#include "board.h"
#include "ai.h"
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

int main(int , char const **)
{
  Board board1(6,6);

  Move moveAI;
  int turn = 1 , P1G = 0 , P2G=0 ;
  int points;

  moveAI = playAI(board1 , 4 , true, P1G , P2G );
  board1.setState(moveAI);

// TEST for board 1

  clock_t start, end;
  start = clock();

  while(board1.statusGame() != GAME_STATUS::END) {
    // cout << "Player " << (turn ? 1 : 2) << endl;
    moveAI = playAI(board1 , 4 , turn, P1G , P2G );

    board1.setState(moveAI);

    //showBoard(board1);

    points = board1.osoWordAround(moveAI.row,moveAI.col);

    if (points == 0) {
      turn = !turn;
    } else {
      if (turn)
        P1G += points;
      else
        P2G += points;
    }

    /*
    cout << "Player 1: " << P1G << endl;
    cout << "Player 2: " << P2G << endl;
    */
  }

  end = clock();
  cout << "Time: " << (double) (end - start) / CLOCKS_PER_SEC << endl;
  cout << endl;

  showBoard(board1);

  cout << "Player 1: " << P1G << endl;
  cout << "Player 2: " << P2G << endl;
  cout << endl;

  cout << "Counter: " << counter << endl;

  return 0;
}