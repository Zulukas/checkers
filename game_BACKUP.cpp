#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <random>
#include <ctime>
#include <cstdlib>
#include <pigpio.h>
#include "coordinate.h"
#include "motor.h"
using namespace std;
/*
const int DELAY = 8500; //8.5mS

struct Mover {
	short y;
	short x;
} mover;

struct gpioSet {
	int a1, a2, b1, b2;
} X, Y;

void moveToCoordinate(Coordinate input);
void moveOnecoordinate(char axis, bool forward);
void accountForGap(bool forward);
void stepX(int a, int b, int c, int d);
void stepY(int a, int b, int c, int d);
void stepForward(char motor);
void stepBackward(char motor);
void controlsOff();
bool setup();*/

/*
void stepX(int a, int b, int c, int d) {
	gpioSetMode(X.a1, a);
	gpioSetMode(X.a2, b);
	gpioSetMode(X.b1, c);
	gpioSetMode(X.b2, d);
}

void stepY(int a, int b, int c, int d) {
	gpioSetMode(Y.a1, a);
	gpioSetMode(Y.a2, b);
	gpioSetMode(Y.b1, c);
	gpioSetMode(Y.b2, d);
}

void stepForward(char motor) {
	if (motor == 'X') {
		stepX(1,0,1,0);
		gpioDelay(DELAY);
		stepX(0,1,1,0);
		gpioDelay(DELAY);
		stepX(0,1,0,1);
		gpioDelay(DELAY);
		stepX(1,0,0,1);
		gpioDelay(DELAY);
	}
	else if (motor == 'Y') {
		stepY(1,0,1,0);
		gpioDelay(DELAY);
		stepY(0,1,1,0);
		gpioDelay(DELAY);
		stepY(0,1,0,1);
		gpioDelay(DELAY);
		stepY(1,0,0,1);
		gpioDelay(DELAY);
	}
}

void stepBackward(char motor) {
	if (motor == 'X') {
		stepX(1,0,0,1);
		gpioDelay(DELAY);
		stepX(0,1,0,1);
		gpioDelay(DELAY);
		stepX(0,1,1,0);
		gpioDelay(DELAY);
		stepX(1,0,1,0);
		gpioDelay(DELAY);
	}
	else if (motor == 'Y') {
		stepY(1,0,0,1);
		gpioDelay(DELAY);
		stepY(0,1,0,1);
		gpioDelay(DELAY);
		stepY(0,1,1,0);
		gpioDelay(DELAY);
		stepY(1,0,1,0);
		gpioDelay(DELAY);
	}
	
}

//Disable output during periods of unuse.
void controlsOff() {
	gpioWrite(X.a1, 0);
	gpioWrite(X.a2, 0);
	gpioWrite(X.b1, 0);
	gpioWrite(X.b2, 0);
	gpioWrite(Y.a1, 0);
	gpioWrite(Y.a2, 0);
	gpioWrite(Y.b1, 0);
	gpioWrite(Y.b2, 0);
}

//Setup the GPIO for use
bool setup() {
	X.a1 = 5;
	X.a2 = 6;
	X.b1 = 13;
	X.b2 = 19;
	Y.a1 = 26;
	Y.a2 = 16;
	Y.b1 = 20;
	Y.b2 = 21;
	
	if (gpioInitialise()) {
		//gpioSet X initialized to {5,6,13,19}
		gpioSetMode(X.a1, PI_OUTPUT);
		gpioSetMode(X.a2, PI_OUTPUT);
		gpioSetMode(X.b1, PI_OUTPUT);
		gpioSetMode(X.b2, PI_OUTPUT);
		
		//gpioSet Y initialized to {26,16,20,21}
		gpioSetMode(Y.a1, PI_OUTPUT);
		gpioSetMode(Y.a2, PI_OUTPUT);
		gpioSetMode(Y.b1, PI_OUTPUT);
		gpioSetMode(Y.b2, PI_OUTPUT);
		
		cout << "MOTOR CONTROL ONLINE!  READY TO GO!\n";
		controlsOff();
		return true;
	}
	
	cout << "FAILURE TO INITIALIZE GPIO SET!  GAME IS ENDING\n";
	return false;
}*/

//////////////////////////
/*PIECE CLASS DEFINITION*/
//////////////////////////

class Piece {
public:
	string owner;
	bool isKinged;
	bool isValid;
	
