#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <limits>
#include <cmath>
#include <iomanip>
#define pi 3.14159265358979323846
#include "AirportInfo.h"
using namespace std;

//Trims whitespace from start and end of a string
static vector<string> parseCSVLine(const string& line) {
	vector<string> result;
	string field;
	bool inQuotes = false;

	//Loop which iterates through each character
	for (size_t i = 0; i < line.size(); ++i) {
		char c = line[i];
		if (inQuotes) {
			if (c == '"') {

				//Checks for escaped double-quote
				if (i + 1 < line.size() && line[i + 1] == '"') {
					field += '"';

					//Skips the second quote
					++i; 
				}
				else {
					//Closing quote
					inQuotes = false;
				}
			}
			else {
				//Regular character inside quotes
				field += c;
			}
		}
		else {
			if (c == '"') {
				//Opening quote
				inQuotes = true;
			}
			//If a comma is found = end of field
			else if (c == ',') {
				result.push_back(field);
				field.clear();
			}
			//Regular character outside quotes
			else {
				field += c;
			}
		}
	}

	//Pushes last field
	result.push_back(field); 
	return result;
}

//Parses a coordinate string like "lat,lon"
static bool parseLatLon(const string &s, double &lat, double &lon) {

	//Simple parser which removes surrounding quotes, split on first comma, trim and stod
	string t = s;

	//Removes surrounding quotes if they exist
	if (t.size() >= 2 && t.front() == '"' && t.back() == '"') t = t.substr(1, t.size()-2);

	//Finds the position of the first comma to split latitude and longitude
	auto pos = t.find(',');

	//If there is no comma, the format is invalid
	if (pos == string::npos) return false;

	//Splits the string into latitude and longitude parts
	string a = t.substr(0, pos);
	string b = t.substr(pos + 1);

	//Lambda function to trim whitespace from the start and end of a string
	auto trim = [](string &x) {
		size_t i = 0, j = x.size();
		while (i < j && isspace((unsigned char)x[i])) ++i;
		while (j > i && isspace((unsigned char)x[j-1])) --j;
		x = x.substr(i, j - i);
	};
	trim(a); trim(b);

	//If either part is empty after trimming, the format is invalid
	if (a.empty() || b.empty()) return false;
	try {
		lat = stod(a);
		lon = stod(b);
		return true;
	} catch (...) {
		return false;
	}
}

//Haversine formula to calculate distance between two latitude and longitude points in kilometers
static long double haversineKm(double lat1, double lon1, double lat2, double lon2) {

	// Converts degrees to radians
	auto degtoRad = [](double deg){ return deg * (pi / 180.0); };

	//Calculate the differences in latitude and longitude in radians
	long double dLat = degtoRad(lat2 - lat1);
	long double dLon = degtoRad(lon2 - lon1);
	long double rlat1 = degtoRad(lat1);
	long double rlat2 = degtoRad(lat2);

	//Apply the Haversine formula
	long double a = sin(dLat/2.0) * sin(dLat/2.0) + cos(rlat1) * cos(rlat2) * sin(dLon/2.0) * sin(dLon/2.0);
	long double c = 2.0 * asin(sqrt(a));

	//Multiply by Earth's radius in kilometers to get the distance
	return 6371.0 * c;
}

static const Airport* findAirportByIdent(const vector<Airport> &airports, const string &ident) {
	for (const auto &a : airports) if (!a.ident.empty() && a.ident == ident) return &a;
	return nullptr;
}

//Reads the airports.csv file and outputs the information for each airport in a readable format
void AirportInfo::loadAirportInfo(const string& path) {
    ifstream file(path);

	//Error handling for file opening, with additional diagnostics to help identify the issue
    if (!file.is_open()) {
		cout << "Error: could not open file '" << path << "'.\n";
		try { cout << "Current working directory: " << filesystem::current_path() << endl; } catch(...) {}
		try { cout << "Attempted path: " << (filesystem::current_path() / path) << endl; } catch(...) {}
		return;
	}

	//Path for append of airports
	csvPath = path;

	//Read the file line by line, splitting each line into columns and outputting the relevant information
	string line, word, temp;
	getline(file, line);

	//Check if the header line has the expected number of columns, and output a warning if not
	while (getline(file, line)) {
		if (line.empty()) continue;

		//Creates vector for rows and splits each line by commas
		vector<string> row = parseCSVLine(line);

		//Error handling if the size of rows exceeds the amount stored
		if (row.size() <= 12) {
			cout << "Warning: unexpected column count (" << row.size() << ") for line: " << line << endl;
			continue;
		}

		//Instance of class Airport which stores information in each row
		Airport a;
		a.ident = row[0];
		a.type = row[1];
		a.name = row[2];
		a.elevation_ft = row[3];
		a.continent = row[4];
		a.iso_country = row[5];
		a.iso_region = row[6];
		a.municipality = row[7];
		a.icao_code = row[8];
		a.iata_code = row[9];
		a.gps_code = row[10];
		a.local_code = row[11];
		a.coordinates = row[12];

     //Try to parse coordinates into cached numeric fields to avoid repeated stod calls later
		double lat=0, lon=0;
		if (parseLatLon(a.coordinates, lat, lon)) { a.lat = lat; a.lon = lon; }
		//Push all information from rows into instance of class 'a'
		airports.push_back(move(a));
	
	}
	//Close the file after reading
	file.close();
    cout << "Loaded " << airports.size() << " airport records." << endl;
}

