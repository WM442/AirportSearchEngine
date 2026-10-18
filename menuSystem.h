#pragma once
#include <string>
//Create a menu system class which stores functions related to the menu system
class AirportInfo;

//Create a MenuSystem class which initialises functions
class MenuSystem {
public:
	//Outputs starting menu and options for user
	int startMenu();

	//Stores user input and outputs corresponding message for chosen option
	int UserChoice(AirportInfo& airportInfo);
};