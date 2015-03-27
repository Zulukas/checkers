#ifndef RANDOMAI
#define RANDOMAI

#include "baseai.h"
#include "coordinate.h"
#include "possiblemove.h"
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

//////////////////////////////////
/*BEGIN RANDOMAI CLASS DEFINTION*/
//////////////////////////////////

class randomAI : public baseAI {
public:
	randomAI(string color, Board *board) {
		this->color = color;
		this->board = board;
	}

	pair<Coordinate, Coordinate> makeJump();
	pair<Coordinate, Coordinate> makeMove();
};

///////////////////////////////////
/*BEGIN RANDOMAI MEMBER FUNCTIONS*/
///////////////////////////////////

pair<Coordinate, Coordinate> randomAI::makeJump() {
	pair<Coordinate, Coordinate> targets;
	
	if (jumps.size() == 0) {
		targets = makeMove();
		return targets;
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
					targets.first = origin;
					targets.second = destination;
				}
			}
		}
	}

	return targets;
}

pair<Coordinate, Coordinate> randomAI::makeMove() {
	pair<Coordinate, Coordinate> targets;

	if (jumps.size() != 0) {
		targets = makeJump();
		return targets;
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
					targets.first = origin;
					targets.second = destination;
				}
			}
		}
	}

	return targets;
}


#endif
