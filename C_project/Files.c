#include "Files.h"


// ===== File Loading Functions =====

bool loadStations(const char* filename, StationsADT* stationsManager) 
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        displayError(10);
        return false;
    }

    char line[MAX_NAME_LEN];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ",");
        if (!token || strcmp(token, "ID") == 0)
            continue; // skip header line

        Station* station = (Station*)calloc(1, sizeof(Station));
        if (!station) {
            displayError(21);
            fclose(fp);
            return false;
        }

        // Init default pointers and counters
        station->left = station->right = NULL;
        station->portsList = NULL;
        station->carQueue = createEmptyQueue();
        station->nCars = 0;

        // Add station ID
        station->id = atoi(token);

        // Add station name (dynamic string)
        token = strtok(NULL, ",");
        if (!token) goto fail;
        station->name = (char*)malloc(strlen(token) + 1);
        if (!station->name) goto fail;
        strcpy(station->name, token);

        // Add number of ports in this station
        token = strtok(NULL, ",");
        if (!token) goto fail;
        station->nPorts = atoi(token);

        // Add X coordinate
        token = strtok(NULL, ",");
        if (!token) goto fail;
        station->coord.x = atof(token);

        // Add Y coordinate
        token = strtok(NULL, ",");
        if (!token) goto fail;
        station->coord.y = atof(token);

        // Insert fully initialized station to BST
        if (searchStationIDRec(stationsManager->root, station->id))
            goto fail;
        insertStation(stationsManager, station);
        Station* inserted = searchStationIDRec(stationsManager->root, station->id);
        if (!inserted) goto fail;
        // The tree node owns the name; release only the temporary struct.
        free(station);
        continue;

    fail:
        if (station->name) free(station->name);
        free(station);
        displayError(ERR_FILE_FORMAT);
        fclose(fp);
        return false;
    }

    bool readOK = !ferror(fp);
    if (fclose(fp) != 0) readOK = false;
    return readOK;
}





// Loads all car data and adds them to the car BST
bool loadCars(const char* filename, CarsADT* carsManager) 
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file: %s\n", filename);
        return false;
    }

    char line[MAX_NAME_LEN];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ",");
        if (!token || strcmp(token, "License") == 0)
            continue;

        Car* car = (Car*)malloc(sizeof(Car));
        if (!car) {
            displayError(22);
            fclose(fp);
            return false;
        }

        // Extract license number
        strncpy(car->nLicense, token, MAX_LICENSE_LEN);
        car->nLicense[MAX_LICENSE_LEN - 1] = '\0';

        // Parse port type enum
        token = strtok(NULL, ",");
        if (!token) { free(car); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        if (strcmp(token, "SLOW") == 0)
            car->portType = SLOW;
        else if (strcmp(token, "MID") == 0)
            car->portType = MID;
        else
            car->portType = FAST;

        // total amount paid
        token = strtok(NULL, ",");
        if (!token) { free(car); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        car->totalPayed = atof(token);

        // station ID
        token = strtok(NULL, ",");
        if (!token) { free(car); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        car->stationId = atoi(token);

        // Skip port number (not used)
        token = strtok(NULL, ",");
        if (!token) { free(car); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }

        // queue flag
        token = strtok(NULL, ",");
        if (!token) { free(car); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        car->inqueue = atoi(token);

        // Initialize pointer
        car->pPort = NULL;

        if (searchCar(carsManager, car->nLicense)) {
            free(car);
            fclose(fp);
            displayError(ERR_FILE_FORMAT);
            return false;
        }
        insertCar(carsManager, car);
        if (searchCar(carsManager, car->nLicense) != car) {
            free(car);
            fclose(fp);
            return false;
        }
    }

    bool readOK = !ferror(fp);
    if (fclose(fp) != 0) readOK = false;
    return readOK;
}

// Loads car queues and assigns them to matching stations
bool loadLineOfCars(const char* filename, StationsADT* stationsManager, CarsADT* carsManager)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        displayError(10);
        return false;
    }

    char line[MAX_NAME_LEN];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ",");
        if (!token || strcmp(token, "License") == 0)
            continue;

        // Get license and station ID
        char license[MAX_LICENSE_LEN];
        strncpy(license, token, MAX_LICENSE_LEN);
        license[MAX_LICENSE_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) { fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        int stationId = atoi(token);

        // Validate station exists
        Station* station = searchStationIDRec(stationsManager->root, stationId);
        if (!station) {
            displayError(30);
            fclose(fp);
            return false;
        }

        // Find matching car in BST
        Car* car = searchCar(carsManager, license);
        if (!car) {
            displayError(31);
            fclose(fp);
            return false;
        }

        // Add to queue only if not already charging
        if (car->inqueue && car->pPort == NULL) {
            carNode* oldRear = station->carQueue.rear;
            enqueueCar(&station->carQueue, car);
            if (station->carQueue.rear == oldRear) {
                fclose(fp);
                return false;
            }
            station->nCars++;
        }
    }

    bool readOK = !ferror(fp);
    if (fclose(fp) != 0) readOK = false;
    return readOK;
}