//Function used to output all information stored about airports
void AirportInfo::displayAirportInfo() {
	for (const auto& a : airports) {
		cout << "ident: " << a.ident << ", type: " << a.type << ", name: " << a.name << ", elevation_ft: " << a.elevation_ft
			<< ", continent: " << a.continent << ", iso_country: " << a.iso_country
			<< ", iso_region: " << a.iso_region << ", municipality: " << a.municipality
		    << ", iata_code: " << a.iata_code << ", gps_code: " << a.gps_code 
			<< ", local_code: " << a.local_code 
			<< ", coordinates: " << a.coordinates << endl;
	}
}

//Function used to search for airport information in CSV file
vector<Airport> AirportInfo::searchAirportInfo(int category, const string& query) const {

	//Create vector called 'out' to store output of search
	vector<Airport> out;
	string q = query;
	
	for (const auto& a : airports) {
		//If user selects 1 from the menu...
		if (category == 1) {
			//Uses find() and compares to npos to check if there is a match and pushes results
			if (a.iso_country.find(q) != string::npos || a.continent.find(q) != string::npos)
				out.push_back(a);
		}
		//If user selects 2 from the menu...
		if (category == 2) {
			//Uses find() and compares to npos to check if there is a match and pushes results
			if (a.name.find(q) != string::npos || a.municipality.find(q) != string::npos)
				out.push_back(a);
		}
		//If user selects 3 from the menu...
		if (category == 3) {
			//Uses find() and compares to npos to check if there is a match and pushes results
			if (a.iata_code.find(q) != string::npos)
				out.push_back(a);
		}
	}
	return out;
}

//Function to count the different types of airports
unordered_map<string, int> AirportInfo::countByType() const {

	//Using unordered_map to map each type to a variable and counts for each type
	unordered_map<string, int> counter;

	//For each airport,  1 is incremented
	for (const auto& a : airports) {
		counter[a.type]++;
	}
	return counter;
}

//Function to add a new airport to memory
void AirportInfo::addAirport(const Airport& a) {
	airports.push_back(a);
}
//Function to save current airports back to CSV
bool AirportInfo::saveToCsv(const string& path) const {
	ofstream ofs(path);
	if (!ofs.is_open()) return false;
	
	// write a simple header matching expected columns
	ofs << "ident,type,name,elevation_ft,continent,iso_country,iso_region,municipality,icao_code,iata_code,gps_code,local_code,coordinates" << '\n';
	
	for (const auto& a : airports) {
		// Simple CSV escaping for quotes and commas
		auto esc = [](const string& s) {
			if (s.find_first_of(",\"") == string::npos && s.find_first_of("\n\r") == string::npos) return s;
			string out = "\"";
			// Escape quotes by doubling them
			for (char c : s) {
				if (c == '"') out += "\"\"";
				else out.push_back(c);
			}
			// Close the field with a quote if it contains special characters
			out += "\"";
			return out;
			};
		// Writes each airport's information to the CSV file
		ofs << esc(a.ident) << ',' << esc(a.type) << ',' << esc(a.name) << ',' << esc(a.elevation_ft) << ',' << esc(a.continent) << ','
			<< esc(a.iso_country) << ',' << esc(a.iso_region) << ',' << esc(a.municipality) << ',' << esc(a.icao_code) << ',' << esc(a.iata_code) << ','
			<< esc(a.gps_code) << ',' << esc(a.local_code) << ',' << esc(a.coordinates) << '\n';
	}
	//Close the file after writin
	ofs.close();
	return true;
}

//Counts total number of airports in the CSV file
int AirportInfo::count() const {
	return static_cast<int>(airports.size());
}

