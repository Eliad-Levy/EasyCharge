#include "PrintFuncs.h"

// prints basic info about nearest station
void printNearest(Station* nearest, double minDist, int active, int slow, int mid, int fast, int occupied)
{
    // show main identifiers
    printf("\nNearest station:\n");
    printf("Name: %s\n", nearest->name);
    printf("ID: %d\n", nearest->id);
    printf("Distance from your location: %.2lf\n", minDist);

    // show port types and usage
    printf("Active ports: %d\n", active);
    printf("Ports by type - SLOW: %d, MID: %d, FAST: %d\n", slow, mid, fast);
    printf("Occupied ports: %d\n", occupied);
    printf("Cars in queue: %d\n", nearest->nCars);
}

// prints all out-of-order ports in station
void printOutOfOrderPorts(Station* st)
{
    Port* curr = st->portsList->head;
    printf("\nOut-of-order ports at station %s (ID: %d):\n", st->name, st->id);

    while (curr)
    {
        // only print ports marked as out of order
        if (curr->status == OUT_OF_ORDER)
        {
            printf("  Port #%d | Type: %s\n",
                curr->num,
                (curr->portType == SLOW) ? "SLOW" :
                (curr->portType == MID) ? "MID" : "FAST");
        }
        curr = curr->next;
    }
}

// prints all cars in a given queue
void printQueue(qCar queue)
{
    carNode* curr = queue.front;
    while (curr) {
        Car* car = curr->pCar;

        // print basic info for each car in queue
        printf("    - License: %s | Port Type: %s | Total Payed: %.2f\n",
            car->nLicense,
            (car->portType == SLOW) ? "SLOW" :
            (car->portType == MID) ? "MID" : "FAST",
            car->totalPayed);

        curr = curr->next;
    }
}

// prints all ports in a station
void printPorts(Port* portList)
{
    while (portList) {
        // show port id, type and current status
        printf("  Port #%d | Type: %s | Status: %s\n",
            portList->num,
            (portList->portType == SLOW) ? "SLOW" :
            (portList->portType == MID) ? "MID" : "FAST",
            (portList->status == OCCUPIED) ? "OCCUPIED" :
            (portList->status == FREE) ? "FREE" : "OUT_OF_ORDER");

        portList = portList->next;
    }
}

// prints all stations in BST with full data -> used for debug
void printStationsFull(Station* root)
{
    if (!root) return;

    // recursive in-order traversal
    printStationsFull(root->left);

    printf("Station ID: %d\n", root->id);
    printf("Name: %s\n", root->name);
    printf("Location: (%.2lf, %.2lf)\n", root->coord.x, root->coord.y);

    printf("Ports:\n");
    printPorts(root->portsList->head);

    printf("Cars in queue: %d\n", root->nCars);
    if (isQueueEmpty(&root->carQueue))
        printf("  [Queue is empty]\n");
    else
        printQueue(root->carQueue);

    printf("-----------------------------\n");

    printStationsFull(root->right);
}

// prints all cars in BST -> used for debug
void printCarsTree(tCar* root)
{
    if (!root)
        return;

    printCarsTree(root->left);

    // print details of the car
    printf("License: %s | Charging Type: ", root->pCar->nLicense);
    switch (root->pCar->portType)
    {
    case FAST: printf("FAST"); break;
    case MID:  printf("MID");  break;
    case SLOW: printf("SLOW"); break;
    default:   printf("UNKNOWN"); break;
    }

    printf(" | Total Paid: %.2f", root->pCar->totalPayed);
    printf(" | In Queue: %s", root->pCar->inqueue ? "Yes" : "No");
    printf(" | Charging: %s", root->pCar->pPort ? "Yes" : "No");
    printf(" | Station ID: %d\n", root->pCar->stationId);

    printCarsTree(root->right);
}

// prints one station's details
void printSingleStation(Station* st)
{
    if (!st) return;

    printf("Station ID: %d\n", st->id);
    printf("Name: %s\n", st->name);
    printf("Location: (%.2lf, %.2lf)\n", st->coord.x, st->coord.y);

    printf("Ports:\n");
    printPorts(st->portsList->head);

    printf("Cars in queue: %d\n", st->nCars);
    printf("-----------------------------\n");
}

// prints one car's details
void printCarDetails(Car* car)
{
    if (!car) return;

    printf("\nCar Details:\n");
    printf("-------------\n");
    printf("License: %s\n", car->nLicense);

    printf("Charging Type: ");
    switch (car->portType)
    {
    case FAST: printf("FAST\n"); break;
    case MID:  printf("MID\n");  break;
    case SLOW: printf("SLOW\n"); break;
    }

    printf("Total Paid: %.2f\n", car->totalPayed);
    printf("-------------\n\n");
}

// prints car charging status with time
void printChargingStatus(Car* car, StationsADT* stations)
{
    if (!car || !car->pPort) {
        displayError(3); // not charging
        return;
    }

    // find station where car is charging
    char idStr[16];
    snprintf(idStr, sizeof(idStr), "%d", car->stationId);
    Station* station = searchStation(stations, idStr, SEARCH_BY_ID, 0, 0);

    if (!station)
    {
        displayError(30); // station not found
        return;
    }

    // display charging info
    printf("Car is charging at station: %s , ID: [%d]\n", station->name, station->id);
    printf("Port number: %d\n", car->pPort->num);

    Date now = getCurrentTime();
    int minutesCharged = calcMinutesPassed(car->pPort->tin, now);
    printf("Charging time: %d minutes\n", minutesCharged);
}

// prints queue status and position of car
void printQueueStatus(Car* car, StationsADT* stations)
{
    if (!car || !car->inqueue) {
        displayError(ERR_NULL_POINTER); // not in queue
        return;
    }

    // find station by cars stationId
    char idStr[16];
    snprintf(idStr, sizeof(idStr), "%d", car->stationId);
    Station* station = searchStation(stations, idStr, SEARCH_BY_ID, 0, 0);

    if (!station)
    {
        displayError(ERR_STATION_NOT_FOUND);
        return;
    }

    printf("Car is in queue at station: %s , ID: [%d]\n", station->name, station->id);

    // look for car's position in queue (only count same portType)
    int position = 1;
    carNode* curr = station->carQueue.front;

    while (curr)
    {
        if (curr->pCar == car)
            break;

        if (curr->pCar->portType == car->portType)
            position++;

        curr = curr->next;
    }

    if (!curr) {
        printf("Car not found in the station queue.\n");
        return;
    }

    const char* typeStr =
        (car->portType == FAST) ? "FAST" :
        (car->portType == MID) ? "MID" : "SLOW";

    printf("Port type: %s | Position in queue: %d\n", typeStr, position);
}