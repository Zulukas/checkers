#include <pigpio.h>
#include <iostream>
using namespace std;

int main() {
	if (!gpioInitialise())
		return 0;

	gpioSetMode(23, PI_OUTPUT);
	gpioSetMode(24, PI_OUTPUT);
	gpioWrite(24, 1);
	
	cout << "Testing the electromagnet...\n\n";
	
	bool on = false;
	
	while (1) {
		cout << "Enter a letter: (q to quit): ";
		char letter;
		cin >> letter;
		
		if (letter == 'q')
			break;
			
		on = !on;
		
		gpioWrite(23, on);
	}
}	
