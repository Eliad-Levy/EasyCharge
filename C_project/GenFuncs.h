#pragma once
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "Cars_BST.h"
#include "Station_BST.h"
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"


// Functions used for helping with stations, cars and general system logic


// Gets (x,y) location from user
void getUserLocation(double* x, double* y);

// Counts ports by status and type
void countPortStats(Port* head, int* active, int* occupied, int* slow, int* mid, int* fast);

// Prompts user for valid license and stores in outLicense
int getValidLicenseFromUser(char* outLicense);

// Generic car input and creation if needed
Car* getCar(CarsADT* cars);

// Retrieves existing car by license input
Car* getExistingCarFromUser(CarsADT* cars);

// Prompts user for a valid station and returns it
Station* getValidStationFromUser(StationsADT* stations);

// Calculates and updates car's payment based on duration
void calculatePayment(Car* car);

// Disconnects car from a port without queue update
void disconnectOnly(Car* car, Port* port);

// Assigns the first matching car in queue to port
void advanceQueue(Port* port, Station* st);

// Prints usage percentage of station's ports
void StationUtilization(Station* st);

// Counts and displays faulty ports
void FaultyPorts(Station* st);

// Analyzes load level based on cars in queue vs charging
void LoadLevel(Station* st);

// Fills array with all cars in-order from BST
void fillCarArrayInorder(tCar* root, Car** arr, int* index);

// Comparator for sorting cars by total payment (descending)
int compareCarsByPayed(const void* a, const void* b);

// Returns 1 if station has at least one faulty port
int hasOutOfOrderPorts(Station* st);

// Prompts user to select a faulty port for removal
int getValidPortToRemove(Station* st);

// Calculates Euclidean distance between two points
double calcDistance(double x1, double y1, double x2, double y2);

// Returns 1 if string contains only digits
int isDigitsOnly(const char* str);

// Returns 1 if string contains only letters and spaces
int isLettersAndSpacesOnly(const char* str);

// Returns number of minutes between two Date values
int calcMinutesPassed(Date start, Date end);

// Gets the current system time as Date struct
Date getCurrentTime();
