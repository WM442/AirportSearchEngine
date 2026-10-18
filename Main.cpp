
#include <iostream>
#include "menuSystem.h"
#include "AirportInfo.h"
using namespace std;

int main()
{
    //Instances of classes
	MenuSystem menu;
    AirportInfo airport;

	cout << "Loading airports..." << endl;

    //Functions to be ran
    airport.loadAirportInfo("airports.csv");

	int choice = 0;
	do {
		menu.startMenu();
		choice = menu.UserChoice(airport);

		if (!(cin)) {
			cout << "Invalid input. Please enter a number corresponding to the menu options." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Pause before showing menu again unless exiting
		else if (choice != 8) {
			cout << endl;
			cout << "Press Enter to continue..." << endl;
			//Clear any error state, remove any leftover characters on the current line, then get user input
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get();
		}
		else break;

		//Outputs all information in CSV file about airports
			//airport.displayAirportInfo();

	} while (choice != 8);
	return 0;
}

