#ifndef MOTOR
#define MOTOR

#include <pigpio.h>
#include "coordinate.h"

const int DELAY = 15000;

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
void moveToZeroSeven();
void controlsOff();
bool setup();

void moveToZeroSeven() {
	for (int i = 0; i < 17; i++) {
		stepForward('X');
	}
}

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
	const char* ERROR = "CRITICAL ERROR: MOVER ATTEMPTING TO MOVE OUT OF BOUNDS!\n";
	
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
		mover.x = -1;
		mover.y = 0;
		
		x = input.x - mover.x;
		y = input.y - mover.y;
	}
	
	if (x < 0)
		for (int i = x; i > 0; i--) {
			stepBackward('X');
			mover.x--;
			
			if (mover.x == 0 && (i-1) > 0) {
				cout << ERROR;
				break;
			}
		}
	else 
		for (int i = 0; i < x; i++) {
			stepForward('X');
			mover.x++;
			
			if (mover.x == 7 && (i+1) < x) {
				cout << ERROR;
				break;
			}
		}
		
	if (y < 0)
		for (int i = y; i > 0; i--) {
			stepBackward('Y');
			mover.y--;
			
			if ((y-1) > 0) {
				if (mover.y == 0) {
					cout << ERROR;
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
					cout << ERROR;
					break;
				}
				
				if (i == 3) {
					accountForGap(true); //positive
				}
			}
		}
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
}

#endif
