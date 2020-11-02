#include "Board.h"

Board::Board()
{
	//start setting up the locations
	// ring 3
	for (char i = 'a'; i < 'h'; i+=3)
	{
		for (int j = 0; j < 7; j += 3)
		{
			positions[i - asciiOffset][j].setup(i, j + 1, 0, '0', true);

		}
	}

	// ring 2
	for (char i = 'b'; i < 'g'; i += 2)
	{
		for (int j = 1; j < 6; j += 2)
		{
			positions[i - asciiOffset][j].setup(i, j + 1, 0, '0', true);
		}
	}

	//ring 1
	for (char i = 'c'; i < 'f'; i ++)
	{
		for (int j = 2; j < 5; j++)
		{
			positions[i - asciiOffset][j].setup(i, j + 1, 0, '0', true);
		}
	}

	//clear out the middle 
	positions['d' - asciiOffset][4-1].setDisp(' ');
	positions['d' - asciiOffset][4 - 1].setTerminator(true);
	positions['d' - asciiOffset][4 - 1].setValid(false);

	
}

void Board::checkAndChangeState(Player p1, Player p2)
{

	//set mill state
	if (p1.getActiveMills() > 0 || p2.getActiveMills() > 0)
	{
		this->tempState = this->state;
		this->setState(mill);
	}
	//restore the previous state when no more active mills
	if (this->state == mill)
	{
		if (getCurrentPlayer().getActiveMills() == 0)
		{
			this->setState(this->tempState);

		}
	}
	if (this->state == phase1)
	{
		if (p1.getPhase1Pieces() + p2.getPhase1Pieces() == 0)
		{
			this->setState(phase2);
		}
	}

	if (this->state == phase2)
	{
		if (p1.getPhase2Pieces() < 3 || p2.getPhase2Pieces() < 3)
		{
			this->setState(gameOver);
		}
	}
}

