#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

//Create a struct to store the information about the airport
struct Airport {
	string ident;
	string type;
	string name;
	string elevation_ft;
	string continent;
	string iso_country;
	string iso_region;
	string municipality;
	string icao_code;
	string gps_code;
	string iata_code;
	string local_code;
	string coordinates;
    // cached numeric coordinates (degrees). set to NaN when unknown
	double lat = std::numeric_limits<double>::quiet_NaN();
	double lon = std::numeric_limits<double>::quiet_NaN();
};


// Create an AirportInfo class which stores functions to display information about the airport
class AirportInfo {
public:
	//Load CSV contents 
	void loadAirportInfo(const string& path);

	//Display the information about the airport in a readable format
	void displayAirportInfo();

	//Search for airports based on a specific category
	vector<Airport> searchAirportInfo(int category, const string& query) const;

	//Count the different types of airports and return a map of type to count
	unordered_map<string, int> countByType() const;

	//Count total number of airports in the CSV file
	int count() const;

	//Add a new airport to memory
	void addAirport(const Airport& a);

	//Save current airports back to CSV
	bool saveToCsv(const string& path) const;

	//Add airport to CSV file
	bool addToCSV(const Airport& a) const;

	//Sort airports by name
	void sortByName();

	//Sort airports by country
	void sortByCountry();

	//Sort airports by type
	void sortByType();

	//Sort airports by elevation
	void sortByElevation();

	//Sort airports by municipality
	void sortByMunicipality();

	//Find the airport with the highest elevation
	int highestElevation();

	// Compute distance between two airports (haversine). Prompts for idents.
	int haversineFunction();

	//Find the airport with the lowest elevation
	int lowestElevation();

	//Find airports with missing IATA code
	void missingIATAcode();

	//function to search for the nearest airport
	void nearestAirport();

	//Group airports by municipality
	void groupByMunicipality();

private:
	//Private vector to store airports
	vector<Airport> airports;

	//Private string to store CSV file path
	string csvPath;
};