	Piece() {
		owner = "empty";
		isKinged = false;
	}	
	
	Piece(string owner) {
		this->owner = owner;
		isKinged = false;
	}
};

///////////////////////////////
/*COORDINATE CLASS DEFINITION*/
///////////////////////////////
/*
class Coordinate {
public:
	int x;
	int y;

	Coordinate() {
		this->x = -1;
		this->y = -1;
	}

	Coordinate(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getX() const { return x; }
	int getY() const { return y; }

	bool operator == (const Coordinate & rhs) {
		return ((getX() == rhs.getX()) && (getY() == rhs.getY()));
	}

	bool operator != (const Coordinate & rhs) {
		return ((getX() != rhs.getX() || getY() != rhs.getY()));
	}

	friend ostream & operator << (ostream & out, const Coordinate & rhs) {
		out << "(" << rhs.x << ", " << rhs.y << ")";

		return out;
	}
};
*/
//////////////////////////////
/*DIAGONALS CLASS DEFINITION*/
//////////////////////////////

class Diagonals {
public:
	Coordinate ul, ulul, ur, urur, dl, dldl, dr, drdr;
	
	Diagonals (Coordinate center) {
		int i = center.x;
		int j = center.y;

		ul.x = i - 1;
		ul.y = j - 1;

		ulul.x = i - 2;
		ulul.y = j - 2;

		ur.x = i + 1;
		ur.y = j - 1;

		urur.x = i + 2;
		urur.y = j - 2;

		dl.x = i - 1;
		dl.y = j + 1;

		dldl.x = i - 2;
		dldl.y = j + 2;

		dr.x = i + 1;
		dr.y = j + 1;

		drdr.x = i + 2;
		drdr.y = j + 2;
	}
};

////////////////////////////////
/*POSSIBLEMOVE CLASS DEFINITON*/
////////////////////////////////

class PossibleMove {
public:
	Coordinate location;
	list<Coordinate> possibles;

	PossibleMove(Coordinate location) {
		this->location = location;
	}

	PossibleMove & operator = (const PossibleMove &rhs) {
		this->location = rhs.location;
		this->possibles = rhs.possibles;

		return *this;
	}
};

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
	void move(Coordinate origin, Coordinate destination);
	void jump(Coordinate origin, Coordinate destination);

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
		cout << i + 1 << " ";

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

void Board::move(Coordinate origin, Coordinate destination) {
	if (whiteTurn && forcedMove("white")) {
		cout << "White player has a forced move!\n";
		return;
	}
	else if (!whiteTurn && forcedMove("red")) {
		cout << "Red player has a forced move!\n";
		return;
	}

	string originOwner = getOwner(origin);
	string destinationOwner = getOwner(destination);

	if (((whiteTurn && originOwner == "white") || (!whiteTurn && originOwner == "red")) && destinationOwner == "empty")  {
		if ((whiteTurn && checkWhiteMove(origin, destination)) || (!whiteTurn && checkRedMove(origin, destination))) {
			swapPieces(origin, destination);
			updateKing(destination);
			whiteTurn = !whiteTurn;
		}
	}
}

