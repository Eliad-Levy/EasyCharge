#include "Ports_List.h"

// create a new port with 
Port* createPort(int num, portType type)
{
    Port* newPort = (Port*)malloc(sizeof(Port));
    if (!newPort) {
        displayError(23);
        return NULL;
    }

    newPort->num = num;
    newPort->portType = type;
    newPort->status = FREE;
    newPort->p2car = NULL;

    // reset time-in fields
    newPort->tin.year = 0;
    newPort->tin.month = 0;
    newPort->tin.day = 0;
    newPort->tin.hour = 0;
    newPort->tin.minute = 0;

    newPort->next = NULL;
    return newPort;
}

// Add port to end of list
void addPortToList(PortList* list, Port* newPort)
{
    if (!list || !newPort)
        return;

    if (!list->head) {
        list->head = newPort;
    }
    else {
        Port* temp = list->head;
        while (temp->next)
            temp = temp->next;
        temp->next = newPort;
    }
}

// Remove port from list by number
bool removePortFromList(PortList* list, int portNum)
{
    if (!list || !list->head)
        return false;

    Port* curr = list->head;
    Port* prev = NULL;

    while (curr) {
        if (curr->num == portNum) {
            if (prev)
                prev->next = curr->next;
            else
                list->head = curr->next;

            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

// Look for available port with matching type
PortSearchResult findAvailablePort(PortList* list, portType type, Port** foundPort)
{
    if (!list || !foundPort)
        return PORT_TYPE_NOT_FOUND;

    *foundPort = NULL;
    int foundUsableType = 0;
    Port* curr = list->head;

    while (curr)
    {
        // found a port with matching type
        if (curr->portType == type)
        {
            // make sure it's not out of order
            if (curr->status != OUT_OF_ORDER)
            {
                foundUsableType = 1; 

                // found a free port , return
                if (curr->status == FREE)
                {
                    *foundPort = curr;
                    return PORT_FOUND;
                }
            }
        }
        curr = curr->next;
    }

    // If type exists but all are busy -> ORTS_BUSY
    // If no matching type at all -> PORT_TYPE_NOT_FOUND
    return foundUsableType ? PORTS_BUSY : PORT_TYPE_NOT_FOUND;
}

// Free all memory in the port list
void freePortList(PortList* list)
{
    if (!list) return;
    Port* curr = list->head;
    while (curr) {
        Port* temp = curr;
        curr = curr->next;
        free(temp);
    }
    list->head = NULL;
}

// find port by number
Port* findPortByNum(PortList* list, int portNum)
{
    if (!list)
        return NULL;

    Port* curr = list->head;
    while (curr) {
        if (curr->num == portNum)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

// mark port as out of order
void markPortOutOfOrder(PortList* list, int portNum)
{
    Port* port = findPortByNum(list, portNum);
    if (port)
        port->status = OUT_OF_ORDER;
}

// count ports with given status
int countPorts(PortList* list, statusType statusToCount)
{
    if (!list)
        return 0;

    int count = 0;
    Port* curr = list->head;
    while (curr) {
        if (curr->status == statusToCount)
            count++;
        curr = curr->next;
    }
    return count;
}