#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <random>
#include <ctime>
#include <cstdlib>
#include <pigpio.h>
#include "coordinate.h"
//#include "motor.h"
#include "board.h"
#include "randomai.h"
#include <utility>
using namespace std;

const int DELAY = 15000;
const int DELAYx20 = 300000;

struct Mover {
	short y;
	short x;
} mover;

struct gpioSet {
	int a1, a2, b1, b2;
} X, Y, SolMag;

//Condensed Controls for Automation
void doPlayerMove(Coordinate origin, Coordinate dest);
void doPlayerJump(Coordinate origin, Coordinate dest, Coordinate jumpee);

//ELECTROMANGET\SOLENOID CONTROLLER
void extendSolenoid();
void retractSolenoid();
void enableMagnet();
void disableMagnet();
void SolMagOff();

//MOTOR CONTROLLER
void moveToCoordinate(Coordinate input);
void moveOnecoordinate(char axis, bool forward);
void accountForGap(bool forward);
void stepX(int a, int b, int c, int d);
void stepY(int a, int b, int c, int d);
void stepForward(char motor);
void stepBackward(char motor);
void moveToZeroSeven();
void controlsOff();
bool setup();
void moveToOrigin();
void moveToDropOff();
void returnFromDropOff();

Coordinate interpret(string input) throw (const char*);
bool moveCommand(Coordinate origin, Coordinate destination, Board &board);
bool jumpCommand(Coordinate origin, Coordinate destination, Board &board);
void saveCommand(Board &board);
void loadCommand(Board &board);
void getParts(string parts[], string input);

//////////////////////////////////
/*BEGIN GAME DEPENDANT FUNCTIONS*/
//////////////////////////////////

void test() {
	cin.clear();
	cin.ignore();
	
	while (true) {
		string input;
		
		cout << "TEST MODE - Enter Command > ";
		getline(cin, input);

		if (input == "quit") {
			break;
		}

		try {
			moveToCoordinate(interpret(input));
			extendSolenoid();
			cout << "extending solenoid \n";
			gpioDelay(DELAYx20);
			enableMagnet();
			gpioDelay(DELAYx20);
			cout << "enabling magnet \n";
			gpioDelay(DELAYx20);
			retractSolenoid();
			cout << "retractding solenoid \n";
			gpioDelay(DELAYx20);
			extendSolenoid();
			cout << "extending solenoid \n";
			gpioDelay(DELAYx20);
			disableMagnet();
			cout << "disabling magnet \n";
			gpioDelay(DELAYx20);
			retractSolenoid();
			cout << "retracting solenoid \n";
		}
		catch (const char* ex) {
			cout << ex << endl;
		}
		
		controlsOff();
	}
}

void start() {
	string garbageInputThatWontEverBeUsed;
	cout << "Game is initializing!  Adjust the mover to the starting location if not so!\n";
	cout << "\n###Starting location is the 'X' to the right of (0,7).###\n";
	cout << "\nPress any key when ready (\"Where is the dang \"any\" key!?\"): ";
	cin >> garbageInputThatWontEverBeUsed;
	
	cout << "\nMoving to (0,7)...\n";
	moveToZeroSeven();
	controlsOff();
	mover.x = 7;
	mover.y = 0;
}

void playerVsAI() {
	/*INITIALIZE GAME LOGIC OBJECTS*/
	Board board;
	randomAI red("red", &board);

	/*BEGIN GAME LOOP*/
	while (1) {
		string player = board.whiteTurn ? "white" : "red";
		string input;

		if (!board.whiteTurn) {
			//system("pause");
			red.displayPossibilities();
			pair<Coordinate, Coordinate> targets = red.makeMove();

			int x = targets.first.x - targets.second.x;
			int y = targets.first.y - targets.second.y;

			if ((x == 1 || x == -1) && (y == 1 || y == -1))
				doPlayerMove(targets.first, targets.second);
			else if ((x == 2 || x == -2) && (y == 2 || y == -2)) {
				Coordinate jumpee = board.calculateJumpee(targets.first, targets.second);
				doPlayerJump(targets.first, targets.second, jumpee);
			}

			continue;
		}
		else { //IF white player's turn, display the board
			cout << endl;
			board.display();
			cout << endl;
		}

		/*Interpret User Input*/
		cout << player << " > ";
		getline(cin, input);

		string parts[3];
		getParts(parts, input);
		
		/*Execute command from user input*/
		try {
			if (parts[0] == "move") { //Move a piece
				Coordinate origin = interpret(parts[1]);
				Coordinate dest = interpret(parts[2]);
				//moveCommand(interpret(parts[1]), interpret(parts[2]), board);
				if (moveCommand(origin, dest, board))

				//Automated section
					doPlayerMove(origin, dest);
			}
			if (parts[0] == "jump") { //Jump a piece
				Coordinate origin = interpret(parts[1]);
				Coordinate dest = interpret(parts[2]);
				Coordinate jumpee = board.calculateJumpee(origin, dest);
				
				//jumpCommand(interpret(parts[1]), interpret(parts[2]), board);			
				if (jumpCommand(origin, dest, board))

				//Automated section
					doPlayerJump(origin, dest, jumpee);
			}
			if (parts[0] == "save") { //Save the board
				saveCommand(board);
			}
			if (parts[0] == "load") { //Load a board
				loadCommand(board);
			}
			if (parts[0] == "quit") { //Quit the game
				break;
			}
			if (parts[0] == "what") { //Testing purposes
				Coordinate check = interpret(parts[1]);
				red.displayPossibilityAtCoordinate(check);
			}
		}
		catch (const char* ex) { //Generic exception catcher
			cout << ex << endl;
		}	
		

		//if (red.gameIsOver())
		//	break;
	}
}

