#ifndef BASEAI
#define BASEAI

#include <list>
#include <iostream>
#include <string>
#include "possiblemove.h"
#include "coordinate.h"
#include "diagonals.h"
#include <utility>

using namespace std;

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

	virtual pair<Coordinate, Coordinate> makeMove() = 0;
	virtual pair<Coordinate, Coordinate> makeJump() = 0;

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

#endif
