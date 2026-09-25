#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Cars_BST.h"
#include "Files.h"
#include "GenFuncs.h"
#include "Menu.h"
#include "Menu_Options.h"
#include "Station_BST.h"
#include "Consts.h"
#include "data_structures.h"

// ===== System Core Functions =====
// Central logic for searching, file updates, and system helpers

// search car by license 
Car* searchCar(CarsADT* manager, const char* license);

// connect car to port (updates port + car + start time)
void assignCar2port(Car* car, Port* port);

// save all data to files (stations + cars)
void updateFiles(StationsADT* stationsManager, CarsADT* carsManager);

// function pointer for station searches by string
typedef Station* (*SearchFuncStr)(StationsADT*, const char*);

// function pointer for station searches by coordinates
typedef Station* (*SearchFuncCoords)(StationsADT*, double, double);

// unified search wrapper (ID, name, or distance)
Station* searchStation(StationsADT* manager, const char* input, SearchType type, double x, double y);

// load all data from files into memory
bool loadFiles(StationsADT* stationsManager, CarsADT* carsManager);

// print user-friendly error by code
void displayError(int code);