bool AirportInfo::addToCSV(const Airport& a) const {
	//Checks if the csvPath is empty, if so then uses airports.csv as default
	string path = csvPath.empty() ? string("airports.csv") : csvPath;

	//Opens csv file for appending
	ofstream ofs(path, ios::app | ios::out);

	//Error handling if file does not open
	if (!ofs.is_open()) {
		cout << "addToCSV: failed to open file for append: " << path << endl;
		return false;
	}

	//Formats new information to format of airports.csv file
	auto esc = [](const string& s) {
		if (s.find_first_of(",\"") == string::npos && s.find_first_of("\n\r") == string::npos) return s;
		string out = "\"";
		for (char c : s) {
			//Escapes quotes by doubling them
			if (c == '"') out += "\"\"";
			else out.push_back(c);
		}
		//Closes the field with a quote if it contains special characters
		out += "\"";
		return out;
		};

	//Writes the new airport information to the CSV file
	ofs << esc(a.ident) << ',' << esc(a.type) << ',' << esc(a.name) << ',' << esc(a.elevation_ft) << ',' << esc(a.continent) << ','
		<< esc(a.iso_country) << ',' << esc(a.iso_region) << ',' << esc(a.municipality) << ',' << esc(a.icao_code) << ',' << esc(a.iata_code) << ','
		<< esc(a.gps_code) << ',' << esc(a.local_code) << ',' << esc(a.coordinates) << '\n';
	ofs.close();

	//Error handling for improper write of data
	if (ofs.fail()) {
		cout << "addToCSV: write failed for file: " << path << endl;
		return false;
	}
	//Returns true if new information is added to CSV
	return true;
}

//Function to sort airports by name
void AirportInfo::sortByName() {
	sort(airports.begin(), airports.end(), [](const Airport& a, const Airport& b) {
		return a.name < b.name;
		});
}

//Function to sort airports by country
void AirportInfo::sortByCountry() {
	sort(airports.begin(), airports.end(), [](const Airport& a, const Airport& b) {
		return a.iso_country < b.iso_country;
		});
}

//Function to sort airports by type
void AirportInfo::sortByType() {
	sort(airports.begin(), airports.end(), [](const Airport& a, const Airport& b) {
		return a.type < b.type;
		});
}

//Function to sort airports by elevation
void AirportInfo::sortByElevation() {
	sort(airports.begin(), airports.end(), [](const Airport& a, const Airport& b) {
		return a.elevation_ft < b.elevation_ft;
		});
}

//Function to sort airports by municipality
void AirportInfo::sortByMunicipality() {
	sort(airports.begin(), airports.end(), [](const Airport& a, const Airport& b) {
		return a.municipality < b.municipality;
		});
}

//Function to find the airport with the highest elevation
int AirportInfo::highestElevation() {

	//Best set to lowest value to start search
    int high = numeric_limits<int>::min();
	for (const auto& a : airports) {
		if (a.elevation_ft.empty()) continue;
		try {
			//Converts elevation from string to int
			int value = stoi(a.elevation_ft);
			if (value > high) high = value;
		}
		//Continues search if there is an error
		catch (...) { }
	}
	//Returns 0 if no valid elevations are found
	if (high == numeric_limits<int>::min()) return 0;
	return high;
}

//Function to find the airport with the lowest elevation
int AirportInfo::lowestElevation() {

	//Low set to highest value to start search
	int low = numeric_limits<int>::max();
	for (const auto& a : airports) {
		if (a.elevation_ft.empty()) continue;
		try {
			//Converts elevation from string to int
			int value = stoi(a.elevation_ft);
			if (value < low) low = value;
		}
		//Continues search if there is an error
		catch (...) { }
	}
	//Returns 0 if no valid elevations are found
	if (low == numeric_limits<int>::max()) return 0;
	return low;
}

//Function using harversine formula to calculate distance between two airports based on their ident codes
int AirportInfo::haversineFunction() {
	string firstAirport, secondAirport;

	//Prompts user for ident codes of the two airports to calculate distance between
	cout << "What is the ident code of the first airport? ";
	if (!(cin >> firstAirport)) { cout << "Error reading first airport input\n"; return 0; }

	cout << "What is the ident code of the second airport? ";
	if (!(cin >> secondAirport)) { cout << "Error reading second airport input\n"; return 0; }

	//Finds the airports in the loaded data based on the provided ident codes
	const Airport* A = findAirportByIdent(airports, firstAirport);
	const Airport* B = findAirportByIdent(airports, secondAirport);

	//Error handling if either airport is not found
	if (!A) { cout << "First airport not found: " << firstAirport << '\n'; return 0; }
	if (!B) { cout << "Second airport not found: " << secondAirport << '\n'; return 0; }

	//Parses the coordinates of the two airports, which are expected to be in the format "lat,lon"
	cout << "Found airports. Parsing coordinates...\n";
	double lat1, lon1, lat2, lon2;
	if (!parseLatLon(A->coordinates, lat1, lon1)) { cout << "Failed to parse coordinates for " << A->ident << " ('" << A->coordinates << "')\n"; return 0; }
	if (!parseLatLon(B->coordinates, lat2, lon2)) { cout << "Failed to parse coordinates for " << B->ident << " ('" << B->coordinates << "')\n"; return 0; }

	cout << "Calculating distance...\n";

	//Calculates the distance between the two airports using the Haversine formula
	long double dist = haversineKm(lat1, lon1, lat2, lon2);
	cout << "Distance " << A->ident << " -> " << B->ident << ": " << dist << " km\n";
}

