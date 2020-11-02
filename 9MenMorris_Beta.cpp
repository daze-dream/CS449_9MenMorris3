// 9MenMorris_Beta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Board.h"
#include "BoardPosition.h"
#include "Player.h"
#include "Enum.h"

using namespace std;

int main()
{
	Board gameboard;
	char sRow, gRow;
	int sColumn, gColumn;
	Player player1(1, false);
	Player player2(2, false);
	gameboard.addPlayers(player1);
	gameboard.addPlayers(player2);
	gameboard.setState(phase1);
	gameboard.setCurrPlayerVar(1);
	gameboard.displayBoard();
	//gameboard.doMovePhase1('a', 2, gameboard.getCurrentPlayer());
	//gameboard.doMovePhase1('a', 4, gameboard.getCurrentPlayer());
	//gameboard.doMovePhase1('a', 7, gameboard.getCurrentPlayer());
	//gameboard.togglePlayer();
	//gameboard.removePiece('a', 4, gameboard.getCurrentPlayer());
	//cout << std::endl;
	//gameboard.displayBoard();

	while (gameboard.getState() != gameOver)
	{
		switch (gameboard.getState())
		{
		case phase1:
			cout << gameboard.getCurrentPlayer().getPlayerID() << "'s Turn|| ";
			cout << "Enter a location to place a piece..." << endl;
			cout << "Column: ";
			cin >> gRow;
			//cout << "    ";
			cout << "Row: ";
			cin >> gColumn;
			cout << endl;
			while (!gameboard.phase1Check(gRow, gColumn))
			{
				cout << "Invalid position. Make sure the position exists and is unoccupied. Try again..." << endl;
				cout << "Column: ";
				cin >> gRow;
				//cout << "    ";
				cout << "Row: ";
				cin >> gColumn;
				cout << endl;
			}
			gameboard.doMovePhase1(gRow, gColumn, gameboard.getCurrentPlayer());
			gameboard.checkAndChangeState(gameboard.getPlayers().at(0), gameboard.getPlayers().at(1));
			gameboard.displayBoard();
			if(gameboard.getState() != mill)
				gameboard.togglePlayer();
			break;
		case phase2:
			gameboard.getPlayers().at(0).timetoFly();
			gameboard.getPlayers().at(1).timetoFly();
			cout << gameboard.getCurrentPlayer().getPlayerID() << "'s Turn|| ";
			cout << "Select a piece to move..." << endl;
			cout << "Column: "; cin >> sRow;
			//cout << "    ";
			cout << "Row: "; cin >> sColumn;
			cout << endl;
			cout << "Select where to move..." << endl;
			cout << "Column: "; cin >> gRow;
			//cout << "    ";
			cout << "Row: "; cin >> gColumn;
			cout << endl;
			while (gameboard.isValidLocation(gRow, gColumn, sRow, sColumn, gameboard.getCurrentPlayer()) == false)
			{
				cout << "Invalid selections. Check that your start location is valid and owned by you, and the goal is valid unoccupied. Try agian... " << endl;
				cout << "Column: "; cin >> sRow;
				//cout << "    ";
				cout << "Row: "; cin >> sColumn;
				cout << endl;
				cout << "Select where to move..." << endl;
				cout << "Column: "; cin >> gRow;
				//cout << "    ";
				cout << "Row: "; cin >> gColumn;
				cout << endl;
			}
			gameboard.doMovePhase2(sRow, sColumn, gRow, gColumn, gameboard.getCurrentPlayer());
			gameboard.checkAndChangeState(gameboard.getPlayers().at(0), gameboard.getPlayers().at(1));
			gameboard.displayBoard();
			if (gameboard.getState() != mill)
				gameboard.togglePlayer();
			break;
		case mill:
			cout << gameboard.getCurrentPlayer().getPlayerID() << " has made a Mill! Select a piece to remove: " << endl;
			cout << "Column: "; cin >> gRow;
			//cout << "    ";
			cout << "Row: "; cin >> gColumn;
			cout << endl;
			while (gameboard.removePiece(gRow, gColumn, gameboard.getCurrentPlayer()) == false)
			{
				cout << "Invalid selection. Make sure that you are removing a piece that exists and is not yours." << endl;
				cout << "Column: "; cin >> gRow;
				//cout << "    ";
				cout << "Row: "; cin >> gColumn;
				cout << endl;
			}
			gameboard.checkAndChangeState(gameboard.getPlayers().at(0), gameboard.getPlayers().at(1));
			gameboard.displayBoard();
			gameboard.togglePlayer();
			break;
		default:
			break;
		}
	}
	cout << gameboard.getCurrentPlayer().getPlayerID() << " is the winner!" << endl;

	system("pause");
	return 0;
}

