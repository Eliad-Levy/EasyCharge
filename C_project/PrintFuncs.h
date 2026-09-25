#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "GenFuncs.h"
#include "LineOfCars.h"
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"

// ===== Display and Print utilities =====

// Nearest station info
void printNearest(Station* nearest, double minDist, int active, int slow, int mid, int fast, int occupied);

// Show all OUT_OF_ORDER ports in a station
void printOutOfOrderPorts(Station* st);

// Print cars in a queue
void printQueue(qCar queue);

// Print all stations recursively
void printStationsFull(Station* root);

// Print all cars in BST
void printCarsTree(tCar* root);

// Print ports in a station
void printPorts(Port* portList);

// Print full info of one station
void printSingleStation(Station* st);

// Show car charging details
void printCarDetails(Car* car);

// Show car charging status
void printChargingStatus(Car* car, StationsADT* stations);

// Show car's position in queue
void printQueueStatus(Car* car, StationsADT* stations);