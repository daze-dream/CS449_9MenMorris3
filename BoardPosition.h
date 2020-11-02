#pragma once
#include <functional>
#include <utility>
#include <tuple>


using std::pair;

class BoardPosition
{
public:
	BoardPosition();
	BoardPosition(char x, int y);
	void setCoord(char x, int y) { this->coord.first = x; this->coord.second = y; };
	/*sets the ID of the location*/
	void setStatus(int x) { this->locationStatus = x; };
	void setDisp(char x) { this->locationDisp = x; };
	void setIsMill(bool x) { this->partOfMill = x; };
	/*sets up the locations X and Y pair, 
		the ID, 
		what should be displayed, 
		and if it is a valid move to differentiate from non-existant locations*/
	void setup(char x, int y, int status, char display, bool valid);
	char getDisp() { return this->locationDisp; };
	int getStatus() { return this->locationStatus; };
	//gets the pair of coord. Remember that first[a-z] and second[1-7] so be careful with data structures
	pair<char, int> getCoord() { return this->coord; };
	//special functions to set up the board
	void setTerminator(bool x) { this->terminateSpot = x; };
	bool getTerminator() { return this->terminateSpot; };
	void setValid(bool x) { this->isValid = x; };
	bool isItValid() { return this->isValid; };
	/*This just returns the variable, does not do a check*/
	bool isPartofMill() { return this->partOfMill; };
	/*maybe instead let each piece check if it is part of a mill by itself dynamically, every time it is
		selected. Distinction: This does a check and modifies its self variable and returns a bool*/
	bool checkIfPartOfMill(BoardPosition positions[][7]);
private:
	// Remember that first[a-z] and second[1-7] so be careful with data structures
	pair<char, int> coord = std::make_pair('0',0);
	//vector<std::unique_ptr<BoardPosition>> MillPals;
	int locationStatus = 0; // this is based on player ID. 0 means empty, 3 means connector.
	char locationDisp = ' '; // what to display.
	bool isValid = false; //if it is a valid place to place a move
	bool partOfMill = false;
	bool terminateSpot = false; //special case to stop the search for the middle section
	int offset = 97;
};

