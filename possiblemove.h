#ifndef POSSIBLEMOVE
#define POSSIBLEMOVE

#include "coordinate.h"
#include <list>
using std::list;

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

#endif
