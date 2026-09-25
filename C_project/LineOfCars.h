#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SystemCoreFuncs.h"
#include "Consts.h"
#include "data_structures.h"


// Queue management functions for handling cars waiting at charging stations


// Create and return an empty car queue
qCar createEmptyQueue();

// Add a car to the end of the queue
void enqueueCar(qCar* queue, Car* car);

// Remove the car at the front of the queue and return it
Car* dequeueCar(qCar* queue);

// Check if the queue is empty
bool isQueueEmpty(qCar* queue);

// Remove a specific car from the queue using license number
bool removeCarFromQueue(qCar* queue, const char* license);

// Free all memory used by the queue
void freeQueue(qCar* queue);