//Function to find airports with missing IATA codes
void AirportInfo::missingIATAcode() {
	for (const auto& a : airports) {
		if (a.iata_code.empty()) {
			cout << "Airport with missing IATA code: " << a.name << " (" << a.ident << ")\n";
		}
	}
}

//Function to find the nearest airport to a given airport based on ident code
void AirportInfo::nearestAirport() {
    string id;

	cout << "Enter the ident code of the airport to find the nearest airport to: ";
	if (!(cin >> id)) { cout << "Error reading airport input\n"; return; }

	//Find the airport in the loaded data based on the provided ident code
	const Airport* A = findAirportByIdent(airports, id);
	if (!A) { cout << "Airport not found: " << id << '\n'; return; }

	//Ensure cached numeric coords are available for A
	double lat1 = A->lat, lon1 = A->lon;
	if (isnan(lat1) || isnan(lon1)) {
		if (!parseLatLon(A->coordinates, lat1, lon1)) { cout << "Failed to parse coordinates for " << A->ident << " ('" << A->coordinates << "')\n"; return; }
	}

	//Convert to radians once
	const long double R = 6371.0L;
	auto toRad = [](long double deg){ return deg * (static_cast<long double>(pi) / 180.0L); };
	long double rlat1 = toRad(lat1);
	long double rlon1 = toRad(lon1);

	//Initialize variables to track the nearest airport and the best distance found
	const Airport* nearest = nullptr;
	long double bestDist = numeric_limits<long double>::max();

	//Use equirectangular approximation to find nearest airport
	for (const auto &a : airports) {
		if (&a == A) continue;
		double lat2 = a.lat, lon2 = a.lon;
		if (isnan(lat2) || isnan(lon2)) {
			if (!parseLatLon(a.coordinates, lat2, lon2)) continue;
		}
		long double rlat2 = toRad(lat2);
		long double rlon2 = toRad(lon2);
		long double x = (rlon2 - rlon1) * cos((rlat1 + rlat2) / 2.0L);
		long double y = (rlat2 - rlat1);
		long double d = R * sqrt(x*x + y*y);
		if (d < bestDist) { bestDist = d; nearest = &a; }
	}

	if (!nearest) { cout << "No nearest airport found for " << A->ident << '\n'; return; }

	//Find distance to nearest airport using harversine function
	double nlat = nearest->lat, nlon = nearest->lon;
	if (isnan(nlat) || isnan(nlon)) {
		if (!parseLatLon(nearest->coordinates, nlat, nlon)) { cout << "Failed to parse coordinates for nearest airport\n"; return; }

	}
	//Calculates the distance between the two airports using the Haversine formula
	long double accurate = haversineKm(lat1, lon1, nlat, nlon);
	cout << "Nearest airport to " << A->ident << " is " << nearest->ident << " (" << nearest->name << ") at a distance of " << fixed << setprecision(2) << accurate << " km\n";
}

//Function to group airports by municipality
void AirportInfo::groupByMunicipality() {
    unordered_map<string, vector<Airport>> groups;
	string q;
	cout << "Enter municipality to group by (or leave empty to group all): ";

	//Reads whole line and allows empty input and skips any leftover whitespace
	getline(cin >> ws, q);

	//Group airports by municipality, filtering by a query string
	for (const auto& a : airports) {
		if (q.empty() || a.municipality.find(q) != string::npos) {
			groups[a.municipality].push_back(a);
		}
	}

	//Output the groups and the count of airports in each municipality
	for (const auto& entry : groups) {
		
		//Assigns municipality name and group of airports to variables for easier access
		const string &municipality = entry.first;
		const auto &group = entry.second;

		//Outputs municipality name with error handling alongside the count of airports
		cout << "Municipality: " << (municipality.empty() ? "(unknown)" : municipality)
			 << " has " << group.size() << " airports." << '\n';
	}
}