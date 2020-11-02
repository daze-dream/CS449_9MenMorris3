#include "Player.h"

Player::Player()
{
}

Player::Player(int id, bool isAI)
{
	this->playerID = id;
	this->isAI = isAI;
}

void Player::timetoFly()
{
	if (this->getPhase2Pieces() <= 3)
	{
		setFlight(true);
	}
}

void Player::subtractPiece(GameStates state)
{
	switch (state) {
	case phase1:
		this->phase1Pieces--;
		break;
	case phase2:
		this->phase2Pieces--;
		break;
	}
}

void Player::addPiece()
{
	this->phase2Pieces++;
}