void Board::jump(Coordinate origin, Coordinate destination) {
	if (whiteTurn && !forcedMove("white"))
		return;
	if (!whiteTurn && !forcedMove("red"))
		return;
	if (forcedJumper != noJump && forcedJumper != origin)
		return;

	Coordinate delta = calculateDelta(origin, destination);

	if (delta.x != 2 && delta.x != -2)
		return;
	if (delta.y != 2 && delta.y != -2)
		return;

	if (!check(origin) || !check(destination))
		return;

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

/////////////////////////////////////
/*BEGIN BASEAI BASE CLASS DEFINTION*/
/////////////////////////////////////

class baseAI {
public:
	string color;
	list<PossibleMove> moves;
	list<PossibleMove> jumps;
	Board *board;

	baseAI();
	baseAI(string color, Board *board);
	void update();
	void displayPossibilities();
	void displayPossibilityAtCoordinate(Coordinate in);

	bool canMoveFrom(Coordinate here, PossibleMove &pm);
	bool canJumpFrom(Coordinate here, PossibleMove &pm);
	bool gameIsOver();

	virtual void makeMove() = 0;
	virtual void makeJump() = 0;

private:
	bool jumpCheck(Coordinate one, Coordinate two, string color);
	bool moveCheck(Coordinate one);
	void determineMoves();
	void determineJumps();
};

/////////////////////////////////
/*BEGIN BASEAI CLASS DEFINTIONS*/
/////////////////////////////////

baseAI::baseAI() {

}

baseAI::baseAI(string color, Board *board) {
	this->color = color;
	this->board = board;

	determineMoves();
	determineJumps();
}

//Determines if a Coordinate is legal and empty
bool baseAI::moveCheck(Coordinate one) {
	return (board->check(one) && board->isEmpty(one));
}

//Determines if a jump vector is legal
bool baseAI::jumpCheck(Coordinate one, Coordinate two, string color) {
	return (board->check(one) && board->getOwner(one) == color && board->check(two) && board->isEmpty(two));
}

bool baseAI::canMoveFrom(Coordinate here, PossibleMove &pm) {
	Diagonals d(here);
	bool canJump = false;

	//White player's move 'up' (y+); check up left and up right diagonals
	if (color == "white") {
		if (moveCheck(d.dl)) {
			canJump = true;
			pm.possibles.push_back(d.dl);
		}
		if (moveCheck(d.dr)) {
			canJump = true;
			pm.possibles.push_back(d.dr);
		}
	}
	//Red player's move 'down' (y-); check down left and down right diagonals
	if (color == "red") {
		if (moveCheck(d.ul)) {
			canJump = true;
			pm.possibles.push_back(d.ul);
		}
		if (moveCheck(d.ur)) {
			canJump = true;
			pm.possibles.push_back(d.ur);
		}
	}

	if (board->pieces[here.y][here.x].isKinged) {
		if (color == "white") {
			//Check the 'down' moves for white if it is kinged.
			if (moveCheck(d.ul)) {
				canJump = true;
				pm.possibles.push_back(d.ul);
			}
			if (moveCheck(d.ur)) {
				canJump = true;
				pm.possibles.push_back(d.ur);
			}
		}
		if (color == "red") {
			//Check the 'up' moves for red if it is kinged.
			if (moveCheck(d.dl)) {
				canJump = true;
				pm.possibles.push_back(d.dl);
			}
			if (moveCheck(d.dr)) {
				canJump = true;
				pm.possibles.push_back(d.dr);
			}
		}
	}

	return canJump;
}

bool baseAI::canJumpFrom(Coordinate here, PossibleMove &pm) {
	Diagonals d(here);
	bool canJump = false;

	//White player's move 'up' (y+); check up left and up right diagonals
	if (color == "white") {
		if (jumpCheck(d.dl, d.dldl, "red")) {
			canJump = true;
			pm.possibles.push_back(d.dldl);
		}
		if (jumpCheck(d.dr, d.drdr, "red")) {
			pm.possibles.push_back(d.drdr);
			canJump = true;
		}
	}
	//Red player's move 'down' (y-); check down left and down right diagonals
	if (color == "red") {
		if (jumpCheck(d.ul, d.ulul, "white")) {
			pm.possibles.push_back(d.ulul);
			canJump = true;
		}
		if (jumpCheck(d.ur, d.urur, "white")) {
			pm.possibles.push_back(d.urur);
			canJump = true;
		}
	}

	if (board->pieces[here.y][here.x].isKinged) {
		if (color == "white") {
			//Check the 'down' moves for white if it is kinged.
			if (jumpCheck(d.ul, d.ulul, "red")) {
				pm.possibles.push_back(d.ulul);
				canJump = true;
			}
			if (jumpCheck(d.ur, d.urur, "red")) {
				pm.possibles.push_back(d.urur);
				canJump = true;
			}
		}
		if (color == "red") {
			//Check the 'up' moves for red if it is kinged.
			if (jumpCheck(d.dl, d.dldl, "white")) {
				pm.possibles.push_back(d.dldl);
				canJump = true;
			}
			if (jumpCheck(d.dr, d.drdr, "white")) {
				pm.possibles.push_back(d.drdr);
				canJump = true;
			}
		}
	}

	return canJump;
}

bool baseAI::gameIsOver() {
	int white = 0;
	int red = 0;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board->pieces[j][i].owner == "white")
				white++;
			if (board->pieces[j][i].owner == "red")
				red++;
		}
	}
	
	if (white == 0) {
		cout << "Red has won the game!\n";
		return true;
	}
	if (red == 0) {
		cout << "White has won the game!\n";
		return true;
	}
	
	determineMoves();
	determineJumps();
	string enemy;
	
	if (color == "red")
		enemy = "white";
	else
		enemy = "red";
	
	if (moves.size() == 0 && jumps.size() == 0) {
		cout << color <<  " has no moves! " << enemy << " has won the game!\n";
		return true;
	}
	
	list<PossibleMove> enemyMoves;
	list<PossibleMove> enemyJumps;
	
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board->pieces[y][x].owner == enemy) {
				Coordinate here(x, y);
				PossibleMove pm(here);

				if (canMoveFrom(here, pm)) {					
					enemyMoves.push_back(pm);
				}
			}
		}
	}
	
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board->pieces[y][x].owner == enemy) {
				Coordinate here(x, y);
				PossibleMove pm(here);

				if (canJumpFrom(here, pm)) {
					enemyJumps.push_back(pm);
				}
			}
		}
	}
	
	if (enemyMoves.size() == 0 && enemyJumps.size() == 0) {
		cout << enemy << " has no moves! " << color << " has won the game!\n";
		return true;
	}
	
	return false;
}

