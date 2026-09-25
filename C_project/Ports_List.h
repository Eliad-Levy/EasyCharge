#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"

// ===== Basic port list operations =====

// Create port with ID and type
Port* createPort(int num, portType type);

// Add port to list (end)
void addPortToList(PortList* list, Port* newPort);

// Remove port by ID
bool removePortFromList(PortList* list, int portNum);

// Find free port of given type
PortSearchResult findAvailablePort(PortList* list, portType type, Port** foundPort);

// Free all ports in list
void freePortList(PortList* list);

// Get port by ID
Port* findPortByNum(PortList* list, int portNum);

// Mark port as out-of-order
void markPortOutOfOrder(PortList* list, int portNum);

// Count ports by status
int countPorts(PortList* list, statusType statusToCount);
