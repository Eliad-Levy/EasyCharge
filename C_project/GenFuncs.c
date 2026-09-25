#include "GenFuncs.h"


// Gets user input for coordinates, allows exiting with 0
void getUserLocation(double* x, double* y)
{
    char input[100];
    double temp;
    char extra;

    // Input X coordinate
    while (1)
    {
        printf("Enter X coordinate (or type 0 to return to the menu): ");
        if (fgets(input, sizeof(input), stdin) && sscanf(input, "%lf %c", &temp, &extra) == 1 && isfinite(temp))
        {
            if (temp == 0)
            {
                *x = *y = -9999;
                return;
            }
            *x = temp;
            break;
        }
        displayError(ERR_INVALID_INPUT);
    }

    // Input Y coordinate
    while (1)
    {
        printf("Enter Y coordinate (or type 0 to return to the menu): ");
        if (fgets(input, sizeof(input), stdin) && sscanf(input, "%lf %c", &temp, &extra) == 1 && isfinite(temp))
        {
            if (temp == 0)
            {
                *x = *y = -9999;
                return;
            }
            *y = temp;
            break;
        }
        displayError(ERR_INVALID_INPUT);
    }
}


// Scans through a list of ports and updates counters for each type/status
void countPortStats(Port* head, int* active, int* occupied, int* slow, int* mid, int* fast)
{
    while (head) {
        if (head->status != OUT_OF_ORDER) {
            (*active)++;
            if (head->portType == SLOW) (*slow)++;
            else if (head->portType == MID) (*mid)++;
            else if (head->portType == FAST) (*fast)++;

            if (head->status == OCCUPIED) (*occupied)++;
        }
        head = head->next;
    }
}


// Prompts user for license input and validates it
int getValidLicenseFromUser(char* license)
{
    while (1)
    {
        printf("Enter car license number (digits only, or 0 to return to main menu): ");

        if (fgets(license, MAX_LICENSE_LEN + 1, stdin) == NULL)
        {
            displayError(32);
            continue;
        }

        if (!strchr(license, '\n')) // check for overflow
        {
            printf("Warning: License number too long. Please enter up to %d digits.\n", MAX_LICENSE_LEN - 1);
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        license[strcspn(license, "\n")] = '\0';

        if (strcmp(license, "0") == 0)
            return 0;

        // Must be digits only
        if (!isDigitsOnly(license))
        {
            displayError(2);
            printf("License must contain digits only.\n");
            printf("Press ENTER to try again or type 0 to return to the main menu: ");

            char choice[4];
            if (fgets(choice, sizeof(choice), stdin) == NULL)
                continue;

            choice[strcspn(choice, "\n")] = '\0';
            if (strcmp(choice, "0") == 0)
                return 0;

            continue;
        }

        return 1;
    }
}


// Gets a car by license if exists in the system
Car* getCar(CarsADT* cars)
{
    char license[MAX_LICENSE_LEN + 1];

    while (getValidLicenseFromUser(license))
    {
        Car* car = searchCar(cars, license);
        if (car)
            return car;

        // If not found, offer retry or exit
        displayError(31);
        printf("Press ENTER to search again or type 0 to return to the main menu: ");

        char choice[4];
        if (fgets(choice, sizeof(choice), stdin) == NULL)
            continue;

        choice[strcspn(choice, "\n")] = '\0';
        if (strcmp(choice, "0") == 0)
            return NULL;
    }

    return NULL;
}


//asks user to enter license and returns car if exists , or adds new one
Car* getExistingCarFromUser(CarsADT* cars)
{
    char license[MAX_LICENSE_LEN + 1];

    // Loop until valid car is returned or user exits
    while (getValidLicenseFromUser(license))
    {
        // Search for existing car
        Car* car = searchCar(cars, license);
        if (car)
            return car;

        // If not found, prompt for next step
        displayError(31);
        printf("Press ENTER to search again, type 0 to return to the main menu, or type 1 to add a new car: ");

        char choice[4];
        if (fgets(choice, sizeof(choice), stdin) == NULL)
            continue;

        choice[strcspn(choice, "\n")] = '\0';

        if (strcmp(choice, "0") == 0)
            return NULL; // user exits
        if (strcmp(choice, "1") == 0)
            return addNewCarToSystem(cars, license); // create new car with this license -> (cars_BST.c)
    }

    return NULL; // fallback in case user exited
}


// Gets valid station from user by ID or name , with validation
Station* getValidStationFromUser(StationsADT* stations)
{
    char input[MAX_NAME_LEN];
    Station* st = NULL;

    while (1)
    {
        printf("Enter station ID or name (or 0 to return to main menu): ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            displayError(2);
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "0") == 0)
            return NULL;

        // checks if input is ID (digits only), name (letters only), or invalid (mixed)
        int hasDigit = 0, hasAlpha = 0;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (isdigit((unsigned char)input[i]))
                hasDigit = 1;
            else
                hasAlpha = 1;
        }

        // mixed input (e.g. "123A")
        if (hasDigit && hasAlpha)
        {
            displayError(2);
            printf("Press ENTER to try again or type 0 to return to the main menu: ");

            char choice[4];
            fgets(choice, sizeof(choice), stdin);
            if (strcmp(choice, "0\n") == 0)
                return NULL;

            continue;
        }

        // Defines search type
        SearchType type = hasDigit ? SEARCH_BY_ID : SEARCH_BY_NAME;

        // shows user what kind of search is done 
        if (type == SEARCH_BY_ID)
            printf("Searching for station with ID: %s\n", input);
        else
            printf("Searching for station with name: %s\n", input);

        // Search and return if found
        st = searchStation(stations, input, type, 0, 0);
        if (!st)
        {
            displayError(30);
            printf("Press ENTER to try again or type 0 to return to the main menu: ");

            char choice[4];
            fgets(choice, sizeof(choice), stdin);
            if (strcmp(choice, "0\n") == 0)
                return NULL;

            continue;
        }

        return st;
    }
}


