#include "BoardPosition.h"

BoardPosition::BoardPosition()
{
	this->coord.first = '0';
	this->coord.second = '0';
}

BoardPosition::BoardPosition(char x, int y)
{
	this->coord.first = x;
	this->coord.second = y;
}

void BoardPosition::setup(char x, int y, int status, char display, bool valid)
{
	this->coord.first = x;
	this->coord.second = y;
	this->locationStatus = status;
	this->locationDisp = display;
	this->isValid = valid;
}

bool BoardPosition::checkIfPartOfMill(BoardPosition positions[][7])
{
	bool result = false;
	//do an internal check to skip having to do each loop
	if (this->isPartofMill())
	{
		return true;
	}
	else
	{
		//Gotta do the checks again...
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
		//begin checking in each direction

		//horizontal forward
		for (char a = static_cast<char>(this->coord.first + 1); a <= 'g'; a++)
		{
			BoardPosition temp = positions[a - offset][this->getCoord().second - 1];
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
					if (temp.getStatus() == this->getStatus())
					{
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
		for (char a = static_cast<char>(this->coord.first + 1); a >= 'a'; a--)
		{
			BoardPosition temp = positions[a - offset][this->getCoord().second - 1];
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
					if (temp.getStatus() == this->getStatus())
					{
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
		//for (int a = column + 1; a <= 7; a++)
		for (int a = this->getCoord().second + 1; a <= 7; a++)
		{
			//BoardPosition temp = board.getPositions()[a][this->getCoord().second - 1];
			BoardPosition temp = positions[this->getCoord().first -offset][a - 1];
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
					if (temp.getStatus() == this->getStatus())
					{
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
		for (int a = this->getCoord().second + 1; a >= 1; a--)
		{
			//BoardPosition temp = board.getPositions()[a][this->getCoord().second - 1];
			BoardPosition temp = positions[this->getCoord().first - offset][a - 1];
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
					if (temp.getStatus() == this->getStatus())
					{
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

		if (hAdj >= 3 || vAdj >= 3)
		{
			this->partOfMill = true;
			result = true;
		}
	}
	return result;
}
