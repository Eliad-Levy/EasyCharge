#include "Menu_Options.h"


// Option 1 - Find and show the closest station to user
// Gets user coordinates, finds the closest station, and prints its details
int locNearSt(StationsADT* stationsManager)
{
    double userX, userY;
    getUserLocation(&userX, &userY); // ask user for coordinates

    // User chose to go back
    if (userX == -9999 && userY == -9999)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    Station* nearest = searchStation(stationsManager, NULL, SEARCH_BY_COORDS, userX, userY);

    // no station found
    if (!nearest) {
        displayError(32);
        return 1;
    }

    double minDist = calcDistance(userX, userY, nearest->coord.x, nearest->coord.y);

    // count stats about the station's ports
    int active = 0, occupied = 0, slow = 0, mid = 0, fast = 0;
    countPortStats(nearest->portsList->head, &active, &occupied, &slow, &mid, &fast);

    // display nearest station info
    printNearest(nearest, minDist, active, slow, mid, fast, occupied);

    return 1;
}


// Option 2 - Connect car to port or add to queue
// Gets valid station and car, tries to assign to port or queue
int ChargeCar(StationsADT* stations, CarsADT* cars)
{
    Station* st = getValidStationFromUser(stations);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    printSingleStation(st); // show station details

    Car* car = getExistingCarFromUser(cars);
    if (!car)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    printCarDetails(car);

    // already connected to a port
    if (car->pPort != NULL)
    {
        displayError(34);
        return 1;
    }

    // Reject a new charging request while the car is already queued.
    if (car->inqueue)
    {
        printf("Car %s is already in the queue at station %d.\n",
            car->nLicense, car->stationId);
        return 1;
    }

    Port* port = NULL;
    PortSearchResult result = findAvailablePort(st->portsList, car->portType, &port);

    switch (result)
    {
    case PORT_FOUND:
        car->stationId = st->id;
        assignCar2port(car, port);
        printf("Car %s assigned to port #%d at station %s ", car->nLicense, port->num, st->name);
        printf("at: %02d/%02d/%04d %02d:%02d\n",
            car->pPort->tin.day,
            car->pPort->tin.month,
            car->pPort->tin.year,
            car->pPort->tin.hour,
            car->pPort->tin.minute);
        printf("\n\n");
        break;

    case PORTS_BUSY:
        displayError(35);

        // Add to station queue
        car->stationId = st->id;
        enqueueCar(&st->carQueue, car);
        car->inqueue = 1;
        printf("Car %s added to the queue at station %s.\n", car->nLicense, st->name);
        st->nCars++;
        break;

    case PORT_TYPE_NOT_FOUND:
        displayError(33);
        printf("Car %s cannot be charged at station %s. Required port type is not supported.\n", car->nLicense, st->name);
        break;
    }

    return 1;
}


// Option 3 - ahow car's status
// Checks if car is charging, in queue, or not in system
int checkCarStatus(StationsADT* stations, CarsADT* cars)
{
    Car* car = getCar(cars);
    if (!car)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    if (car->pPort)
    {
        printChargingStatus(car, stations);
        return 1;
    }

    if (car->inqueue)
    {
        printQueueStatus(car, stations);
        return 1;
    }

    // not charging or waiting
    printf("Car with license number %s is neither connected to a charging port nor waiting in any station queue.\n", car->nLicense);

    return 1;
}


// Option 4 - Stop charging a car
// Disconnects the car from port and advances queue if needed
int stopCharge(StationsADT* stations, CarsADT* cars)
{
    Car* car = getCar(cars);
    if (!car)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    if (!car->pPort)
    {
        printf("Car with license number %s isn't connected to a charging port.\n", car->nLicense);
        return 1;
    }

    char idStr[16];
    snprintf(idStr, sizeof(idStr), "%d", car->stationId);

    Station* st = searchStation(stations, idStr, SEARCH_BY_ID, 0, 0);
    if (!st)
    {
        displayError(ERR_STATION_NOT_FOUND);
        return 0;
    }

    // disconnect and check if next car can be moved to port
    Port* port = car->pPort;
    disconnectOnly(car, port);
    advanceQueue(port, st);

    // print final car status
    printCarDetails(car);

    return 1;
}