// Calculates payment for a car and adds it to total
void calculatePayment(Car* car)
{
    if (!car || !car->pPort)
    {
        displayError(ERR_NULL_POINTER);
        return;
    }

    // Get current time and calculate minutes
    Date now = getCurrentTime();
    int minutes = calcMinutesPassed(car->pPort->tin, now);
    double payment = minutes * PAY_PER_MINUTE;

    // Update car's total payment
    car->totalPayed += payment;

    printf("Charging stopped for car %s.\n", car->nLicense);
    printf("Total charging time: %d minutes | Payment: %.2f NIS\n", minutes, payment);
}


// Disconnects a car from port, clears pointers and status
void disconnectOnly(Car* car, Port* port)
{
    if (!car || !port)
    {
        displayError(ERR_NULL_POINTER);
        return;
    }

    calculatePayment(car);

    car->pPort = NULL;
    car->stationId = 0;
    port->p2car = NULL;
    port->status = FREE;
}


// Assigns first compatible car in queue to a free port
void advanceQueue(Port* port, Station* st)
{
    if (!port || !st)
    {
        displayError(ERR_NULL_POINTER);
        return;
    }

    carNode* prev = NULL;
    carNode* curr = st->carQueue.front;

    // Search for first matching car by port type
    while (curr)
    {
        if (curr->pCar->portType == port->portType)
            break;
        prev = curr;
        curr = curr->next;
    }

    if (curr)
    {
        Car* queuedCar = curr->pCar;

        // Remove from queue
        if (prev)
            prev->next = curr->next;
        else
            st->carQueue.front = curr->next;

        if (!curr->next)
            st->carQueue.rear = prev;

        st->nCars--;
        queuedCar->inqueue = false;

        // Assign to port
        assignCar2port(queuedCar, port);

        printf("Car %s assigned from queue to port #%d at station %s.\n",
            queuedCar->nLicense, port->num, st->name);

        free(curr);
    }
    else
    {
        printf("Port #%d at station %s is now FREE. No matching car in queue.\n",
            port->num, st->name);
    }
}



// Calculates and prints station utilization (occupied ports vs total)
void StationUtilization(Station* st)
{
    int occupied = 0;
    Port* curr = st->portsList->head;

    // Count occupied ports
    while (curr != NULL)
    {
        if (curr->status == OCCUPIED)
            occupied++;
        curr = curr->next;
    }

    double utilization = 0;
    if (st->nPorts > 0)
        utilization = ((double)occupied / st->nPorts) * 100;

    printf("Utilization      : %.2f%%  (%d out of %d ports occupied)\n", utilization, occupied, st->nPorts);
}


// Calculates and prints the percentage of faulty (out-of-order) ports
void FaultyPorts(Station* st)
{
    int faulty = 0;
    Port* curr = st->portsList->head;

    // Count faulty ports
    while (curr != NULL)
    {
        if (curr->status == OUT_OF_ORDER)
            faulty++;
        curr = curr->next;
    }

    double faultPercent = 0;
    if (st->nPorts > 0)
        faultPercent = (double)faulty / st->nPorts * 100;

    printf("Faulty Ports     : %.2f%%  (%d out of %d ports)\n", faultPercent, faulty, st->nPorts);
}


