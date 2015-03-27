#ifndef PIECE
#define PIECE

#include <string>
using std::string;

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

#endif
