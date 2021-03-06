#ifndef BOARD
#define BOARD

#include <iostream>
#include <string>
#include "coordinate.h"
#include "piece.h"
#include "diagonals.h"

using namespace std;

//////////////////////////
/*BOARD CLASS DEFINITION*/
//////////////////////////

class Board {
public:
	Piece pieces[8][8];
	bool whiteTurn;
	list <Coordinate> temporary;
	Coordinate forcedJumper;
	Coordinate noJump;

	Board();
	
	void display();
	void updateKing(Coordinate piece);
	void swapPieces(Coordinate one, Coordinate two);
	bool move(Coordinate origin, Coordinate destination);
	bool jump(Coordinate origin, Coordinate destination);

	Coordinate calculateJumpee(Coordinate in1, Coordinate in2);
	Coordinate calculateDelta(Coordinate in1, Coordinate in2);

	bool check(Coordinate input);
	bool isEmpty(Coordinate input);
	bool forcedMove(string owner);
	bool checkWhiteMove(Coordinate origin, Coordinate destination);
	bool checkRedMove(Coordinate origin, Coordinate destination);
	bool checkWhiteJump(Coordinate origin, Coordinate destination);
	bool checkRedJump(Coordinate origin, Coordinate destination);
	bool jumperHasAnotherJump(Coordinate input);
	string getOwner(Coordinate input);
};

////////////////////////////////
/*BEGIN BOARD MEMBER FUNCTIONS*/
////////////////////////////////

Board::Board() {
	whiteTurn = true;
	int count = 0;
	int whitePieces[] = { 1, 3, 5, 7, 8, 10, 12, 14, 17, 19, 21, 23 };
	int redPieces[] = { 40, 42, 44, 46, 49, 51, 53, 55, 56, 58, 60, 62 };

	noJump.x = noJump.y = -1;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++, count++) {
			bool placed = false;
			if (count < 24) {
				for (int k = 0; k < 12; k++) {
					if (whitePieces[k] == count) {
						Piece newPiece("white");
						pieces[i][j] = newPiece;
						placed = true;
					}
				}
			}
			else if (count > 39) {
				for (int k = 0; k < 12; k++) {
					if (redPieces[k] == count) {
						Piece newPiece("red");
						pieces[i][j] = newPiece;
						placed = true;
					}
				}
			}

			if (!placed) {
				Piece newPiece("empty");
				pieces[i][j] = newPiece;
			}
		}
	}
}

void Board::display() {
	cout << "   A   B   C   D   E   F   G   H\n";
	for (int i = 0; i < 8; i++) {
		cout << 8 - i /*+ 1*/ << " ";

		for (int j = 0; j < 8; j++) {
			char letter = pieces[i][j].owner[0];

			if (letter == 'w' && pieces[i][j].isKinged)
				letter = 'W';
			else if (letter == 'w')
				letter = 'w';
			else if (letter == 'r' &&  pieces[i][j].isKinged)
				letter = 'R';
			else if (letter == 'r')
				letter = 'r';
			else
				letter = ' ';

			cout << " " << letter << " ";

			if (j != 7)
				cout << "|";
		}

		cout << endl;

		if (i != 7)
			cout << "  -------------------------------\n";
	}
}

void Board::updateKing(Coordinate piece) {
	int x = piece.x;
	int y = piece.y;

	if (pieces[y][x].owner == "white" && y == 7)
		pieces[y][x].isKinged = true;

	if (pieces[y][x].owner == "red" && y == 0)
		pieces[y][x].isKinged = true;
}

void Board::swapPieces(Coordinate one, Coordinate two) {
	Piece temp = pieces[one.y][one.x];
	pieces[one.y][one.x] = pieces[two.y][two.x];
	pieces[two.y][two.x] = temp;
}

