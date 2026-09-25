#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Consts.h"
#include "data_structures.h"
#include "GenFuncs.h"
#include "SystemCoreFuncs.h"
#include "Menu_Options.h"
#include "Files.h"


// Basic menu stuff for showing options, reading input and reacting to it


// Shows the main menu
void Menu();

// Asks user to pick a number between 0-13, returns the valid choice
int getValidMenuChoice();

// Runs the action that matches the user choice
void userChoice(int choice, StationsADT* stationsManager, CarsADT* carsManager);

// Waits for user to press ENTER before continuing
void waitForEnter();

// Clears the screen so it's not messy
void clearScreen();



