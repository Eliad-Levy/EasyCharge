#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Consts.h"
#include "data_structures.h"
#include "Menu.h"
#include "SystemCoreFuncs.h"

// ====== Car Manager Functions ======

// Creates the car manager (BST wrapper)
CarsADT* createCarsManager();

// Allocates and sets up a new car node
tCar* createCarNode(Car* newCar);

// Adds a new car to the BST, ordered by license
void insertCar(CarsADT* manager, Car* newCar);

// Recursively inserts a car into the correct place in the tree
tCar* insertCarRec(tCar* root, Car* newCar);

// Starts in-order traversal of all cars
void inOrderCars(CarsADT* manager);

// Recursive in-order print helper
void inOrderCarsRec(tCar* root);

// Frees all memory used by car manager (BST)
void freeCarTree(CarsADT* manager);

// Recursively releases all car nodes and car memory
void freeCarTreeRec(tCar* root);

// Removes a car from the BST by license
void deleteCarByLicense(CarsADT* manager, const char* license);

// Recursively searches and deletes a car by license
tCar* deleteCarRec(tCar* root, const char* license);

// Initializes a new car struct, sets its data and inserts it
Car* addNewCarToSystem(CarsADT* cars, const char* license);

// Counts total number of cars stored in the BST
int countCarsInBST(tCar* root);

// recursive search for car in BST
tCar* searchCarRec(tCar* root, const char* license);