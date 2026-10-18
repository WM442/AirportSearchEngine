#include <iostream>
#include "menuSystem.h"
#include <string>
#include <sstream>
#include <limits>
#include "AirportInfo.h"
#include <fstream>
using namespace std;

//Outputs starting menu and options for user
int MenuSystem::startMenu() {

	cout << "\n";
	cout << "\n";
	cout << "\n";
	cout << "====== Nottingham SkyRoute Explorer ======" << endl;
	cout << "1. Search airports by country" << endl;
	cout << "2. Search airports by name" << endl;
	cout << "3. Search by IATA code" << endl;
	cout << "4. Count airports by type" << endl;
	cout << "5. Add a new airport" << endl;
	cout << "6. Advanced options" << endl;
	cout << "7. Save changes" << endl;
	cout << "8. Exit" << endl;
	cout << "Please select an option: ";
	return 0;
}

//Stores user input and outputs corresponding message for chosen option
int MenuSystem::UserChoice(AirportInfo& airportInfo) {
	int MenuOption;
	if (!(cin >> MenuOption)) return -1;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	//If statements to select each possible user input
	if (MenuOption == 1) {
		cout << "You have selected option 1: Search airports by country or continent" << endl;
		cout << "Enter country or continent to search: ";

		//Uses q and user input for search boundaries and saves it to a variable res
		string q; getline(cin, q);
		auto res = airportInfo.searchAirportInfo(1, q);

		//CSV file handling to ensure user gets message when nothing is found
		if (res.empty()) {
			cout << "No airports found for query '" << q << "'." << endl;
		}

		//Outputs each airport and its information
		else {
			cout << res.size() << " result(s):\n";
			for (const auto& a : res) cout << a.name << " (" << a.iso_country << ") - " << a.coordinates << "\n";
		}
	}

	else if (MenuOption == 2) {
		cout << "You have selected option 2: Search airports by name" << endl;
		cout << "Enter name or city to search: ";

		//Uses q and user input for search boundaries and saves it to a variable res
		string q; getline(cin, q);
		auto res = airportInfo.searchAirportInfo(2, q);

		//CSV file handling to ensure user gets message when nothing is found
		if (res.empty()) {
			cout << "No airports found for query '" << q << "'." << endl;
		}

		//Outputs each airport and its information
		else {
			cout << res.size() << " result(s):" << endl;;
			for (const auto& a : res) cout << a.name << " - " << a.municipality << "(" << a.iata_code << ")" << " - " << a.coordinates << endl;
		}
	}

	else if (MenuOption == 3) {
		cout << "You have selected option 3: Search by IATA code" << endl;
		cout << "Enter IATA code to search: ";

		//Uses q and user input for search boundaries and saves it to a variable res
		string q; getline(cin, q);
		auto res = airportInfo.searchAirportInfo(3, q);

		//CSV file handling to ensure user gets message when nothing is found
		if (res.empty()) {
			cout << "No airports found for query '" << q << "'." << endl;
		}

		//Outputs each airport and its information
		else {
			cout << res.size() << " result(s):\n";
			for (const auto& a : res) cout << a.name << " - " << a.iata_code << "\n";
		}
	}

	else if (MenuOption == 4) {
		cout << "You have selected option 4: Count airports by type" << endl;

		//Uses function countByType and outputs a total counter for each type
		auto count = airportInfo.countByType();
		for (const auto& a : count) cout << a.first << ": " << a.second << "\n";
	}

	else if (MenuOption == 5) {
		cout << "You have selected option 5: Add a new airport" << endl;

		//Creates instance of class Airport
		Airport a;

		//Collection of user input to insert into CSV file
		cout << "Enter ident: ";
		getline(cin, a.ident);
		cout << "Enter type: ";
		getline(cin, a.type);
		cout << "Enter name: ";
		getline(cin, a.name);
		cout << "Enter elevation_ft: ";
		getline(cin, a.elevation_ft);
		cout << "Enter continent: ";
		getline(cin, a.continent);
		cout << "Enter iso_country: ";
		getline(cin, a.iso_country);
		cout << "Enter iso_region: ";
		getline(cin, a.iso_region);
		cout << "Enter municipality: ";
		getline(cin, a.municipality);
		cout << "Enter icao_code: ";
		getline(cin, a.icao_code);
		cout << "Enter gps_code: ";
		getline(cin, a.gps_code);
		cout << "Enter iata_code: ";
		getline(cin, a.iata_code);
		cout << "Enter local_code: ";
		getline(cin, a.local_code);
		cout << "Enter coordinates: ";
		getline(cin, a.coordinates);

		//Adds the new airport to memory
		airportInfo.addAirport(a);

		//Adds the new airport to the CSV file
		airportInfo.addToCSV(a);

		cout << "Airport added. Total airports now: " << airportInfo.count() << "\n";

	}

	//Sub menu for advanced options
	else if (MenuOption == 6) {

		//Declares variables for user input in sub menu
		int subMenuOption;
		int AnalysisOption;
		int SortOption;

		//Outputs options for user to select in sub menu
		cout << endl;
		cout << "You have selected option 6: Advanced options" << endl;
		cout << "1. Data analysis" << endl;
		cout << "2. Sorting" << endl;
		cout << "3. Distance calculation" << endl;
		cout << "4. Missing IATA codes" << endl;
		cout << "5. Nearest airport search" << endl;
		cout << "6. Group airports by municipality" << endl;
		cout << "Please select an option: ";
		cin >> subMenuOption;

		//User path if data analysis is chosen
		if (subMenuOption == 1) {

			cout << endl;
			cout << "You have selected data analysis" << endl;
			cout << "1. Find the airport with the highest elevation" << endl;
			cout << "2. Find the airport with the lowest elevation" << endl;
			cout << "Enter 1 or 2 to search: ";
			cin >> AnalysisOption;

			if (AnalysisOption == 1) {
				//Calls function to find the airport with the highest elevation
				int highest = airportInfo.highestElevation();
				cout << "The airport with the highest elevation is: " << highest << "ft";
				cout << endl;
			}

			else if (AnalysisOption == 2) {
				//Calls function to find the airport with the lowest elevation
				int lowest = airportInfo.lowestElevation();
				cout << "The airport with the lowest elevation is: " << lowest << "ft";
				cout << endl;
			}
			//User input validation for sub menu
			else {
				cout << "Invalid option. Returning to main menu." << endl;
			}
		}
		
		//User path if sorting is chosen
		else if (subMenuOption == 2) {

			cout << endl;
			cout << "You have selected sorting" << endl;
			cout << "1. Sort airports by name" << endl;
			cout << "2. Sort airports by country" << endl;
			cout << "3. Sort airports by type" << endl;
			cout << "4. Sort airports by elevation" << endl;
			cout << "5. Sort airports by municipiality" << endl;
			cout << "Enter a number to sort: ";
			cin >> SortOption;

			//User input validation for sub menu
			if (SortOption == 1) {
				//Calls function to sort airports by name
				airportInfo.sortByName();
				cout << "Airports sorted by name." << endl;
				cout << endl;
				
			}
			else if (SortOption == 2) {
				//Calls function to sort airports by country
				airportInfo.sortByCountry();
				cout << "Airports sorted by country." << endl;
				cout << endl;
			}
			else if (SortOption == 3) {
				//Calls function to sort airports by type
				airportInfo.sortByType();
				cout << "Airports sorted by type." << endl;
				cout << endl;
				
			}
			else if (SortOption == 4) {
				//Calls function to sort airports by elevation
				airportInfo.sortByElevation();
				cout << "Airports sorted by elevation." << endl;
				cout << endl;
				
			}
			else if (SortOption == 5) {
				//Calls function to sort airports by municipiality
				airportInfo.sortByMunicipality();
				cout << "Airports sorted by municipiality." << endl;
				cout << endl;
				
			}
			else {
				cout << "Invalid option. Returning to main menu." << endl;
				
			}
		}

		else if (subMenuOption == 3) {
			cout << endl;
			cout << "You have selected distance calculation" << endl;
			//Calls function to calculate the distance between two airports using the Haversine formula
			airportInfo.haversineFunction();
			cout << endl;

		}

		else if (subMenuOption == 4) {
			cout << endl;
			cout << "You have selected missing IATA codes" << endl;
			//Calls function to find airports with missing IATA codes
			airportInfo.missingIATAcode();
			cout << endl;
		}

		else if (subMenuOption == 5) {
			cout << endl;
			cout << "You have selected nearest airport search" << endl;
			//Calls function to find the nearest airport to a given airport
			airportInfo.nearestAirport();
			cout << endl;
		}

		else if (subMenuOption == 6) {
			cout << endl;
			cout << "You have selected group airports by municipality" << endl;
			//Calls function to group airports by municipality
			airportInfo.groupByMunicipality();
			cout << endl;
		}

		else {
			cout << "Invalid option. Returning to main menu." << endl;

		}
	}

	else if (MenuOption == 7) {
		cout << "You have selected option 7: Save changes" << endl;
		cout << "Enter filename to save to (default: airports.csv): ";

		//Creates a string of file name
		string fname;

		//Uses getline for user input if they want to save it to another file
		getline(cin, fname);

		//If user input is empty, it saves to the default file name
		if (fname.empty()) fname = "airports.csv";

		//Confirmation message once saved
		if (airportInfo.saveToCsv(fname)) {
			cout << "Saved " << airportInfo.count() << " airports to " << fname << "\n";
		}

		//Error handling message if file is not saved properly
		else {
			cout << "Failed to save to " << fname << "\n";
		}

	}

	else if (MenuOption == 8) {
		cout << "Exiting the program. Goodbye!" << endl;
	}

	//User input validation to ensure programs handles properly
	else {
		cout << "Invalid option. Please select a valid option from the menu." << endl;
	}
	return MenuOption;
}