// Loads port data and assigns them to the appropriate stations
bool loadPorts(const char* filename, StationsADT* stationsManager, CarsADT* carsManager)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        displayError(10);
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ",");
        if (!token || strcmp(token, "StationID") == 0)
            continue; // skip header line

        int stationId = atoi(token);
        Station* station = searchStationIDRec(stationsManager->root, stationId);
        if (!station) {
            displayError(30);
            fclose(fp);
            return false;
        }

        Port* port = (Port*)malloc(sizeof(Port));
        if (!port) {
            displayError(23);
            fclose(fp);
            return false;
        }

        // port number
        token = strtok(NULL, ",");
        if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        port->num = atoi(token);

        // port type
        token = strtok(NULL, ",");
        if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        if (strcmp(token, "SLOW") == 0)
            port->portType = SLOW;
        else if (strcmp(token, "MID") == 0)
            port->portType = MID;
        else
            port->portType = FAST;

        // port status
        token = strtok(NULL, ",");
        if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; }
        port->status = atoi(token);

        // charging start time (Date struct)
        token = strtok(NULL, ","); if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; } port->tin.year = atoi(token);
        token = strtok(NULL, ","); if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; } port->tin.month = atoi(token);
        token = strtok(NULL, ","); if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; } port->tin.day = atoi(token);
        token = strtok(NULL, ","); if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; } port->tin.hour = atoi(token);
        token = strtok(NULL, ","); if (!token) { free(port); fclose(fp); displayError(ERR_FILE_FORMAT); return false; } port->tin.minute = atoi(token);

        // Try to link port to a car (if any)
        token = strtok(NULL, ",");
        port->p2car = NULL;
        if (token && strcmp(token, "-1") != 0) {
            Car* car = searchCar(carsManager, token);
            if (car) {
                port->p2car = car;
                car->pPort = port;
                car->inqueue = false;
            }
            else {
                displayError(31);
                free(port);
                fclose(fp);
                return false;
            }
        }

        port->next = NULL;

        // Create port list if needed, then add port
        if (station->portsList == NULL) {
            station->portsList = (PortList*)malloc(sizeof(PortList));
            if (!station->portsList) {
                displayError(24);
                free(port);
                fclose(fp);
                return false;
            }
            station->portsList->head = NULL;
        }
        addPortToList(station->portsList, port);
    }

    bool readOK = !ferror(fp);
    if (fclose(fp) != 0) readOK = false;
    return readOK;
}



// ===== File Updating Functions =====


// Writes all station data to the specified file
void writeStationsToFile(StationsADT* manager, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        displayError(ERR_FILE_OPEN);
        return;
    }

    fprintf(fp, "ID,StationName,NumOfPorts,CoordX,CoordY\n");
    writeStationsRec(fp, manager->root);
    fclose(fp);
}


// Recursively writes each station in-order
void writeStationsRec(FILE* fp, Station* root) 
{
    if (!root) return;
    writeStationsRec(fp, root->left);
    writeSingleStation(fp, root);
    writeStationsRec(fp, root->right);
}