void baseAI::determineMoves() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board->pieces[y][x].owner == color) {
				Coordinate here(x, y);
				PossibleMove pm(here);

				if (canMoveFrom(here, pm)) {					
					moves.push_back(pm);
				}
			}
		}
	}
}

void baseAI::determineJumps() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board->pieces[y][x].owner == color) {
				Coordinate here(x, y);
				PossibleMove pm(here);

				if (canJumpFrom(here, pm)) {
					jumps.push_back(pm);
				}
			}
		}
	}
}

void baseAI::displayPossibilityAtCoordinate(Coordinate in) {
	for (list<PossibleMove>::iterator it = moves.begin(); it != moves.end(); it++) {
		PossibleMove temp = *it;
		Coordinate c = temp.location;

		if (c == in) {
			cout << "Possible Moves from " << c << ":\n\t";
			int count = 0;
			for (list<Coordinate>::iterator it2 = temp.possibles.begin(); it2 != temp.possibles.end(); count++) {				
				Coordinate co = *it2;
				cout << co;

				it2++;

				if (it2 != temp.possibles.end()) {
					cout << ", ";
				}
			}

			if (count == 0) {
				cout << "NO POSSIBLE MOVES, SOMETHING WENT WRONG!\n";
			}
		}
	}
}

void baseAI::displayPossibilities() {
	update();
	cout << "POSSIBLE MOVES: ";

	int numMoves = 0;
	for (list<PossibleMove>::iterator it = moves.begin(); it != moves.end();) {
		PossibleMove temp = *it; 
		cout << temp.location;
		numMoves++;
		it++;

		if (it != moves.end()) {
			cout << ", ";
		}
	}

	if (numMoves == 0) {
		cout << "No possible moves!\n";
	}
	else {
		cout << endl;
	}

	cout << "POSSIBLE JUMPS: ";

	numMoves = 0;
	for (list<PossibleMove>::iterator it = jumps.begin(); it != jumps.end();) {
		PossibleMove temp = *it;
		cout << temp.location;
		it++;
		numMoves++;

		if (it != jumps.end()) {
			cout << ", ";
		}
	}

	if (numMoves == 0) {
		cout << "No possible jumps!\n";
	}
	else {
		cout << endl;
	}
}

void baseAI::update() {
	moves.clear();
	jumps.clear();

	determineMoves();
	determineJumps();
}

//////////////////////////////////
/*BEGIN RANDOMAI CLASS DEFINTION*/
//////////////////////////////////

class randomAI : public baseAI {
public:
	randomAI(string color, Board *board) {
		this->color = color;
		this->board = board;
	}

	void makeJump();
	void makeMove();
};

///////////////////////////////////
/*BEGIN RANDOMAI MEMBER FUNCTIONS*/
///////////////////////////////////