bool Board::move(Coordinate origin, Coordinate destination) {
	if (whiteTurn && forcedMove("white")) {
		cout << "White player has a forced move!\n";
		return false;
	}
	else if (!whiteTurn && forcedMove("red")) {
		cout << "Red player has a forced move!\n";
		return false;
	}

	string originOwner = getOwner(origin);
	string destinationOwner = getOwner(destination);

	if (((whiteTurn && originOwner == "white") || (!whiteTurn && originOwner == "red")) && destinationOwner == "empty")  {
		if ((whiteTurn && checkWhiteMove(origin, destination)) || (!whiteTurn && checkRedMove(origin, destination))) {
			swapPieces(origin, destination);
			updateKing(destination);
			whiteTurn = !whiteTurn;
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

bool Board::jump(Coordinate origin, Coordinate destination) {
	if (whiteTurn && !forcedMove("white"))
		return false;
	if (!whiteTurn && !forcedMove("red"))
		return false;
	if (forcedJumper != noJump && forcedJumper != origin)
		return false;

	Coordinate delta = calculateDelta(origin, destination);

	if (delta.x != 2 && delta.x != -2)
		return false;
	if (delta.y != 2 && delta.y != -2)
		return false;

	if (!check(origin) || !check(destination))
		return false;

	Piece newEmptyPiece("empty");
	Coordinate jumpee = calculateJumpee(origin, destination);

	if ((whiteTurn && checkWhiteJump(origin, destination)) || (!whiteTurn && checkRedJump(origin, destination))) {
		swapPieces(origin, destination);
		pieces[jumpee.y][jumpee.x] = newEmptyPiece;
		updateKing(destination);

		if (jumperHasAnotherJump(destination)) {
			forcedJumper = destination;
		}
		else {
			whiteTurn = !whiteTurn;
			forcedJumper = noJump;
		}
	}
	else
		return false;

	return true;
}

Coordinate Board::calculateJumpee(Coordinate in1, Coordinate in2) {
	Coordinate delta = calculateDelta(in1, in2);

	Coordinate jumpee;

	if (delta.x == 2)
		jumpee.x = in1.x + 1;
	else if (delta.x == -2)
		jumpee.x = in1.x - 1;

	if (delta.y == 2)
		jumpee.y = in1.y + 1;
	else if (delta.y == -2)
		jumpee.y = in1.y - 1;

	return jumpee;
}

Coordinate Board::calculateDelta(Coordinate in1, Coordinate in2) {
	Coordinate value;
	value.x = in2.x - in1.x;
	value.y = in2.y - in1.y;

	return value;
}

bool Board::check(Coordinate input) {
	return ((input.x >= 0 && input.x <= 7) && (input.y >= 0 && input.y <= 7));
}

bool Board::isEmpty(Coordinate input) {
	return (pieces[input.y][input.x].owner == "empty");
}

bool Board::forcedMove(string owner) {
	string enemy;

	if (owner == "white")
		enemy = "red";
	else if (owner == "red")
		enemy = "white";
	else
		return false;


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j].owner == "white" && owner == "white") {
				Coordinate temp(j, i);
				Diagonals d(temp);

				//check for up left jump...
				if (check(d.ul) && check(d.ulul) && pieces[i][j].isKinged) {
					if (getOwner(d.ul) == enemy && isEmpty(d.ulul))
						return true;
				}

				//check for up right jump...
				if (check(d.ur) && check(d.urur) && pieces[i][j].isKinged) {
					if (getOwner(d.ur) == enemy && isEmpty(d.urur))
						return true;
				}

				//check for down left jump...
				if (check(d.dl) && check(d.dldl)) {
					if (getOwner(d.dl) == enemy && isEmpty(d.dldl))
						return true;
				}

				//check for down right jump...
				if (check(d.dr) && check(d.drdr)) {
					if (getOwner(d.dr) == enemy && isEmpty(d.drdr))
						return true;
				}
			}
			else if (pieces[i][j].owner == "red" && owner == "red") {
				Coordinate temp(j, i);
				Diagonals d(temp);

				//check for up left jump...
				if (check(d.ul) && check(d.ulul)) {
					if (getOwner(d.ul) == enemy && isEmpty(d.ulul))
						return true;
				}

				//check for up right jump...
				if (check(d.ur) && check(d.urur)) {
					if (getOwner(d.ur) == enemy && isEmpty(d.urur))
						return true;
				}

				//check for down left jump...
				if (check(d.dl) && check(d.dldl) && pieces[i][j].isKinged) {
					if (getOwner(d.dl) == enemy && isEmpty(d.dldl))
						return true;
				}

				//check for down right jump...
				if (check(d.dr) && check(d.drdr) && pieces[i][j].isKinged) {
					if (getOwner(d.dr) == enemy && isEmpty(d.drdr))
						return true;
				}
			}
		}
	}

	return false;
}

