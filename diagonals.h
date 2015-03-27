#ifndef DIAGONALS
#define DIAGONALS

#include "coordinate.h"

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

#endif