void playerVsPlayer() {
}

void AIVsAI() {
	/*INITIALIZE GAME LOGIC OBJECTS*/
	Board board;
	randomAI red("red", &board);
	randomAI white("white", &board);

	/*BEGIN GAME LOOP*/
	while (1) {
		string player = board.whiteTurn ? "white" : "red";
		cout << player << " turn...\n";
	
		if (board.whiteTurn) {
			
			pair<Coordinate, Coordinate> targets = white.makeMove();
			
			int x = targets.first.x - targets.second.x;
			int y = targets.first.y - targets.second.y;
			
			if ((x == 1 || x == -1) && (y == 1 || y == -1))
				doPlayerMove(targets.first, targets.second);
			else if ((x == 2 || x == -2) && (y == 2 || y ==  -2)) {
				Coordinate jumpee = board.calculateJumpee(targets.first, targets.second);
				doPlayerJump(targets.first, targets.second, jumpee);
			}
		}
		if (!board.whiteTurn) {
			//system("pause");
			red.displayPossibilities();
			pair<Coordinate, Coordinate> targets = red.makeMove();

			int x = targets.first.x - targets.second.x;
			int y = targets.first.y - targets.second.y;

			if ((x == 1 || x == -1) && (y == 1 || y == -1))
				doPlayerMove(targets.first, targets.second);
			else if ((x == 2 || x == -2) && (y == 2 || y == -2)) {
				Coordinate jumpee = board.calculateJumpee(targets.first, targets.second);
				doPlayerJump(targets.first, targets.second, jumpee);
			}
		}
		

		//if (red.gameIsOver())
		//	break;
	}
}

//int _tmain(int argc, _TCHAR* argv[])
int main()
{	
	/*INITALIZE GPIO*/
	if (gpioInitialise() < 0) 
		return 0;
	if (!setup())
		return 0;
	start();
		
	/*ENTER TEST MODE*/
	char testMode;
	cout << "Would you like to enter test mode? (Y\\N): ";
	cin >> testMode;
	
	if (toupper(testMode) == 'Y') {
		test();
		return 0;
	}
	
	int gameMode = 0;;
	
	//Select game mode
	cout << "Please enter the game mode...\n\t1. Human (red) vs AI (white)" 
		  << "\n\t2. AI(red) vs AI(white)\n\t3. Human vs Human\nEnter choice: ";
	cin >> gameMode;
	
	if (gameMode == 1) {
		playerVsAI();
	}
	if (gameMode == 2) {
		AIVsAI();
	}
	if (gameMode == 3) {
		playerVsPlayer();
	}
	else
		return 0;

	return 0;
}

void getParts(string parts[], string input) {
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
}