// Outputs a single station's data to the file
void writeSingleStation(FILE* fp, Station* st) 
{
    fprintf(fp, "%d,%s,%d,%.2lf,%.2lf\n",
        st->id,
        st->name,
        st->nPorts,
        st->coord.x,
        st->coord.y);
}


// Writes all car data to the specified file
void writeCarsToFile(CarsADT* carsManager, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        displayError(ERR_FILE_OPEN);
        return;
    }

    fprintf(fp, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");
    writeCarsRec(fp, carsManager->root);
    fclose(fp);
}


// Recursively writes car data in in-order traversal
void writeCarsRec(FILE* fp, tCar* root)
{
    if (!root) return;
    writeCarsRec(fp, root->left);
    writeSingleCar(fp, root->pCar);
    writeCarsRec(fp, root->right);
}


// Outputs a single car's data to the file
void writeSingleCar(FILE* fp, Car* car) 
{
    int portNum = (car->pPort) ? car->pPort->num : 0;
    fprintf(fp, "%s,%s,%.2f,%d,%d,%d\n",
        car->nLicense,
        portTypeToString(car->portType),
        car->totalPayed,
        car->stationId,
        portNum,
        car->inqueue ? 1 : 0);
}


// Converts enum to string representation
const char* portTypeToString(portType type)
{
    switch (type) {
    case SLOW: return "SLOW";
    case MID: return "MID";
    case FAST: return "FAST";
    default: return "UNKNOWN";
    }
}


// Writes all ports data from all stations into the specified file
void writePortsToFile(StationsADT* stationsManager, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        displayError(ERR_FILE_OPEN);
        return;
    }

    // Write CSV header
    fprintf(fp, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n");

    // Recursively write ports for all stations
    writePortsRec(fp, stationsManager->root);

    fclose(fp);
}


// Recursive in-order traversal to write all station ports
void writePortsRec(FILE* fp, Station* root)
{
    if (!root) return;

    writePortsRec(fp, root->left);
    writeStationPorts(fp, root);
    writePortsRec(fp, root->right);
}


// Iterates through a station's port list and writes each port
void writeStationPorts(FILE* fp, Station* station)
{
    if (!station || !station->portsList) return;

    Port* curr = station->portsList->head;
    while (curr)
    {
        writeSinglePort(fp, station->id, curr);
        curr = curr->next;
    }
}


// Outputs a single port's data to file
void writeSinglePort(FILE* fp, int stationId, Port* port)
{
    const char* type = portTypeToString(port->portType);
    const char* license = (port->p2car) ? port->p2car->nLicense : "-1";

    fprintf(fp, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",
        stationId,
        port->num,
        type,
        port->status,
        port->tin.year,
        port->tin.month,
        port->tin.day,
        port->tin.hour,
        port->tin.minute,
        license);
}


// Writes all car queue data from all stations into the specified file
void writeLineOfCarsToFile(StationsADT* stationsManager, const char* filename)
{
    FILE* fp = fopen(filename, "w");
    if (!fp)
    {
        displayError(ERR_FILE_OPEN);
        return;
    }

    // Write CSV header
    fprintf(fp, "License,StationID\n");

    // Recursively write queues for all stations
    writeLineOfCarsRec(fp, stationsManager->root);

    fclose(fp);
}


// Recursive in-order traversal to write queues of all stations
void writeLineOfCarsRec(FILE* fp, Station* root)
{
    if (!root) return;

    writeLineOfCarsRec(fp, root->left);
    writeStationQueue(fp, root);
    writeLineOfCarsRec(fp, root->right);
}


// Writes the car queue of a single station
void writeStationQueue(FILE* fp, Station* station)
{
    if (!station || !station->carQueue.front) return;

    carNode* curr = station->carQueue.front;
    while (curr)
    {
        fprintf(fp, "%s,%d\n", curr->pCar->nLicense, station->id);
        curr = curr->next;
    }
}