bool Board::checkWhiteMove(Coordinate origin, Coordinate destination) {
	Coordinate delta = calculateDelta(origin, destination);

	if (delta.x != 1 && delta.x != -1)
		return false;
	if (delta.y != 1 && delta.y != -1)
		return false;

	if (delta.y == -1 && !pieces[origin.y][origin.x].isKinged)
		return false;

	if (whiteTurn && getOwner(origin) != "white" || getOwner(destination) != "empty")
		return false;

	return true;
}

bool Board::checkRedMove(Coordinate origin, Coordinate destination) {
	Coordinate delta = calculateDelta(origin, destination);

	if (delta.x != 1 && delta.x != -1)
		return false;
	if (delta.y != 1 && delta.y != -1)
		return false;

	if (delta.y == 1 && !pieces[origin.y][origin.x].isKinged)
		return false;

	if (whiteTurn && getOwner(origin) != "red" || getOwner(destination) != "empty")
		return false;

	return true;
}

bool Board::checkWhiteJump(Coordinate origin, Coordinate destination) {
	Coordinate delta = calculateDelta(origin, destination);
	Coordinate jumpee = calculateJumpee(origin, destination);

	if (delta.x != 2 && delta.x != -2)
		return false;
	if (delta.y != 2 && delta.y != -2)
		return false;
	if (delta.y == -2 && !pieces[origin.y][origin.x].isKinged)
		return false;
	if (whiteTurn && getOwner(origin) != "white" || getOwner(jumpee) != "red" || getOwner(destination) != "empty")
		return false;

	return true;
}

bool Board::checkRedJump(Coordinate origin, Coordinate destination) {
	Coordinate delta = calculateDelta(origin, destination);
	Coordinate jumpee = calculateJumpee(origin, destination);

	if (delta.x != 2 && delta.x != -2)
		return false;
	if (delta.y != 2 && delta.y != -2)
		return false;
	if (delta.y == 2 && !pieces[origin.x][origin.y].isKinged)
		return false;
	if (whiteTurn && getOwner(origin) != "red" || getOwner(jumpee) != "white" || getOwner(destination) != "empty")
		return false;

	return true;
}

string Board::getOwner(Coordinate input) {
	return pieces[input.y][input.x].owner;
}

bool Board::jumperHasAnotherJump(Coordinate input) {
	Diagonals d(input);
	int x = input.x;
	int y = input.y;
	string color = pieces[y][x].owner;
	bool isKinged = pieces[y][x].isKinged;
	bool hasJump = false;

	if (color == "red") {
		if (isKinged) {
			if (check(d.dl) && (check(d.dldl)))
				if (getOwner(d.dl) == "white" && isEmpty(d.dldl)) {
					temporary.push_back(d.dldl);
					hasJump = true;
				}
			if (check(d.dr) && (check(d.drdr)))
				if (getOwner(d.dr) == "white" && isEmpty(d.drdr)) {
					temporary.push_back(d.drdr);
					hasJump = true;
				}
		}
		if (check(d.ul) && (check(d.ulul)))
			if (getOwner(d.ul) == "white" && isEmpty(d.ulul)) {
				temporary.push_back(d.ulul);
				hasJump = true;
			}
		if (check(d.ur) && (check(d.urur)))
			if (getOwner(d.ur) == "white" && isEmpty(d.urur)) {
				temporary.push_back(d.urur);
				hasJump = true;
			}
	}
	else if (color == "white") {
		if (isKinged) {
			if (check(d.ul) && (check(d.ulul)))
				if (getOwner(d.ul) == "red" && isEmpty(d.ulul)) {
					temporary.push_back(d.ulul);
					hasJump = true;
				}
			if (check(d.ur) && (check(d.urur)))
				if (getOwner(d.ur) == "red" && isEmpty(d.urur)) {
					temporary.push_back(d.urur);
					hasJump = true;
				}
		}
		if (check(d.dl) && (check(d.dldl)))
			if (getOwner(d.dl) == "red" && isEmpty(d.dldl)) {
				temporary.push_back(d.dldl);
				hasJump = true;
			}	
		if (check(d.dr) && (check(d.drdr)))
			if (getOwner(d.dr) == "red" && isEmpty(d.drdr)) {
				temporary.push_back(d.drdr);
				hasJump = true;
			}
	}

	return hasJump;
}

#endif
