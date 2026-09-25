#include "Cars_BST.h"


// Create and return a new car manager
CarsADT* createCarsManager()
{
    CarsADT* manager = (CarsADT*)malloc(sizeof(CarsADT));
    if (!manager) {
        displayError(25);
        return NULL;
    }
    manager->root = NULL;
    return manager;
}

// Set up a new node with car data
tCar* createCarNode(Car* newCar)
{
    tCar* node = (tCar*)malloc(sizeof(tCar));
    if (!node) {
        displayError(26);
        return NULL;
    }
    node->pCar = newCar;
    node->left = node->right = NULL;
    return node;
}

// Entry point to insert a car into tree
void insertCar(CarsADT* manager, Car* newCar)
{
    manager->root = insertCarRec(manager->root, newCar);
}

// Recursive insert based on license string
tCar* insertCarRec(tCar* root, Car* newCar)
{
    if (!root)
        return createCarNode(newCar);

    int cmp = strcmp(newCar->nLicense, root->pCar->nLicense);

    if (cmp < 0)
        root->left = insertCarRec(root->left, newCar);
    else if (cmp > 0)
        root->right = insertCarRec(root->right, newCar);

    // Duplicate license - skip insert
    return root;
}

// Begin in-order print of car data
void inOrderCars(CarsADT* manager)
{
    inOrderCarsRec(manager->root);
}

// In-order: left → node → right
void inOrderCarsRec(tCar* root)
{
    if (!root) return;
    inOrderCarsRec(root->left);
    printf("Car License: %s, Total Payed: %.2f\n", root->pCar->nLicense, root->pCar->totalPayed);
    inOrderCarsRec(root->right);
}

// Cleanup entire tree
void freeCarTree(CarsADT* manager)
{
    freeCarTreeRec(manager->root);
    manager->root = NULL;
}

// Post-order cleanup of tree
void freeCarTreeRec(tCar* root)
{
    if (!root) return;
    freeCarTreeRec(root->left);
    freeCarTreeRec(root->right);
    free(root->pCar);
    free(root);
}

// Remove car from tree by license
void deleteCarByLicense(CarsADT* m, const char* lic)
{
    if (!m || !lic) return;
    m->root = deleteCarRec(m->root, lic);
}

// Recursive deletion (leaf, one child, or two)
tCar* deleteCarRec(tCar* root, const char* license)
{
    if (!root)
        return NULL;

    int cmp = strcmp(license, root->pCar->nLicense);

    if (cmp < 0)
        root->left = deleteCarRec(root->left, license);
    else if (cmp > 0)
        root->right = deleteCarRec(root->right, license);
    else
    {
        // Found the matching license

        if (!root->left && !root->right)
        {
            // No children
            free(root->pCar);
            free(root);
            return NULL;
        }
        else if (!root->left || !root->right)
        {
            // One child
            tCar* child = root->left ? root->left : root->right;
            free(root->pCar);
            free(root);
            return child;
        }
        else
        {
            // Two children: use in-order successor
            tCar* parent = root;
            tCar* successor = root->right;

            while (successor->left)
            {
                parent = successor;
                successor = successor->left;
            }

            if (parent != root)
            {
                parent->left = successor->right;
                successor->right = root->right;
            }
            successor->left = root->left;

            free(root->pCar);
            free(root);
            return successor;
        }
    }

    return root;
}

// Let user add a new car - fills fields and inserts
Car* addNewCarToSystem(CarsADT* cars, const char* license)
{
    clearScreen();

    Car* newCar = (Car*)malloc(sizeof(Car));
    if (!newCar)
    {
        displayError(22);
        return NULL;
    }

    // Copy license with safety
    strncpy(newCar->nLicense, license, MAX_LICENSE_LEN);
    newCar->nLicense[MAX_LICENSE_LEN - 1] = '\0';

    int portChoice = -1;
    char buffer[10];

    // Prompt for port type
    while (1)
    {
        printf("Select charging speed for the car:\n");
        printf("1. FAST\n2. MID\n3. SLOW\n");
        printf("Your choice: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            displayError(2);
            printf("Please enter 1, 2 or 3.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &portChoice) != 1 || portChoice < 1 || portChoice > 3)
        {
            displayError(2);
            printf("Please enter 1, 2 or 3.\n");
            continue;
        }

        break;
    }

    // Map choice to enum
    switch (portChoice)
    {
    case 1: newCar->portType = FAST; break;
    case 2: newCar->portType = MID; break;
    case 3: newCar->portType = SLOW; break;
    }

    // Defaults
    newCar->totalPayed = 0;
    newCar->stationId = 0;
    newCar->pPort = NULL;
    newCar->inqueue = 0;

    insertCar(cars, newCar);
    printf("New car with license %s added successfully!\n", license);
    return newCar;
}

// Count how many cars are stored (recursive scan)
int countCarsInBST(tCar* root)
{
    if (!root)
        return 0;
    return 1 + countCarsInBST(root->left) + countCarsInBST(root->right);
}

// recursive string comparison to locate car
tCar* searchCarRec(tCar* root, const char* license)
{
    if (!root) return NULL;
    int cmp = strcmp(license, root->pCar->nLicense);
    if (cmp < 0)
        return searchCarRec(root->left, license);
    else if (cmp > 0)
        return searchCarRec(root->right, license);
    else
        return root;
}