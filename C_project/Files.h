#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cars_BST.h"
#include "LineOfCars.h"
#include "Ports_List.h"
#include "Station_BST.h"
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"


// === Load files funcs ===

// grabs stations info and builds the BST
bool loadStations(const char* filename, StationsADT* stationsManager);

// pulls car info and drops them into tree
bool loadCars(const char* filename, CarsADT* carsManager);

// adds queued cars to matching stations
bool loadLineOfCars(const char* filename, StationsADT* stationsManager, CarsADT* carsManager);

// loads port data and links ports to cars if connected
bool loadPorts(const char* filename, StationsADT* stationsManager, CarsADT* carsManager);


// === Update files funcs ===

// dumps all stations to file
void writeStationsToFile(StationsADT* manager, const char* filename);

// saves one station (used by rec helper)
void writeSingleStation(FILE* fp, Station* st);

// walks station tree and writes each to file
void writeStationsRec(FILE* fp, Station* root);

// saves car tree to file
void writeCarsToFile(CarsADT* carsManager, const char* filename);

// helper to walk car tree and write
void writeCarsRec(FILE* fp, tCar* root);

// writes car details to one line
void writeSingleCar(FILE* fp, Car* car);

// tiny util for enum ? string
const char* portTypeToString(portType type);

// saves all ports (per station)
void writePortsToFile(StationsADT* stationsManager, const char* filename);

// walks through station tree and saves ports
void writePortsRec(FILE* fp, Station* root);

// loops through ports in one station and saves them
void writeStationPorts(FILE* fp, Station* station);

// single port output line
void writeSinglePort(FILE* fp, int stationId, Port* port);

// saves all station queues (cars waiting)
void writeLineOfCarsToFile(StationsADT* stationsManager, const char* filename);

// helper to loop through station tree and save queues
void writeLineOfCarsRec(FILE* fp, Station* root);

// writes queue of one station
void writeStationQueue(FILE* fp, Station* station);