// Option 5 - Display all stations
// Recursively prints all stations in the BST with their info
void dispAllSt(StationsADT* stationsManager)
{
    if (!stationsManager || !stationsManager->root)
    {
        printf("No stations available in the system.\n");
        return;
    }

    Station* root = stationsManager->root;

    if (root->left)
    {
        StationsADT leftSubtree = { root->left };
        dispAllSt(&leftSubtree);
    }

    // count how many cars are currently charging at this station
    int chargingCount = 0;
    Port* curr = root->portsList->head;
    while (curr)
    {
        if (curr->status == OCCUPIED)
            chargingCount++;
        curr = curr->next;
    }

    printf("Station ID: %d\n", root->id);
    printf("Name: %s\n", root->name);
    printf("Location: (%.2lf, %.2lf)\n", root->coord.x, root->coord.y);

    printf("Ports:\n");
    printPorts(root->portsList->head);

    printf("Cars charging now: %d\n", chargingCount);
    printf("Cars in queue: %d\n", root->nCars);
    printf("-----------------------------\n\n\n");

    if (root->right)
    {
        StationsADT rightSubtree = { root->right };
        dispAllSt(&rightSubtree);
    }
}


// Option 6 - Display cars in a station (charging and in queue)
int dispCarsAtSt(StationsADT* stationsManager)
{
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    int hasChargingCars = 0;
    Port* port = st->portsList->head;
    while (port)
    {
        if (port->status == OCCUPIED && port->p2car)
            hasChargingCars = 1;
        port = port->next;
    }

    // print charging cars
    if (hasChargingCars)
    {
        printf("Cars currently charging at station: %s (ID: %d)\n", st->name, st->id);
        port = st->portsList->head;
        while (port)
        {
            if (port->status == OCCUPIED && port->p2car)
            {
                Car* car = port->p2car;
                printf("  - License: %s | Port Type: %s",
                    car->nLicense,
                    (car->portType == FAST) ? "FAST" :
                    (car->portType == MID) ? "MID" : "SLOW");

                Date now = getCurrentTime();
                int duration = calcMinutesPassed(port->tin, now);
                printf(" | Charging Time: %d minutes\n", duration);
            }
            port = port->next;
        }
    }
    else
    {
        printf("No cars currently charging at station: %s (ID: %d)\n", st->name, st->id);
    }

    // print queue
    if (isQueueEmpty(&st->carQueue))
    {
        printf("No cars currently waiting in queue at station: %s (ID: %d)\n", st->name, st->id);
    }
    else
    {
        printf("\nCars in queue at station: %s (ID: %d)\n", st->name, st->id);
        carNode* curr = st->carQueue.front;
        while (curr)
        {
            Car* car = curr->pCar;
            printf("  - License: %s | Port Type: %s\n", car->nLicense,
                (car->portType == FAST) ? "FAST" :
                (car->portType == MID) ? "MID" : "SLOW");
            curr = curr->next;
        }
    }
    return 1;
}



// Option 7 - Display statistics for a specific station
// Shows station utilization, faulty ports and load level
int reportStStat(StationsADT* stationsManager)
{
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    printf("\n=== Station Statistics for: %s (ID: %d) ===\n", st->name, st->id);
    StationUtilization(st);
    FaultyPorts(st);
    LoadLevel(st);
    printf("============================================\n\n");
    return 1;
}


// Option 8 - Show top 5 customers based on total payment
void dispTopCustomers(CarsADT* cars)
{
    if (!cars || !cars->root)
        return;

    int count = countCarsInBST(cars->root);
    if (count == 0)
        return;

    // create array of car pointers and fill with inorder traversal
    Car** carArray = (Car**)malloc(sizeof(Car*) * count);
    if (!carArray)
    {
        displayError(ERR_MEMORY_ALLOC_GENERAL);
        return;
    }

    int index = 0;
    fillCarArrayInorder(cars->root, carArray, &index);

    // sort array by totalPayed 
    qsort(carArray, count, sizeof(Car*), compareCarsByPayed);

    int limit = count < 5 ? count : 5;
    printf("Top %d Customers:\n", limit);
    printf("-------------------\n");
    for (int i = 0; i < limit; i++)
    {
        printf("%d. License: %s | Total Payed: %.2f\n", i + 1, carArray[i]->nLicense, carArray[i]->totalPayed);
    }
    printf("-------------------\n");

    free(carArray);
    return;
}