void randomAI::makeJump() {
	if (jumps.size() == 0) {
		makeMove();
		return;
	}

	if (board->forcedJumper != board->noJump && board->getOwner(board->forcedJumper) == color) {
		jumps.clear();
		PossibleMove pm(board->forcedJumper);

		list<Coordinate> l = board->temporary;

		for (list<Coordinate>::iterator it = l.begin(); it != l.end(); it++) {
			Coordinate temp = *it;
			pm.possibles.push_back(temp);
		}

		jumps.push_back(pm);
	}

	int number = 0;
	srand (time(NULL));

	if (jumps.size() > 1)		
		number = rand() % (jumps.size() - 1);

	int count = 0;

	for (list<PossibleMove>::iterator it = jumps.begin(); it != jumps.end(); it++, count++) {
		if (count == number) {
			int choose = 0;

			PossibleMove thisMove = *it;

			if (thisMove.possibles.size() > 1) {
				choose = rand() % (thisMove.possibles.size() - 1);
			}

			int count2 = 0;
			for (list<Coordinate>::iterator it2 = thisMove.possibles.begin(); it2 != thisMove.possibles.end(); it2++, count2++) {
				if (count2 == choose) {
					Coordinate destination = *it2;
					Coordinate origin = thisMove.location;

					cout << "ATTEMPTING TO JUMP FROM ORIGIN: " << origin << " TO DESTINATION: " << destination << endl;
					board->jump(origin, destination);
				}
			}
		}
	}
}

void randomAI::makeMove() {
	if (jumps.size() != 0) {
		makeJump();
		return;
	}

	int number = 0;
	srand(time(NULL));

	if (moves.size() > 1)
		number = rand() % (moves.size() - 1);

	int count = 0;

	for (list<PossibleMove>::iterator it = moves.begin(); it != moves.end(); it++, count++) {
		if (count == number) {
			int choose = 0;

			PossibleMove thisMove = *it;

			if (thisMove.possibles.size() > 1)
				choose = rand() % (thisMove.possibles.size() - 1);

			int count2 = 0;

			for (list <Coordinate>::iterator it2 = thisMove.possibles.begin(); it2 != thisMove.possibles.end(); it2++, count2++) {
				if (count2 == choose) {
					Coordinate destination = *it2;
					Coordinate origin = thisMove.location;

					cout << "ATTEMPTING TO MOVE FROM ORIGIN: " << origin << " TO DESTINATION " << destination << endl;
					board->move(origin, destination);
				}
			}
		}
	}
}

/////////////////////////////////
/*BEGIN MOTOR CONTROLS FOR GAME*/
/////////////////////////////////
/*
void accountForGap(bool forward) {
	if (forward) {
		//stepForward('Y');
	}
	else {
		//stepBackward('Y');
	}
}

void moveOneCoordinate(char axis, bool forward) {
	const int steps = 10;
	
	if (axis != 'X' && axis != 'Y')
		return;
	
	for (int i = 0; i < steps; i++) {
		if (forward)
			stepForward(axis);
		else
			stepBackward(axis);
	}
}

void moveToCoordinate(Coordinate input) {
	if (input.x > 7 || input.x < 0 || input.y < -1 || input.y > 7) {
		cout << "Invalid input coordinate, destination is out of bounds\n";
		return;
	}
	
	//Get the distances to move....
	int x = input.x - mover.x;
	int y = input.y - mover.y;
	
	if ((x + mover.x > 7) || (x + mover.x < 0) || (y + mover.y > 7) || (y + mover.y < 0)) {
		cout << "Mover calibration necessary, please reset to origin\n";
		cout << "Press any key when calibrated to origin!\n";
		string something;
		cin >> something;
		mover.x = 0;
		mover.y = -1;
		
		x = input.x - mover.x;
		y = input.y - mover.y;
	}
	
	if (x < 0)
		for (int i = x; i > 0; i--) {
			stepBackward('X');
			mover.x--;
			
			if (mover.x == 0 && (i-1) > 0) {
				cout << "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
				break;
			}
		}
	else 
		for (int i = 0; i < x; i++) {
			stepForward('X');
			mover.x++;
			
			if (mover.x == 7 && (i+1) < x) {
				cout << "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
				break;
			}
		}
		
	if (y < 0)
		for (int i = y; i > 0; i--) {
			stepBackward('Y');
			mover.y--;
			
			if ((y-1) > 0) {
				if (mover.y == 0) {
					cout << "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
					break;
				}
				
				if (i == 4) {
					accountForGap(false); //negative
				}
			}
		}
	else
		for (int i = 0; i < y; i++) {
			stepForward('Y');
			mover.y++;
			
			if ((i+1) < y) {
				if (mover.y == 7 && (i+1) < y) {
					cout << "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
					break;
				}
				
				if (i == 3) {
					accountForGap(true); //positive
				}
			}
		}
}
*/
//////////////////////////////////
/*BEGIN GAME DEPENDANT FUNCTIONS*/
//////////////////////////////////