bool Board::isValidLocation(char row, int column, char rowSource, int columnSource, Player player)
{
	int x = row - asciiOffset;
	int y = column - 1;
	bool available;
	//if the board state is MILL
	if (this->state == mill)
	{
		bool yaPassed = false;
		/*see if the position selected's state is the opposite of the player.
			No need to check if the location exists because it has to be 
			1 or 2 in the status anyway.*/
		if (/*positions[x][y].isItValid() &&*/ positions[x][y].getStatus() == (3 - player.getPlayerID()))
		{
			yaPassed = true;
			/*if it is part of a mill...*/
			//if (positions[x][y].isPartofMill())
			if (positions[x][y].checkIfPartOfMill(this->positions))
			{
				/*...and if the player only has 3 pieces currently, in any phase...*/
				// check for phase 1 pieces is obsolete because we start at 9 always.
				if (/*BoardPlayers.at(player.getEnemy() - 1).getPhase1Pieces() <= 3
					||*/ BoardPlayers.at(player.getEnemy() - 1).getPhase2Pieces() <= 3)
				{
					//...it can be taken
					return true;
				}
				else
				{
					//...else it was an invalid move
					return false;
				}
			}
			else
			{
				//if it is not part of a mill, and it still passed the original test, then it is still valid.
					return true;
			}
			
		}
		else 
		{
			// else, not valid at all
			return false;
		}
	}
	//if game is in phase 1
	else 
	if (this->state == phase1)
	{
		//simply check if the location exists and is not occupied
		if (positions[x][y].isItValid() && positions[x][y].getStatus() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//if game is in phase 2
	else
	if (this->state == phase2)
	{
		//if the player is in flight mode...
		if (player.getFlight())
		{
			//Repeat phase 1 check: simply check if the location exists and is not occupied
			if (positions[x][y].isItValid() && positions[x][y].getStatus() == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//TODO: Mostly Done, but keep chekcing: finish the check if the position is only one "spot" away for phase 2
		/*Just do a comparison each way: up down left right, and get the first available space each time to compare*/
		else
		{
			//start making temporary variables for the start and goal
			BoardPosition start = positions[rowSource - asciiOffset][columnSource - 1];
			BoardPosition goal = positions[row - asciiOffset][column - 1];

			//check each position to see if they even exist first
			if (start.isItValid() == false)
			{
				return false;
			}
			else
				if(goal.isItValid() == false)
				{
					return false;
				}
				else
				{
					//horizontal forward aka change char
					for (char a = start.getCoord().first; a <= 'g'; a++)
					{
						//get a temporary position based on the start location
						BoardPosition temp = positions[a - asciiOffset][column - 1];
						//if it is the middle position, break out of the loop because it is not valid.
						if (temp.getTerminator())
						{
							break;
						}
						else
						{
							//check if the spot is valid aka exists. It is the first and only location to check because it's one "spot" away.
							if (temp.isItValid())
							{
								//check if the goal location matches it
								if (temp.getCoord().first == goal.getCoord().first
									&& temp.getCoord().second == goal.getCoord().second)
								{
									//if it does, now check if it is occupied
									if (temp.getStatus() == 0)
									{
										/*if it is not, then that location is
											1. one "spot" away
											2.	unoccupied
										and the move is valid. Return true;*/

										return true;
									}
									else
									{
										//else if it is occupied, then wrong move
										return false;
									}
								}
								else
								{
									//else if it doesn't match, then break out and check other 
									break;
								}
							}
							else
							{
								//if the location is not valid, continue traversing.
								continue;
							}
						}
					}
					//horizontal backward aka change char. Hopefully implicit ASCII conversion works.
					for (char a = start.getCoord().first; a >= 'a'; a--)
					{
						//get a temporary position based on the start location
						BoardPosition temp = positions[a - asciiOffset][column - 1];
						//if it is the middle position, break out of the loop because it is not valid.
						if (temp.getTerminator())
						{
							break;
						}
						else
						{
							//check if the spot is valid aka exists. It is the first and only location to check because it's one "spot" away.
							if (temp.isItValid())
							{
								//check if the goal location matches it
								if (temp.getCoord().first == goal.getCoord().first
									&& temp.getCoord().second == goal.getCoord().second)
								{
									//if it does, now check if it is occupied
									if (temp.getStatus() == 0)
									{
										/*if it is not, then that location is
											1. one "spot" away
											2.	unoccupied
										and the move is valid. Return true;*/

										return true;
									}
									else
									{
										//else if it is occupied, then wrong move
										return false;
									}
								}
								else
								{
									//else if it doesn't match, then break out and check other 
									break;
								}
							}
							else
							{
								//if the location is not valid, continue traversing.
								continue;
							}
						}
					}
					//vertical up aka change int
					for (int a = start.getCoord().second + 1 /*skip the current location*/; a <= 7; a++)
					{
						//get a temporary position based on the start location
						BoardPosition temp = positions[row - asciiOffset][a - 1];
						//if it is the middle position, break out of the loop because it is not valid.
						if (temp.getTerminator())
						{
							break;
						}
						else
						{
							//check if the spot is valid aka exists. It is the first and only location to check because it's one "spot" away.
							if (temp.isItValid())
							{
								//check if the goal location matches it
								if (temp.getCoord().first == goal.getCoord().first
									&& temp.getCoord().second == goal.getCoord().second)
								{
									//if it does, now check if it is occupied
									if (temp.getStatus() == 0)
									{
										/*if it is not, then that location is
											1. one "spot" away
											2.	unoccupied
										and the move is valid. Return true;*/

										return true;
									}
									else
									{
										//else if it is occupied, then wrong move
										return false;
									}
								}
								else
								{
									//else if it doesn't match, then break out and check other 
									break;
								}
							}
							else
							{
								//if the location is not valid, continue traversing.
								continue;
							}
						}
					}
					//vertical down aka change int
					for (int a = start.getCoord().second - 1 /*skip the current location*/; a >= 1; a--)
					{
						//get a temporary position based on the start location
						BoardPosition temp = positions[row - asciiOffset][a - 1];
						//if it is the middle position, break out of the loop because it is not valid.
						if (temp.getTerminator())
						{
							break;
						}
						else
						{
							//check if the spot is valid aka exists. It is the first and only location to check because it's one "spot" away.
							if (temp.isItValid())
							{
								//check if the goal location matches it
								if (temp.getCoord().first == goal.getCoord().first
									&& temp.getCoord().second == goal.getCoord().second)
								{
									//if it does, now check if it is occupied
									if (temp.getStatus() == 0)
									{
										/*if it is not, then that location is
											1. one "spot" away
											2.	unoccupied
										and the move is valid. Return true;*/

										return true;
									}
									else
									{
										//else if it is occupied, then wrong move
										return false;
									}
								}
								else
								{
									//else if it doesn't match, then break out and check other 
									break;
								}
							}
							else
							{
								//if the location is not valid, continue traversing to find first adjacent spot.
								continue;
							}
						}
					}
				}
		}

	}
	else
	{
		return false;
	}
}

void Board::doMovePhase1(char row, int column, Player &player)
{
	if (isValidLocation(row, column, 0, 0, player))
	{
		positions[row - asciiOffset][column - 1].setStatus(player.getPlayerID());
		positions[row - asciiOffset][column - 1].setDisp(static_cast<char>('0' + player.getPlayerID()));
		player.subtractPiece(this->state);
		player.addPiece();

		std::cout << "Placed piece." << std::endl;
		pair<bool, int> a = this->isMill(row, column, player);
		switch (a.first)
		{
		case true:
			player.setActiveMills(a.second);
			//should be handled by checkAndChangeState();
			//this->checkAndChangeState(BoardPlayers.at(0), BoardPlayers.at(1));
			break;
		default:
			break;
		}
	}
	else
	{
		std::cout << "Invalid location. Make sure it is a legal spot and is unoccupied" << std::endl;
	}
}

void Board::doMovePhase2(char rowStart, int columnStart, char rowGoal, int columnGoal, Player player)
{
	if (isValidLocation(rowGoal, columnGoal, rowStart, columnStart, player))
	{
		//toggle off where you started
		positions[rowStart - asciiOffset][columnStart - 1].setStatus(0);
		positions[rowStart - asciiOffset][columnStart - 1].setDisp('0');

		//toggle the new location
		positions[rowGoal - asciiOffset][columnGoal - 1].setStatus(player.getPlayerID());
		positions[rowGoal - asciiOffset][columnGoal - 1].setDisp(static_cast<char>(player.getPlayerID()));

		//check for mills
		pair<bool, int> a = this->isMill(rowGoal, columnGoal, player);

		//this->checkIsMillAfterAMove(player);

		switch (a.first)
		{
		case true:
			player.setActiveMills(a.second);
			//this->setState(mill);
			break;
		default:
			break;
		}
		std::cout << "Moved piece" << std::endl;
	}
	else
	{
		std::cout << "Invalid location. Select an adjacent and unoccupied spot." << std::endl;
	}
}

bool Board::removePiece(char row, int column, Player player)
{
	bool toReturn = false;
	if (isValidLocation(row, column, 0, 0, player))
	{
		toReturn = true;
		positions[row - asciiOffset][column - 1].setStatus(0);
		positions[row - asciiOffset][column - 1].setDisp('0');
		int enemy = player.getEnemy() - 1;

		BoardPlayers.at(player.getEnemy() - 1).subtractPiece(this->tempState);
		BoardPlayers.at(player.getPlayerID() -1 ).subtractMills();
		return toReturn;
	}
	return toReturn;
}

pair<bool, int> Board::isMill(char row, int column, Player player)
{
	//number of mills found
	int totalMills = 0;
	//if mills were found
	bool millFound = false;
	//number of vertical adjacencies found. Init at 1 because the current position is counted as part of the imaginary "mill."
	int vAdj = 1;
	//number of horizontal adjacencies found. Init at 1 because the current position is counted as part of the imaginary "mill."
	int hAdj = 1;
	//pair to return
	pair<bool, int> results;
	//vector to keep track of horizontal mills. Is deleted when they don't exist.
	vector<BoardPosition*> hMills;
	//vector to keep track of vertical mills. Is deleted when they don't exist.
	vector<BoardPosition*> vMills;
	//add the current position to the mill vectors
	BoardPosition* x = &positions[row - asciiOffset][column - 1];
	hMills.push_back(x);
	vMills.push_back(x);
	//begin checking in each direction

	//horizontal forward
	for (char a = static_cast<char>(row + 1); a <= 'g'; a++)
	{
		//make a temp position for the search
		BoardPosition temp = positions[a-asciiOffset][column - 1];
		BoardPosition *p = &positions[a - asciiOffset][column - 1];
		//if it is the center position, end the search immediately to prevent going into the other side
		if (temp.getTerminator())
		{
			break;
		}
		else
		//if it is a valid location...
		if(temp.isItValid())
		{
			//...and if the ID of the location is the same as the player, increase the hAdj as we found something adjacent.
			if (temp.getStatus() == player.getPlayerID())
			{
				hMills.push_back(p);
				hAdj++;
			}
			//...and if the ID of the location is not the same as the player, then the search stops right there as it is blocked
			else
			{
				break;
			}
		}
		//if it is not a valid location aka doesn't exist, keep searching in that direction.
		else
		{
			continue;
		}
	}
	//horizontal backward
	for (char a = static_cast<char>(row - 1); a >= 'a'; a--)
	{
		//make a temp position for the search
		BoardPosition temp = positions[a- asciiOffset][column - 1];
		BoardPosition* p = &positions[a - asciiOffset][column - 1];
		//if it is the center position, end the search immediately to prevent going into the other side
		if (temp.getTerminator())
		{
			break;
		}
		else
			//if it is a valid location...
			if (temp.isItValid())
			{
				//...and if the ID of the location is the same as the player, increase the hAdj as we found something adjacent.
				if (temp.getStatus() == player.getPlayerID())
				{
					hMills.push_back(p);
					hAdj++;
				}
				//...and if the ID of the location is not the same as the player, then the search stops right there as it is blocked
				else
				{
					break;
				}
			}
		//if it is not a valid location aka doesn't exist, keep searching in that direction.
			else
			{
				continue;
			}
	}
	//vertical up
	for (int a = column + 1; a <= 7; a++)
	{
		//make a temp position for the search
		BoardPosition temp = positions[row - asciiOffset][a - 1];
		BoardPosition* p = &positions[row - asciiOffset][a - 1];
		//if it is the center position, end the search immediately to prevent going into the other side
		if (temp.getTerminator())
		{
			break;
		}
		else
			//if it is a valid location...
			if (temp.isItValid())
			{
				//...and if the ID of the location is the same as the player, increase the hAdj as we found something adjacent.
				if (temp.getStatus() == player.getPlayerID())
				{
					vMills.push_back(p);
					vAdj++;
				}
				//...and if the ID of the location is not the same as the player, then the search stops right there as it is blocked
				else
				{
					break;
				}
			}
		//if it is not a valid location aka doesn't exist, keep searching in that direction.
			else
			{
				continue;
			}
	}
	//vertical down
	for (int a = column - 1; a >= 1; a--)
	{
		//make a temp position for the search
		BoardPosition temp = positions[row - asciiOffset][a - 1];
		BoardPosition* p = &positions[row - asciiOffset][a - 1];
		//if it is the center position, end the search immediately to prevent going into the other side
		if (temp.getTerminator())
		{
			break;
		}
		else
			//if it is a valid location...
			if (temp.isItValid())
			{
				//...and if the ID of the location is the same as the player, increase the hAdj as we found something adjacent.
				if (temp.getStatus() == player.getPlayerID())
				{
					vMills.push_back(p);
					vAdj++;
				}
				//...and if the ID of the location is not the same as the player, then the search stops right there as it is blocked
				else
				{
					break;
				}
			}
		//if it is not a valid location aka doesn't exist, keep searching in that direction.
			else
			{
				continue;
			}
	}
	//total mills is found by divding each by 3 to see if they had a mill.
	totalMills = (hAdj / 3) + (vAdj / 3);
	//now check if each vector should be kept and their status changed based on the hAdj and vAdj numbers
	/*if (hAdj >= 3 && vAdj >=3)
	{
		player.
	}*/
	
	if (totalMills > 0)
	{
		millFound = true;
		//set the data for that position
		positions[row - asciiOffset][column - 1].setIsMill(true);
	}

	results = make_pair(millFound, totalMills);
	return results;
}

void Board::checkIsMillAfterAMove(Player player)
{

}

void Board::displayBoard()
{
	for (int i = 0; i < 7; i++)
	{
		std:: cout << i + 1 << "| ";
		for (int j = 0; j < 7; j++)
		{
			std::cout << positions[j][i].getDisp();
		}

		std::cout << std::endl;
	}
	std::cout << "   _______" << std::endl;
	std::cout << "   abcdefg" << std::endl;
	std::cout << "Board State: " << this->state << std::endl;
	std::cout << "Player 1: " << this->BoardPlayers.at(0).getPhase1Pieces() << " / " << this->BoardPlayers.at(0).getPhase2Pieces() << std::endl;
	std::cout << "Player 2: " << this->BoardPlayers.at(1).getPhase1Pieces() << " / " << this->BoardPlayers.at(1).getPhase2Pieces() << std::endl;

}

bool Board::phase1Check(char row, int column)
{
	BoardPosition temp = this->positions[row - asciiOffset][column - 1];
	bool toReturn = false;
	if (temp.isItValid() && temp.getStatus() == 0)
	{
		toReturn = true;
	}
	return toReturn;
}

bool Board::phase2_1Check(char sRow, int sColumn)
{
	BoardPosition temp = positions[sRow - asciiOffset][sColumn - 1];
	bool toReturn = false;
	//if the location exists and the ID of the location matches the current player, it is valid.
	if(temp.isItValid() && temp.getStatus() == this->getCurrentPlayer().getPlayerID())
	{
		toReturn = true;
	}
	return toReturn;
}

bool Board::phase2_2Check(char row, int column)
{
	return false;
}
