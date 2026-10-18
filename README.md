# Airport Search Engine

A C++ console application for searching, analysing and managing a dataset of airports. It loads airport records from a CSV file and lets you search them, sort them, calculate distances between airports, add new entries and save your changes, all through a text menu.

Built as a university coursework project.

## Features

The program opens with the **Nottingham SkyRoute Explorer** menu:

```
====== Nottingham SkyRoute Explorer ======
1. Search airports by country
2. Search airports by name
3. Search by IATA code
4. Count airports by type
5. Add a new airport
6. Advanced options
7. Save changes
8. Exit
```

**Searching**
- Search by country or continent, by airport name or city, or by IATA code
- Clear "no airports found" feedback when a search has no matches

**Analysis and data management**
- Count airports by type (for example, how many of each category are in the dataset)
- Add a new airport by entering its details, which updates both the in-memory list and the CSV file
- Save the current list to a CSV file of your choice (defaults to `airports.csv`)

**Advanced options**

| Option | What it does |
|--------|--------------|
| Data analysis | Find the airports with the highest and lowest elevation |
| Sorting | Sort the airport list by name, country, type, elevation or municipality |
| Distance calculation | Calculate the distance between two airports using the Haversine formula |
| Missing IATA codes | List airports that have no IATA code |
| Nearest airport search | Find the airport closest to a given airport |
| Group by municipality | Group airports by the municipality they serve |

Invalid menu input is handled without crashing, and the program pauses after each action so results stay readable.

## Getting started

**Requirements:** Windows with Visual Studio (a recent version that supports `.slnx` solution files) and the "Desktop development with C++" workload.

1. Clone the repository:
   ```bash
   git clone https://github.com/WM442/AirportSearchEngine.git
   ```
2. Open `AirportSearchEngineCoursework.slnx` in Visual Studio.
3. Make sure the data file is named `airports.csv` and sits in the same folder as the source files. The program loads this exact filename on start-up.
4. Build and run (`Ctrl + F5`).

## How it works

The code is split into three parts so that the menu and the data logic stay separate:

- **`Main.cpp`** creates the menu and the airport data, loads the CSV, then runs the main loop until the user chooses Exit. It also handles invalid input so a wrong key doesn't break the program.
- **`menuSystem.h / menuSystem.cpp`** defines the `MenuSystem` class. It prints the menus, reads the user's choices and displays results, but contains no search or sorting logic itself.
- **`AirportInfo.h / AirportInfo.cpp`** defines the `Airport` struct (one record, holding fields such as identifier, type, name, elevation, country, municipality, IATA code and coordinates) and the `AirportInfo` class, which holds the full list of airports in a `vector` and provides every operation: loading and saving the CSV, searching, counting by type with an `unordered_map`, sorting, and the distance and nearest-airport calculations.

Keeping the interface in one class and the data operations in another meant I could change how a feature works without rewriting the menu around it.

## Project structure

```
├── Main.cpp                                      # Program entry point and main loop
├── menuSystem.h / menuSystem.cpp                 # Menu display and user input
├── AirportInfo.h / AirportInfo.cpp               # Airport data and all operations
├── airports.csv                                  # Airport dataset
├── AirportSearchEngineCoursework.slnx            # Visual Studio solution
├── AirportSearchEngineCoursework.vcxproj         # Visual Studio project
└── README.md
```

## Possible improvements

- Validate input in the Advanced options sub-menus, which currently read the choice without checking for non-numeric input
- Store elevation and coordinates as numbers when loading the file, instead of keeping them as text
- Reduce the repeated code between the three search options by sharing one display function
- Let the user choose which CSV file to load instead of using a fixed filename
- Add automated tests for the search, sorting and distance functions
