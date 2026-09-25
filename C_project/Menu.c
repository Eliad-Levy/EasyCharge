#include "Menu.h"


// Prints the list of available menu options
void Menu()
{
    printf("Please choose an option:\n");
    printf("1. Locate nearest station\n");
    printf("2. Charge car\n");
    printf("3. Check car status\n");
    printf("4. Stop charge\n");
    printf("5. Display all stations\n");
    printf("6. Display cars at station\n");
    printf("7. Report of stations' statistics\n");
    printf("8. Display Top customers\n");
    printf("9. Add new port\n");
    printf("10. Release charging ports\n");
    printf("11. Remove out of order ports\n");
    printf("12. Remove customer\n");
    printf("13. Close station\n");
    printf("0. Exit system\n");
}


// Repeats menu and reads a valid number from user (0-13)
int getValidMenuChoice()
{
    char input[100];
    int choice;

    while (1)
    {
        Menu();

        // Read input as string for validation
        if (!fgets(input, sizeof(input), stdin))
        {
            clearScreen();
            displayError(ERR_INVALID_INPUT);
            waitForEnter();
            continue;
        }

        // Remove newline
        input[strcspn(input, "\r\n")] = '\0';

        // Make sure only digits were entered
        if (!isDigitsOnly(input))
        {
            clearScreen();
            displayError(ERR_INVALID_OPTION);
            waitForEnter();
            continue;
        }

        choice = atoi(input);

        // Check range
        if (choice < 0 || choice > 13)
        {
            clearScreen();
            displayError(ERR_INVALID_OPTION);
            waitForEnter();
            continue;
        }

        return choice;
    }
}


// Calls the right function based on what the user chose
void userChoice(int choice, StationsADT* stationsManager, CarsADT* carsManager)
{
    switch (choice)
    {
    case 1:
        clearScreen();
        if (locNearSt(stationsManager)) waitForEnter();
        break;

    case 2:
        clearScreen();
        if (ChargeCar(stationsManager, carsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 3:
        clearScreen();
        if (checkCarStatus(stationsManager, carsManager)) waitForEnter();
        break;

    case 4:
        clearScreen();
        if (stopCharge(stationsManager, carsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 5:
        clearScreen();
        dispAllSt(stationsManager);
        waitForEnter();
        break;

    case 6:
        clearScreen();
        if (dispCarsAtSt(stationsManager))
        {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 7:
        clearScreen();
        if (reportStStat(stationsManager)) waitForEnter();
        break;

    case 8:
        clearScreen();
        dispTopCustomers(carsManager);
        waitForEnter();
        break;

    case 9:
        clearScreen();
        if (addNewPort(stationsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 10:
        clearScreen();
        if (releasePorts(stationsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 11:
        clearScreen();
        if (remOutOrderPort(stationsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 12:
        clearScreen();
        if (remCustomer(carsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 13:
        clearScreen();
        if (closeSt(stationsManager)) {
            updateFiles(stationsManager, carsManager);
            waitForEnter();
        }
        break;

    case 0:
        // exit selected , exitsys function called from main
        break;

    default:
        displayError(ERR_INVALID_OPTION);
        waitForEnter();
        break;
    }
}


// Shows message and waits for entet before clearing screen and continuing
void waitForEnter()
{
    printf("\nPress ENTER to return to the menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    clearScreen();
}


// Clears the console screen (platform dependent)
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#else
    // If not recognized, do nothing
#endif
}
