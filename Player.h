#pragma once
#include <iostream>
#include <vector>
#include "Enum.h"
#include "BoardPosition.h"

using std::vector;
using std::string;
class Player
{
public:
	Player();
	Player(int id, bool isAI);
	//getters

	int getPlayerID() { return playerID; };
	bool getIsAI() { return isAI; };
	int getPhase1Pieces() { return phase1Pieces; };
	int getPhase2Pieces() { return phase2Pieces; };
	bool getFlight() { return isFlight; };
	int getActiveMills() { return this->activeMills; };

	// returns their enemy. Remember it is 1 or 2, be careful for accessing data structures
	int getEnemy() { return 3 - this->playerID; }; 

	//setters

	void setID(int id) { this->playerID = id; };
	void setAI(bool isAI) { this->isAI = isAI; };
	void setPhase1Pieces(int x) { this->phase1Pieces = x; };
	void setPhase2Pieces(int x) { this->phase2Pieces = x; };
	void setFlight(bool flight) { this->isFlight = flight; };
	void setActiveMills(int x) { this->activeMills = x; };
	//functions

	//player check called after mill phase, and toggles if they are ready to leave the nest.
	void timetoFly();
	//subtract a piece. Changes depending on the passed board state.
	void subtractPiece(GameStates state);
	//add a piece. Only really applies to phase 2 so no need to pass state.
	void addPiece();
	//decrement active mills
	void subtractMills() { this->activeMills--; };


private:
	int playerID = 0;
	bool isAI = false;
	int phase1Pieces = 9;
	int phase2Pieces = 0;
	bool isFlight = false;
	int activeMills = 0;




};

