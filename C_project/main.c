#include <stdio.h>
#include "data_structures.h"
#include "Consts.h"
#include "Station_BST.h"
#include "Cars_BST.h"
#include "Files.h"
#include "Menu.h"
#include "SystemCoreFuncs.h"

// ===== EasyCharge Main Entry Point =====
// initializes system, loads data, runs menu loop

int main()
{
    StationsADT* stationsADT = createStationsManager();
    CarsADT* carsADT = createCarsManager();

    // check memory
    if (!stationsADT || !carsADT)
    {
        printf("Fatal error: Could not initialize system.\n");
        free(stationsADT);
        free(carsADT);
        return 1;
    }

    // load all data from files
    if (!loadFiles(stationsADT, carsADT))
    {
        // Free partial state without calling exitSys, which saves files.
        freeStationTree(stationsADT);
        freeCarTree(carsADT);
        free(stationsADT);
        free(carsADT);
        return 1;
    }

    printf("=== Welcome to EasyCharge Main Menu ===\n\n");

    int choice;
    do {
        choice = getValidMenuChoice();          // get user selection
        userChoice(choice, stationsADT, carsADT); // process option
    } while (choice != 0);

    // exit and free memory
    exitSys(stationsADT, carsADT);
    return 0;
}
