#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
using namespace std;

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

#endif
