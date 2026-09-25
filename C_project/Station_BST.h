#pragma once
#include <stdlib.h>
#include <string.h>
#include "Consts.h"
#include "data_structures.h"
#include "LineOfCars.h"
#include "Ports_List.h"
#include "SystemCoreFuncs.h"

// ===== Station Tree Management (BST) =====

// create empty station manager (with root = NULL)
StationsADT* createStationsManager();

// create new station node (already filled with data)
Station* createStationNode(Station* newStation);

// insert station to BST (by ID)
void insertStation(StationsADT* manager, Station* newStation);

// recursive helper for insert
Station* insertStationRec(Station* root, Station* newStation);

// search station using ID string (parsed to int)
Station* searchStationByIdStr(StationsADT* manager, const char* input);

// recursive search by ID
Station* searchStationIDRec(Station* root, int id);

// recursive search by location (for nearest station)
void findNearestStation(Station* node, double userX, double userY, Station** nearestStation, double* minDistance);

// wrapper for nearest station search
Station* searchStationByDistance(StationsADT* manager, double x, double y);

// search by station name (exact match)
Station* searchStationByName(StationsADT* manager, const char* name);

// recursive helper for name search
Station* searchStationByNameRec(Station* root, const char* name);

// print all stations in-order (by ID)
void inOrderStations(StationsADT* manager);

// recursive in-order print
void inOrderStationsRec(Station* root);

// free all memory of station tree
void freeStationTree(StationsADT* manager);

// recursive helper to free
void freeStationTreeRec(Station* root);

// find station that owns a given port pointer
Station* findStationOfPortRec(Station* root, Port* pPort);

// remove station from BST by ID
Station* removeStationFromBST(Station* root, int id);