void saveCommand(Board &board) {
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

void loadCommand(Board &board) {
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

bool moveCommand(Coordinate origin, Coordinate destination, Board &board) {
	return board.move(origin, destination);
}

bool jumpCommand(Coordinate origin, Coordinate destination, Board &board) {
	return board.jump(origin, destination);
}

void moveToZeroSeven() {
	for (int i = 0; i < 8; i++) {
		stepForward('X');
	}
}

void stepX(int a, int b, int c, int d) {
	gpioWrite(X.a1, a);//
	gpioWrite(X.a2, b);//
	gpioWrite(X.b1, c);//
	gpioWrite(X.b2, d);//
}

void stepY(int a, int b, int c, int d) {
	gpioWrite(Y.a1, a);
	gpioWrite(Y.a2, b);
	gpioWrite(Y.b1, c);
	gpioWrite(Y.b2, d);
}

void stepForward(char motor) {
	if (motor == 'X') {
		cout << "stepping motor forward on the x axis...\n";
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
		cout << "stepping motor forward on the y axis...\n";
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
		cout << "stepping motor backward on the x axis...\n";
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
		cout << "stepping motor backward on the y axis...\n";
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

void accountForGap(bool forward) {
	const int gapSteps = 2;
	if (forward) {
		for (int i = 0; i < gapSteps; i++)
			stepForward('Y');
	}
	else {
		for (int i = 0; i < gapSteps; i++)
			stepBackward('Y');
	}
}

void moveOneCoordinate(char axis, bool forward) {
	const int steps = 13;
	
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
//	input.x
	
	const char* ERROR = "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
	
	cout << "MOVER LOCATION CURRENTLY AT: " << mover.x << ", " 
			<< mover.y << endl << endl;
	
	cout << "###ATTEMPTING TO MOVE TO " << input << endl << endl;
	
	if (input.x > 7 || input.x < -1 || input.y < 0 || input.y > 7) {
		cout << "Invalid input coordinate, destination is out of bounds\n";
		return;
	}
	
	//Get the distances to move....
	int x = input.x - mover.x;
	int y = input.y - mover.y;
	
	cout << "Moving " << x << " in the X direction and " << y <<
				" in the Y direction...\n\n";
	
	if ((x + mover.x > 7) || (x + mover.x < 0) || (y + mover.y > 7) || (y + mover.y < 0)) {
		cout << "Mover calibration necessary, please reset to origin\n";
		cout << "Press any key when calibrated to origin!\n";
		string something;
		cin >> something;
		mover.x = -1;
		mover.y = 0;
		
		x = input.x - mover.x;
		y = input.y - mover.y;
	}
	
	//HANDLE X MOVEMENT
	if (x < 0)
		for (int i = x; i < 0; i++) {
			cout << "x-- ";
			//stepBackward('X');
			moveOneCoordinate('X', true);
			mover.x--;
			
			if (mover.x == 0 && (i-1) > 0) {
				cout << ERROR;
				break;
			}
		}
	else 
		for (int i = 0; i < x; i++) {
			//stepForward('X');
			moveOneCoordinate('X', false);
			mover.x++;
			cout << "x++ ";
			
			if (mover.x == 7 && (i+1) < x) {
				cout << ERROR;
				break;
			}
		}
		
	//HANDLE Y MOVEMENT
	if (y < 0)
		for (int i = y; i < 0; i++) {
			//stepBackward('Y');
			moveOneCoordinate('Y', true);
			mover.y--;
			cout << "y-- " << mover.y;
			if (mover.y == 3) {
				accountForGap(true); //positive
				cout << "\nGap account - NEGATIVE\n";
			}
			
//			if (i == 3 && (i - 
			
			if ((y-1) > 0) {
				if (mover.y == 0) {
					cout << ERROR;
					break;
				}
				/*
				if (mover.y == 3) {
					accountForGap(false); //negative
					cout << "\nGap account - NEGATIVE\n";
				}*/
			}
		}
	else
		for (int i = 0; i < y; i++) {
			//stepForward('Y');
			moveOneCoordinate('Y', false);
			mover.y++;
			cout << "y++ " << mover.y;
			if (mover.y == 4) {
				cout << "\nGap account - Positive\n";
				accountForGap(false); //positive
			}
			
			if ((i+1) < y) {
				if (mover.y == 7 && (i+1) < y) {
					cout << ERROR;
					break;
				}
				/*
				if (mover.y == 3) {
					cout << "\nGap account - Positive\n";
					accountForGap(false/*true*//*); //positive
				}*/
			}
		}
		
	cout << "\n\nMOVER NOW AT " << mover.x << ", " << mover.y << endl;
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
	SolMag.a1 = 23;
	SolMag.a2 = 24;
	SolMag.b1 = 17;
	SolMag.b2 = 27;
	
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

		//Solenoid and Magnet Control
		gpioSetMode(SolMag.a1, PI_OUTPUT);
		gpioSetMode(SolMag.a2, PI_OUTPUT);
		gpioSetMode(SolMag.b1, PI_OUTPUT);
		gpioSetMode(SolMag.b2, PI_OUTPUT);
		
		cout << "MOTOR CONTROL ONLINE!  READY TO GO!\n";
		controlsOff();
		return true;
	}
	
	cout << "FAILURE TO INITIALIZE GPIO SET!  GAME IS ENDING\n";
	return false;
}

Coordinate interpret(string input) throw (const char*) {
	const char* invalidSize = "ERROR:  INPUT TOO LARGE!";
	const char* invalidX = "ERROR: X  INPUT MUST BE 'A'-'H'!";
	const char* invalidY = "ERROR: Y  INPUT MUST BE '1'-'8'!";
	const int swapCoord[] = {7, 6, 5, 4, 3, 2, 1, 0};

	cout << input << endl;

	if (input.size() != 2)
		throw invalidSize;
	if (input[0] < 'a' || input[0] > 'h')
		throw invalidX;
	if (input[1] < '1' || input[1] > '8')
		throw invalidY;

	Coordinate result((int)input[0] - 'a', swapCoord[input[1] - '1'])/*(int)input[1] - '1')*/;

	return result;
}

void extendSolenoid() {
	gpioWrite(SolMag.b2, 1);
}

void retractSolenoid() {
	gpioWrite(SolMag.b2, 0);
}

void enableMagnet() {
	gpioWrite(SolMag.a1, 1);
}

void disableMagnet() {
	gpioWrite(SolMag.a1, 0);
}

void SolMagOff() {
	gpioWrite(SolMag.a1, 0);
	gpioWrite(SolMag.a2, 0);
	gpioWrite(SolMag.b1, 0);
	gpioWrite(SolMag.b2, 0);
}


/*DESIGN NOTES
	1: Do not keep the solenoid, magnet, and motors active unless using!
		a. This is to prevent overheating and unnescessary power usage
	2: Pieces when being moved will need to be lifted to avoid bumping\pushing other pieces (confirm this)
	3: Look into possibilities for reduction of solenoid and electromagnet currents, we don't want to run the L298N hotter than it needs to be.  This is the most at-risk driver
*/

/*TODOs:
	1: See if the magnet can be moved safely while retracted
*/
void doPlayerJump(Coordinate origin, Coordinate dest, Coordinate jumpee) {
	//Move to the origin 
	moveToCoordinate(origin);
	gpioDelay(DELAYx20);
	extendSolenoid(); 
	gpioDelay(DELAYx20);
	enableMagnet(); //Pick up the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	gpioDelay(DELAYx20);
	
	//Move to the destination
	moveToCoordinate(dest);
	gpioDelay(DELAYx20);
	extendSolenoid();
	gpioDelay(DELAYx20);
	disableMagnet(); //Drop the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	gpioDelay(DELAYx20);

	//Move to the jumped piece
	moveToCoordinate(jumpee);
	gpioDelay(DELAYx20);
	extendSolenoid();
	gpioDelay(DELAYx20);
	enableMagnet(); //Pick up the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	gpioDelay(DELAYx20);
	
	//Move to the drop off spot
	moveToDropOff();
	gpioDelay(DELAYx20);
	extendSolenoid();
	gpioDelay(DELAYx20);
	disableMagnet(); //Drop the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	gpioDelay(DELAYx20);
	returnFromDropOff(); //Get back on the board now
	moveToCoordinate(jumpee);
	controlsOff();
}

void doPlayerMove(Coordinate origin, Coordinate dest) {
	//Move to origin
	moveToCoordinate(origin);
	gpioDelay(DELAYx20);
	extendSolenoid();
	gpioDelay(DELAYx20);
	enableMagnet(); //Pick up the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	gpioDelay(DELAYx20);
	
	//Move to the destination
	moveToCoordinate(dest);
	gpioDelay(DELAYx20);
	extendSolenoid();
	gpioDelay(DELAYx20);
	disableMagnet(); //drop the piece
	gpioDelay(DELAYx20);
	retractSolenoid();
	
	controlsOff();
}

void moveToOrigin() {
	Coordinate origin;
	origin.x = 7;
	origin.y = 0;

	moveToCoordinate(origin);

	for (int i = 0; i < 8; i++) {
		stepBackward('X');
	}
	controlsOff();
}

void moveToDropOff() {
	Coordinate dropoff;
	dropoff.x = 0;
	dropoff.y = 0;

	moveToCoordinate(dropoff);

	for (int i = 0; i < 13; i++) {
		stepForward('X');
	}
	controlsOff();
}

void returnFromDropOff() {
	for (int i = 0; i < 13; i++) {
		stepBackward('X');
	}
	controlsOff();
}
