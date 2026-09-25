#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Cars_BST.h"
#include "GenFuncs.h"
#include "LineOfCars.h"
#include "Menu.h"
#include "Ports_List.h"
#include "PrintFuncs.h"
#include "Station_BST.h"
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"

// Functions that handle all 13 main menu options and system exit

// Option 1 - Find and show the closest station to user
int locNearSt(StationsADT* stationsManager);

// Option 2 - Charge a car (connect or queue)
int ChargeCar(StationsADT* stations, CarsADT* cars);

// Option 3 - shows car status
int checkCarStatus(StationsADT* stations, CarsADT* cars);

// Option 4 - Stop charging and calculate payment
int stopCharge(StationsADT* stations, CarsADT* cars);

// Option 5 - Print all stations
void dispAllSt(StationsADT* stationsManager);

// Option 6 - Show all cars charging or waiting in queue at station
int dispCarsAtSt(StationsADT* stationsManager);

// Option 7 - Show stats for each station
int reportStStat(StationsADT* stationsManager);

// Option 8 - Print top 5 paying customers
void dispTopCustomers(CarsADT* cars);

// Option 9 - Add a new port to a station
int addNewPort(StationsADT* stationsManager);

// Option 10 - Release ports (and assign next in queue)
int releasePorts(StationsADT* stationsManager);

// Option 11 - Remove port that’s out of order
int remOutOrderPort(StationsADT* stationsManager);

// Option 12 - Delete car if not charging
int remCustomer(CarsADT* cars);

// Option 13 - Close station and free its data
int closeSt(StationsADT* stationsManager);

// Exit - Save everything and clean up memory
void exitSys(StationsADT* stations, CarsADT* cars);
