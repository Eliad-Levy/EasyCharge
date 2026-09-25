#include "Station_BST.h"

// allocate and initialize station manager struct
StationsADT* createStationsManager()
{
    StationsADT* manager = (StationsADT*)malloc(sizeof(StationsADT));
    if (!manager) {
        displayError(25); // memory allocation failed
        return NULL;
    }
    manager->root = NULL;
    return manager;
}

// make a new station node 
Station* createStationNode(Station* newStation)
{
    Station* node = (Station*)malloc(sizeof(Station));
    if (!node) {
        displayError(25);
        return NULL;
    }
    *node = *newStation; 
    node->left = node->right = NULL;
    node->portsList = (PortList*)calloc(1, sizeof(PortList));
    if (!node->portsList) {
        displayError(ERR_MEMORY_ALLOC_PORTLIST);
        free(node);
        return NULL;
    }
    node->carQueue.front = node->carQueue.rear = NULL;
    return node;
}

// insert station into BST (by ID)
void insertStation(StationsADT* manager, Station* newStation)
{
    manager->root = insertStationRec(manager->root, newStation);
}

// recursive insert by ID
Station* insertStationRec(Station* root, Station* newStation)
{
    if (!root)
        return createStationNode(newStation);

    if (newStation->id < root->id)
        root->left = insertStationRec(root->left, newStation);
    else if (newStation->id > root->id)
        root->right = insertStationRec(root->right, newStation);

    return root;
}

// search station by name (recursive)
Station* searchStationByName(StationsADT* manager, const char* name)
{
    return searchStationByNameRec(manager->root, name);
}

Station* searchStationByNameRec(Station* root, const char* name)
{
    if (!root) return NULL;

    Station* found = searchStationByNameRec(root->left, name);
    if (found) return found;

    if (strcmp(root->name, name) == 0)
        return root;

    return searchStationByNameRec(root->right, name);
}

// search station by ID (input as string)
Station* searchStationByIdStr(StationsADT* manager, const char* input)
{
    int id = atoi(input);
    return searchStationIDRec(manager->root, id);
}

// recursive ID search
Station* searchStationIDRec(Station* root, int id)
{
    if (!root) return NULL;

    if (id < root->id)
        return searchStationIDRec(root->left, id);
    else if (id > root->id)
        return searchStationIDRec(root->right, id);
    else
        return root;
}

// recursive search for closest station to user location
void findNearestStation(Station* node, double userX, double userY, Station** nearestStation, double* minDistance)
{
    if (!node) return;

    double dist = calcDistance(userX, userY, node->coord.x, node->coord.y);
    if (*nearestStation == NULL || dist < *minDistance) {
        *nearestStation = node;
        *minDistance = dist;
    }

    findNearestStation(node->left, userX, userY, nearestStation, minDistance);
    findNearestStation(node->right, userX, userY, nearestStation, minDistance);
}

// find station by distance
Station* searchStationByDistance(StationsADT* manager, double x, double y)
{
    Station* nearest = NULL;
    double minDist = 0;
    findNearestStation(manager->root, x, y, &nearest, &minDist);
    return nearest;
}

// print all stations in order (by ID)
void inOrderStations(StationsADT* manager)
{
    inOrderStationsRec(manager->root);
}

void inOrderStationsRec(Station* root)
{
    if (!root) return;

    inOrderStationsRec(root->left);
    printf("Station ID: %d, Name: %s\n", root->id, root->name);
    inOrderStationsRec(root->right);
}

// free the full station tree
void freeStationTree(StationsADT* manager)
{
    freeStationTreeRec(manager->root);
    manager->root = NULL;
}

void freeStationTreeRec(Station* root)
{
    if (!root) return;

    freeStationTreeRec(root->left);
    freeStationTreeRec(root->right);

    free(root->name);
    freeQueue(&root->carQueue);
    freePortList(root->portsList);
    free(root->portsList);
    free(root);
}

// find the station that owns a specific port pointer
Station* findStationOfPortRec(Station* root, Port* pPort)
{
    if (!root) return NULL;

    Port* curr = root->portsList ? root->portsList->head : NULL;
    while (curr) {
        if (curr == pPort)
            return root;
        curr = curr->next;
    }

    Station* leftResult = findStationOfPortRec(root->left, pPort);
    if (leftResult) return leftResult;

    return findStationOfPortRec(root->right, pPort);
}

// remove a station from the BST
Station* removeStationFromBST(Station* root, int id)
{
    if (!root)
        return NULL;

    if (id < root->id)
        root->left = removeStationFromBST(root->left, id);
    else if (id > root->id)
        root->right = removeStationFromBST(root->right, id);
    else
    {
        // case 1: no children
        if (!root->left && !root->right) {
            free(root->name);
            freePortList(root->portsList);
            free(root->portsList);
            freeQueue(&(root->carQueue));
            free(root);
            return NULL;
        }

        // case 2: one child
        if (!root->left || !root->right) {
            Station* temp = root->left ? root->left : root->right;
            free(root->name);
            freePortList(root->portsList);
            free(root->portsList);
            freeQueue(&(root->carQueue));
            free(root);
            return temp;
        }

        // case 3: two children
        Station* parent = root;
        Station* successor = root->right;

        // find leftmost node in right subtree
        while (successor->left) {
            parent = successor;
            successor = successor->left;
        }

        // link around successor
        if (parent != root)
            parent->left = successor->right;
        else
            parent->right = successor->right;

        // attach children to successor
        successor->left = root->left;
        successor->right = root->right;

        free(root->name);
        freePortList(root->portsList);
        free(root->portsList);
        freeQueue(&(root->carQueue));
        free(root);

        return successor;
    }

    return root;
}