// Option 9 - Add new port to a station
// Gets valid station, asks for port type, creates and adds new port, then tries to serve waiting cars
int addNewPort(StationsADT* stationsManager)
{
    printf("Please select station for your new port\n");
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    int portTypeInput;
    printf("Your station is \"%s\"\n", st->name);
    printf("Enter port type (1=SLOW, 2=MID, 3=FAST): ");

    if (scanf("%d", &portTypeInput) != 1 || portTypeInput < 1 || portTypeInput > 3)
    {
        displayError(ERR_INVALID_INPUT);
        return 0;
    }

    // convert input to enum value (0 = SLOW, 1 = MID, 2 = FAST)
    portType selectedType = (portType)(portTypeInput - 1);

    // Assign a number greater than all existing port numbers.
    int maxPortNum = 0;

    for (Port* current = st->portsList->head;
        current != NULL;
        current = current->next)
    {
        if (current->num > maxPortNum)
            maxPortNum = current->num;
    }

    int newPnum = maxPortNum + 1;

    // create the port object
    Port* newPort = createPort(newPnum, selectedType);
    if (!newPort)
        return 0;

    // add to station's port list and update count
    addPortToList(st->portsList, newPort);
    st->nPorts++;

    const char* portTypeStr[] = { "SLOW", "MID", "FAST" };
    printf("New charging port added successfully to station %s (ID: %d)\n", st->name, st->id);
    printf("Port number: #%d | Charging type: %s\n", newPort->num, portTypeStr[newPort->portType]);

    // attempt to assign waiting car from queue if any
    advanceQueue(newPort, st);

    getchar(); // consume leftover newline character from scanf
    return 1;
}


// Option 10 - Release ports where cars overstay
// Disconnects cars that exceeded max charging time and tries to serve next cars in queue
int releasePorts(StationsADT* stationsManager)
{
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    printf("Found station: %s (ID: %d)\n\n", st->name, st->id);

    Date now = getCurrentTime(); // get current time
    Port* port = st->portsList->head;
    int released = 0; // counter for released ports

    // go through each port in the station
    while (port)
    {
        // check ports that are occupied and have a car
        if (port->status == OCCUPIED && port->p2car)
        {
            int duration = calcMinutesPassed(port->tin, now); // Charging duration in minutes

            // If charging time exceeds allowed , release the port
            if (duration >= MAX_CHARGE_MINUTES)
            {
                disconnectOnly(port->p2car, port); // disconnect car from port
                advanceQueue(port, st); // try to assign next car from queue
                printf("\n");
                released++;
            }
        }
        port = port->next;
    }

    // If no cars were released, notify the user
    if (released == 0)
    {
        printf("No ports released. All cars are within allowed charging time.\n");
    }

    return 1;
}


// Option 11 - Remove a faulty charging port from a station
int remOutOrderPort(StationsADT* stationsManager)
{
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    if (!hasOutOfOrderPorts(st))
    {
        printf("There are no out-of-order ports at station %s.\n", st->name);
        waitForEnter();
        return 0;
    }

    printOutOfOrderPorts(st);
    int portNum = getValidPortToRemove(st);
    if (portNum == 0)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    // try to remove port
    if (removePortFromList(st->portsList, portNum))
    {
        st->nPorts--;
        printf("\nPort #%d was successfully removed from station \"%s\".\n", portNum, st->name);
    }
    else
    {
        printf("\nFailed to remove port #%d from station \"%s\".\n", portNum, st->name);
    }

    return 1;
}


// Option 12 - Remove a car from the system
int remCustomer(CarsADT* cars)
{
    Car* car = getCar(cars);
    if (!car)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    // don't allow removing a car that is active
    if (car->pPort != NULL || car->inqueue)
    {
        displayError(ERR_REMOVE_ACTIVE_CAR);
        return 1;
    }

    char license[MAX_LICENSE_LEN];
    strcpy(license, car->nLicense);
    deleteCarByLicense(cars, car->nLicense);
    printf("Car has %s been successfully removed from the system.\n", license);
    return 1;
}


// Option 13 - Close a station and free all memory
int closeSt(StationsADT* stationsManager)
{
    Station* st = getValidStationFromUser(stationsManager);
    if (!st)
    {
        clearScreen();
        printf("Returning to main menu...\n\n");
        return 0;
    }

    printf("\nProceeding with closure of station: %s\n\n", st->name);
    Port* curr = st->portsList->head;
    int disconnectedCount = 0;

    // disconnect all cars before removing station
    while (curr)
    {
        if (curr->status == OCCUPIED && curr->p2car)
        {
            if (disconnectedCount > 0)
                printf("\n");

            disconnectOnly(curr->p2car, curr);
            disconnectedCount++;
        }
        curr = curr->next;
    }

    char name[100];
    strcpy(name, st->name);
    stationsManager->root = removeStationFromBST(stationsManager->root, st->id);
    printf("\nStation %s closed successfully.\n", name);

    return 1;
}


// Option 0 - Exit system safely and save all data
void exitSys(StationsADT* stations, CarsADT* cars)
{
    clearScreen();
    updateFiles(stations, cars); // Save data to files
    freeStationTree(stations);
    freeCarTree(cars);
    free(stations);
    free(cars);
    printf("=========================================\n\n");
    printf("All data has been saved successfully , Thank you for using EasyCharge!\n");
    printf("Goodbye, and have an electrifying day \n\n");
    printf("=========================================\n");
}