// Evaluates station load based on charging ports vs cars in queue
void LoadLevel(Station* st)
{
    int charging = 0;
    Port* curr = st->portsList->head;

    // Count how many cars are currently charging
    while (curr != NULL)
    {
        if (curr->p2car != NULL)
            charging++;
        curr = curr->next;
    }

    printf("Load Level       : ");

    // If queue is empty
    if (st->nCars == 0)
    {
        printf("Balanced (queue empty)\n");
        return;
    }

    //valuate load based on ratio of charging to waiting
    double ratio = (double)charging / st->nCars;

    if (ratio >= 1.0)
        printf("Balanced\n");
    else if (ratio > 0.2)
        printf("Heavy load\n");
    else
        printf("Very heavy load\n");
}


// Recursively fills an array with cars using inorder traversal of the tree
void fillCarArrayInorder(tCar* root, Car** arr, int* index)
{
    if (!root)
        return;

    fillCarArrayInorder(root->left, arr, index);
    arr[*index] = root->pCar;
    (*index)++;
    fillCarArrayInorder(root->right, arr, index);
}


// Comparator to sort cars by total payed amount
int compareCarsByPayed(const void* a, const void* b)
{
    Car* carA = *(Car**)a;
    Car* carB = *(Car**)b;

    if (carA->totalPayed < carB->totalPayed)
        return 1;
    if (carA->totalPayed > carB->totalPayed)
        return -1;
    return 0;
}


// Checks if station has any out of order ports
int hasOutOfOrderPorts(Station* st)
{
    Port* curr = st->portsList->head;

    while (curr)
    {
        if (curr->status == OUT_OF_ORDER)
            return 1;
        curr = curr->next;
    }
    return 0;
}


// Gets a valid port number from user to remove (must be OUT_OF_ORDER)
int getValidPortToRemove(Station* st)
{
    char input[16];

    while (1)
    {
        printf("\nEnter the number of the port to remove (or 0 to return to menu): ");
        if (!fgets(input, sizeof(input), stdin))
            continue;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "0") == 0)
            return 0;

        // digits only
        if (!isDigitsOnly(input))
        {
            displayError(ERR_INVALID_INPUT);
            printf("Press ENTER to try again or type 0 to return to menu: ");
            char retry[8];
            fgets(retry, sizeof(retry), stdin);
            retry[strcspn(retry, "\n")] = '\0';
            if (strcmp(retry, "0") == 0)
                return 0;
            continue;
        }

        // Convert and validate existence
        int portNum = atoi(input);
        Port* curr = st->portsList->head;
        while (curr)
        {
            if (curr->num == portNum && curr->status == OUT_OF_ORDER)
                return portNum;
            curr = curr->next;
        }

        // If no match
        displayError(ERR_INVALID_INPUT);
        printf("Port #%d is not found or not marked as out-of-order.\n", portNum);
        printf("Press ENTER to try again or type 0 to return to menu: ");
        char retry[8];
        fgets(retry, sizeof(retry), stdin);
        retry[strcspn(retry, "\n")] = '\0';
        if (strcmp(retry, "0") == 0)
            return 0;
    }
}


// Calculates distance between two coordinates
double calcDistance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}


// Returns 1 if string has only digits , 0 otherwise
int isDigitsOnly(const char* str)
{
    if (!str || *str == '\0')
        return 0;

    for (int i = 0; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}


// Returns 1 if string has only letters and spaces , 0 otherwise
int isLettersAndSpacesOnly(const char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i]))
            return 0;
    }
    return 1;
}


// Calculates the difference in minutes between two Date structs
int calcMinutesPassed(Date start, Date end)
{
    struct tm t_start = { 0 }, t_end = { 0 };

    // Convert start Date to struct 
    t_start.tm_year = start.year - 1900;
    t_start.tm_mon = start.month - 1;
    t_start.tm_mday = start.day;
    t_start.tm_hour = start.hour;
    t_start.tm_min = start.minute;

    // Convert end Date to struct 
    t_end.tm_year = end.year - 1900;
    t_end.tm_mon = end.month - 1;
    t_end.tm_mday = end.day;
    t_end.tm_hour = end.hour;
    t_end.tm_min = end.minute;

    // Convert both to time_t  
    time_t time1 = mktime(&t_start);
    time_t time2 = mktime(&t_end);

    // If conversion fails, return error
    if (time1 == (time_t)(-1) || time2 == (time_t)(-1))
    {
        displayError(ERR_NULL_POINTER);
        return 0;
    }

    // Calculate difference in seconds and convert to minutes
    double diffSeconds = difftime(time2, time1);
    return (int)(diffSeconds / 60);
}


// Returns the current system time as a Date struct
Date getCurrentTime()
{
    Date now;
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    // Extract fields from struct tm into our Date struct
    now.year = tm_info->tm_year + 1900;
    now.month = tm_info->tm_mon + 1;
    now.day = tm_info->tm_mday;
    now.hour = tm_info->tm_hour;
    now.minute = tm_info->tm_min;

    return now;
}