Coordinate interpret(string input) throw (const char*) {
	const char* invalidSize = "ERROR:  INPUT TOO LARGE!";
	const char* invalidX = "ERROR: X  INPUT MUST BE 'A'-'H'!";
	const char* invalidY = "ERROR: Y  INPUT MUST BE '1'-'8'!";

	cout << input << endl;

	if (input.size() != 2)
		throw invalidSize;
	if (input[0] < 'a' || input[0] > 'h')
		throw invalidX;
	if (input[1] < '1' || input[1] > '8')
		throw invalidY;

	Coordinate result((int)input[0] - 'a', (int)input[1] - '1');

	return result;
}

void start() {
	string garbageInputThatWontEverBeUsed;
	cout << "Game is initializing!  Adjust the mover to the starting location if not so!\n";
	cout << "Starting location: X and Y rails are fully retracted.";
	cout << "Press any key when ready (\"Where is the dang \"any\" key!?\"): ";
	cin >> garbageInputThatWontEverBeUsed;
}

//int _tmain(int argc, _TCHAR* argv[])
int main()
{
	start();
	
	if (!setup())
		return 0;
	
	Board board;
	randomAI red("red", &board);

	board.display();

	while (1) {
		string player = board.whiteTurn ? "white" : "red";
		string input;

		red.displayPossibilities();

		if (!board.whiteTurn) {
			//system("pause");
			red.makeMove();
			continue;
		}
		else {
			cout << endl;
			board.display();
			cout << endl;
		}

		cout << player << " > ";
		getline(cin, input);

		string parts[3];
		int wordMarker = 0;
		for (int i = 0; i < input.size(); i++) {
			if (input[i] == ' ') {
				wordMarker++;

				if (wordMarker > 2)
					break;
			}
			else
				parts[wordMarker] += tolower(input[i]);
		}

		if (parts[0] == "move") {
			try {
				Coordinate origin = interpret(parts[1]);
				Coordinate destination = interpret(parts[2]);

				board.move(origin, destination);
			}
			catch (const char* ex) {
				cout << ex << endl;
			}

			//continue;
		}
		if (parts[0] == "jump") {
			try {
				Coordinate origin = interpret(parts[1]);
				Coordinate destination = interpret(parts[2]);

				board.jump(origin, destination);
			}
			catch (const char* ex) {
				cout << ex << endl;
			}

			//continue;
		}
		if (parts[0] == "save") {
			ofstream fout("game.txt");

			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 8; x++) {
					string owner = board.pieces[y][x].owner;
					char letter;

					if (owner == "white") {
						if (board.pieces[y][x].isKinged)
							letter = 'W';
						else
							letter = 'w';
					}
					else if (owner == "red") {
						if (board.pieces[y][x].isKinged)
							letter = 'R';
						else
							letter = 'r';
					}
					else
						letter = '0';

					fout << letter << " ";
				}

				fout << endl;
			}
		}
		if (parts[0] == "load") {
			ifstream fin("game.txt");

			for (int y = 0; y < 8; y++)  {
				for (int x = 0; x < 8; x++) {
					char letter;
					fin >> letter;

					if (letter == 'R') {
						board.pieces[y][x].owner = "red";
						board.pieces[y][x].isKinged = true;
					}
					else if (letter == 'r') {
						board.pieces[y][x].owner = "red";
						board.pieces[y][x].isKinged = false;
					}
					else if (letter == 'W') {
						board.pieces[y][x].owner = "white";
						board.pieces[y][x].isKinged = true;
					}
					else if (letter == 'w') {
						board.pieces[y][x].owner = "white";
						board.pieces[y][x].isKinged = false;
					}
					else {
						board.pieces[y][x].owner = "empty";
						board.pieces[y][x].isKinged = false;
					}
				}
			}
		}
		if (parts[0] == "quit") {


			break;
		}
		if (parts[0] == "what") {
			Coordinate check = interpret(parts[1]);
			red.displayPossibilityAtCoordinate(check);
		}

		if (red.gameIsOver())
			break;
	}

	return 0;
}
