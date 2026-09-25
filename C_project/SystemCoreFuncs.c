#include "SystemCoreFuncs.h"

// search car in BST by license
Car* searchCar(CarsADT* manager, const char* license)
{
    tCar* node = searchCarRec(manager->root, license);
    return node ? node->pCar : NULL;
}

// load data from all files and init both trees
bool loadFiles(StationsADT* stationsManager, CarsADT* carsManager)
{
    if (!loadStations(FILE_STATIONS, stationsManager) ||
        !loadCars(FILE_CARS, carsManager) ||
        !loadPorts(FILE_PORTS, stationsManager, carsManager) ||
        !loadLineOfCars(FILE_LINE_OF_CARS, stationsManager, carsManager))
    {
        printf("Startup failed: required data could not be loaded. No files were saved.\n");
        return false;
    }

    printf("System data loaded successfully.\n\n");
    return true;
}

// write system state back to files
void updateFiles(StationsADT* stationsManager, CarsADT* carsManager)
{
    writeStationsToFile(stationsManager, FILE_STATIONS);
    writePortsToFile(stationsManager, FILE_PORTS);
    writeCarsToFile(carsManager, FILE_CARS);
    writeLineOfCarsToFile(stationsManager, FILE_LINE_OF_CARS);
}

// unified search wrapper — picks function based on type
Station* searchStation(StationsADT* manager, const char* input, SearchType type, double x, double y)
{
    if (type == SEARCH_BY_COORDS)
    {
        SearchFuncCoords func = searchStationByDistance;
        return func(manager, x, y);
    }

    SearchFuncStr funcs[] = {
        searchStationByIdStr,    // SEARCH_BY_ID
        searchStationByName      // SEARCH_BY_NAME
    };

    return funcs[type](manager, input);
}

// link car to port and update time + status
void assignCar2port(Car* car, Port* port)
{
    if (!car || !port)
    {
        displayError(3);
        return;
    }

    car->pPort = port;
    port->p2car = car;
    port->status = OCCUPIED;

    Date now = getCurrentTime();
    port->tin = now;
    car->inqueue = false;
}

// show error message by code
void displayError(int code)
{
    switch (code)
    {
    case ERR_INVALID_OPTION:
        printf("Error: Invalid menu option selected.\n"); break;
    case ERR_INVALID_INPUT:
        printf("Error: Invalid input format.\n"); break;
    case ERR_NULL_POINTER:
        printf("Error: Null pointer encountered.\n"); break;
    case ERR_FILE_OPEN:
        printf("Error: Failed to open file.\n"); break;
    case ERR_FILE_FORMAT:
        printf("Error: Invalid file format.\n"); break;

    case ERR_MEMORY_ALLOC_GENERAL:
        printf("Error: Memory allocation failed.\n"); break;
    case ERR_MEMORY_ALLOC_STATION:
        printf("Error: Memory allocation failed for station.\n"); break;
    case ERR_MEMORY_ALLOC_CAR:
        printf("Error: Memory allocation failed for car.\n"); break;
    case ERR_MEMORY_ALLOC_PORT:
        printf("Error: Memory allocation failed for port.\n"); break;
    case ERR_MEMORY_ALLOC_PORTLIST:
        printf("Error: Memory allocation failed for port list.\n"); break;
    case ERR_MEMORY_ALLOC_STATION_NODE:
        printf("Error: Memory allocation failed for station node.\n"); break;
    case ERR_MEMORY_ALLOC_CAR_NODE:
        printf("Error: Memory allocation failed for car node.\n"); break;
    case ERR_MEMORY_ALLOC_REPLACE_CAR:
        printf("Error: Memory allocation failed during car replacement.\n"); break;
    case ERR_MEMORY_ALLOC_ENQUEUE:
        printf("Error: Memory allocation failed while enqueuing car.\n"); break;

    case ERR_STATION_NOT_FOUND:
        printf("Error: Station not found.\n"); break;
    case ERR_CAR_NOT_FOUND:
        printf("Error: Car not found.\n"); break;
    case ERR_NO_STATIONS_AVAILABLE:
        printf("Error: No stations available in the system.\n"); break;
    case ERR_NO_MATCHING_PORT_TYPE:
        printf("Error: This station does not support the required charging type\n"); break;
    case ERR_CAR_ALREADY_CHARGING:
        printf("Error: This car is already connected to a charging port.\n"); break;
    case ERR_NO_FREE_PORT:
        printf("No available FREE ports of the required type at this station.\n"); break;
    case ERR_REMOVE_ACTIVE_CAR:
        printf("Error: Cannot remove a car that is currently in queue or charging.\n"); break;

    default:
        printf("Error: Unknown error occurred (code %d).\n", code); break;
    }
}
