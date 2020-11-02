#pragma once
#include <array>
#include <vector>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include "Enum.h"
#include "BoardPosition.h"
#include "Player.h"

using std::array;
using std::vector;
using std::pair;
using std::make_pair;

class Board
{
public:
	//TODO: add conditions to set up board for other morris game modes
	//default constructor sets up all the valid locations for 9 men morris. 
	Board(); 
	void setState(GameStates inc) { this->state = inc; };
	void setCurrPlayerVar(int x) { this->currentPlayer = x; };
	auto getPositions() { return this->positions; };
	/*checks the remaining pieces of each player and determiens the phase
		of the game from that. If either player has a mill, changes to that state.*/
	void checkAndChangeState(Player p1, Player p2);
	//TODO: recheck this one a lot.
	/*checks if the location is valid. changes depending on 
		the current game phase and if the player is in flight mode.
		The Source parameters are only for phase 2, and by default are 0 in the main function for phase 1.
		This is what is called to determine if a move should be done in the 'cin' loop in main.*/
	bool isValidLocation(char row, int column, char rowSource, int columnSource, Player player);
	//TODONE: finish doMove function for phase 1
	/*Does the move for phase one, aka PlacePiece. Calls the isValidLocation() to make sure the space isn't occupied,
		calls isMill() to check for mills, and then sets the board state and player's active mills if applicable.
		Might want to change to bool type */
	void doMovePhase1(char row, int column, Player &player);
	//TODONE: finish movePiece function for moving pieces in phase 2 with checkIsMillAfterAMove() call
	/*Does the move for phase two. Requires the source and goal coordinates to do right. Calls the isValid() function
		to make sure the move is adjacent if not in flight mode, then calls isMill() to check for mills, and then sets 
		the board state and player's active mills if applicable.
		Might want to change to bool type*/
	void doMovePhase2(char rowStart, int columnStart, char rowGoal, int columnGoal, Player player);
	//TODONE: remove piece function
	bool removePiece(char row, int column, Player player);
	/*TODO : isMill mostly finished, keep checking for errors later */
	/*check if the current position is in a mill. Remember that if it has been
		in a mill then ignore it (if it even comes up).
		Idea is to iterate with + and - up and down through
		the boardpositions array and check if the position matches the current player state
		and if the total in either is 3, then it is a mill, and is added to total mills
		because you can make multiple mills. Returns if there are mills and how many mills 
		were made. 
		Distinction: The doMove function is responsible for adding the mills to the player
			class and setting the board state.*/
	pair<bool, int> isMill(char row, int column, Player player);

	//OBSOLETE: Each location is responsible for checking if it is part of a mill.
	void checkIsMillAfterAMove(Player player);

	/*Toggles the currentPlayer variable for quick access to the BoardPlayers vector*/
	void togglePlayer() { this->currentPlayer = 3 - currentPlayer; };
	/*returns the current player from the vector BoardPlayers. Takes care of overflow with data structures*/
	Player& getCurrentPlayer() { return this->BoardPlayers.at(currentPlayer - 1); };
	/*Prints board to console*/
	void displayBoard();

	//function to allow adding to the BoardPlayers vector
	void addPlayers(Player player) { this->BoardPlayers.push_back(player); }
	
	//get the asciiOffset Value
	int getOffset() { return this->asciiOffset; };

	GameStates getState() { return this->state; };

	vector<Player> getPlayers() { return this->BoardPlayers; }

	//quick check for inputs in phase 1 without going through the whole isValidMove() call;
	bool phase1Check(char row, int column);

	//qucik check for phase2 source move
	bool phase2_1Check(char sRow, int sColumn);

	//quick check for phase2 goal move
	bool phase2_2Check(char row, int column);
	
private:
	array<array<char, 7>, 7> board = { {} };
	BoardPosition positions[7][7];
	vector<Player> BoardPlayers; //stores the current players
	GameStates state = empty;
	GameStates tempState = empty; //stores a temporary state because restoring state after mill is done.
	bool millFound = false; //might be obsolete
	int asciiOffset = 97; //for subtracting from characters to get an int value for data structures
	int currentPlayer = 0; // used to identify the current player. Remember it is either 2 or 1 so subtract accordinly for data structures.
};

