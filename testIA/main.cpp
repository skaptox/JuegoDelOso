#include "Board.h"
#include "ai.h"
#include <string>
#include <iostream>

using namespace std;
 
int main(int , char const **)
{
	Board board1(8,8);
	Board board2(12,12);
	Board board3(4,4);	
	Move moveAI(1,1,STATE::S);
	int value =0 , nextTurn =-1 , P1G=0 , P2G=0 ; 
	int points;



/************** TEST for board 1 ****************/

	board1.setState(0,0,STATE::O);
	board1.setState(0,2,STATE::O);
	board1.setState(2,0,STATE::O);
	board1.setState(2,2,STATE::O);
	board1.setState(4,4,STATE::O);
	board1.setState(4,0,STATE::O);
	board1.setState(4,2,STATE::O);
	board1.setState(4,6,STATE::O);
	board1.setState(2,6,STATE::O);
	board1.setState(6,2,STATE::O);
	board1.setState(6,4,STATE::O);

	cout << "Print Board 1" << endl;
	cout << "Resolve AI board1 " <<endl;

	cout << " value for board deph 4 " << alphaBeta(-500,500,3,board1,-1,0,0)<<endl;
	showBoard(board1);

	while(board1.statusGame() != GAME_STATUS::END)
	{
		moveAI = playAI(board1 , 4 , nextTurn , P1G , P2G );

		board1.setState(moveAI);
	
		cout<<"\n\n";

		showBoard(board1);

    points = board1.osoWordAround(moveAI.row,moveAI.col);

		else
			if(nextTurn == 1)
				P1G += points;
			else
				P2G += points;
	}
	cout<<"\n\n";

	
/************ END TEST on board 1 *********************/


/*********** TEST on board 2 **************************/


	board2.setState(0,0,STATE::O);
	board2.setState(1,1,STATE::S);
	board2.setState(2,2,STATE::O);
	board2.setState(4,0,STATE::S);
	board2.setState(4,3,STATE::O);
	board2.setState(5,2,STATE::O);
	board2.setState(7,0,STATE::S);

	cout << "test on board2 "<< endl;

	showBoard(board2);

	nextTurn = -1; 
	P1G = 1;
	P2G = 0;
/*
	value = alphaBeta(-500 , 500 , 4 ,board2, -1 , 2, 0);
	cout << "value obtained = " << value << endl;
	cout<<"\n\n";
*/
/*
	while(board2.statusGame() != GAME_STATUS::END)
	{
		moveAI = playAITime(board2);

		board2.setState(moveAI);

		cout<<"\n\n";

		showBoard(board2);
		cout << " turn : "<<nextTurn << endl;

		points = board2.osoWordAround(moveAI.row,moveAI.col);
		if(points < 1)
		{
			nextTurn = playerToggle(nextTurn);
			continue;
		}

		else
			if(nextTurn == 1)
				P1G += points;
			else
				P2G += points;
	}
	cout<<"\n\n";

	cout <<" PLayer 1 points : "<<P1G << endl;
	cout <<" PLayer 2 points : "<<P2G << endl;

 

/***************** Test on board 3 *************/
/*

	board3.setState(0,0,STATE::O);
	board3.setState(1,1,STATE::S);
	board3.setState(2,2,STATE::O);
	board3.setState(0,2,STATE::S);
	board3.setState(0,3,STATE::S);
	board3.setState(2,3,STATE::O);
	board3.setState(3,1,STATE::S);
	board3.setState(3,3,STATE::S);
	board3.setState(3,0,STATE::O);
	showBoard(board3);

/************* test osoWordAround *****************/
/*
	board3.setState(3,2,STATE::O);
	if(board3.osoWordAround(3,2) > 0)
		cout<<"passed"<<endl;
	else
		cout<<"failed"<<endl;
	board3.unSetState(3,2);
/************** end test osoWordAround *************/

/************** test sortMoves  ********************/

	QVector<Move> mvs = generateMoves(board3);

	sortMoves(mvs,	board3);

	for(auto it : mvs )
	{
		cout<<printMove(it);
	}
	cout << printMove(mvs.first())<<endl;
	if(mvs.first().row == 3 and mvs.first().col == 2 and mvs.first().state == STATE::O)
		cout<<"passed sortMoves"<<endl;
	else
		cout<<"failed sortMoves"<<endl;
/*************** end test sortMoves ******************/


/************** test alphaBeta *****************/
/*
	board3.setState(3,2,STATE::O);

	value = alphaBeta(-500 , 500 , 4 ,board3, 1 , 3, 0);
	cout << "value obtained board 3 = " << value << endl;

	board3.unSetState(3,2);
	if(value < 2)
		cout << "falied test alphaBeta"<< endl;
/************** end test alphaBeta ***************/


/************** test playAITime ***********************/
/*
	moveAI = playAITime(board3,3,1,0,1);
	board3.setState(moveAI);
	showBoard(board3);
	cout<<"\n\n";
/************** end test playAITime ********************

	Board board4(2,2);
	evalBoardCount = 0;
	alphaBetaCount = 0;
	value = alphaBeta(-500,500,20,board4,1,0,0);
	cout << "value obtained = " << value << endl;
	cout << "counts of nodes explored " << evalBoardCount << endl;	
	cout << "couts of alphaBeta function calls "<< alphaBetaCount << endl;

	cout <<"\n\n";

	Board board6(2,2);

	evalBoardCount = 0;
	value = alphaBetaIterative(-500,500,20,board6,1,0,0,10);
	cout << "value obtained = " << value << endl;
	cout << "counts of nodes explored " << evalBoardCount << endl;	
	cout << "couts of alphaBetaIterative function calls "<< itertiveDeph << endl;

	*/
	cout <<"\n\n";

	Board board5(2,2);

	evalBoardCount = 0;
	//value = minimaxExaustiveSearch(board5,1,0,0);
	cout << "value obtained = " << value << endl;
	cout << "counts of nodes explored " << evalBoardCount << endl;	
	cout << "couts of minimaxExaustiveSearch function calls "<< minMaxCount << endl;




	return 0;
}