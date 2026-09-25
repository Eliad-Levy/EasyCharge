#include "LineOfCars.h"


// Initializes an empty queue of cars
qCar createEmptyQueue()
{
    qCar q = { NULL, NULL };
    return q;
}

// Enqueues a car at the end of the queue
void enqueueCar(qCar* q, Car* car)
{
    carNode* node = malloc(sizeof(carNode));
    if (!node) {
        displayError(28);
        return;
    }

    node->pCar = car;
    node->next = NULL;

    if (!q->rear) {
        q->front = q->rear = node;
    }
    else {
        q->rear->next = node;
        q->rear = node;
    }
}

// Removes the car at the front of the queue
Car* dequeueCar(qCar* q)
{
    if (!q->front)
        return NULL;

    carNode* temp = q->front;
    Car* car = temp->pCar;

    q->front = temp->next;
    if (!q->front)
        q->rear = NULL;

    free(temp);
    return car;
}

// Returns true if the queue is empty
bool isQueueEmpty(qCar* q)
{
    return (q->front == NULL);
}

// Removes a car with a specific license from the queue
bool removeCarFromQueue(qCar* q, const char* license)
{
    if (!q || !q->front)
        return false;

    carNode* curr = q->front, * prev = NULL;

    while (curr) {
        if (strcmp(curr->pCar->nLicense, license) == 0) {
            if (prev)
                prev->next = curr->next;
            else
                q->front = curr->next;

            if (curr == q->rear)
                q->rear = prev;

            free(curr);
            return true;
        }

        prev = curr;
        curr = curr->next;
    }

    return false;
}

// Frees all nodes in the queue and resets state
void freeQueue(qCar* q)
{
    carNode* curr = q->front;

    while (curr) {
        if (curr->pCar)
        {
            curr->pCar->inqueue = false;
            if (!curr->pCar->pPort)
                curr->pCar->stationId = 0;
        }

        carNode* next = curr->next;
        free(curr);
        curr = next;
    }

    q->front = q->rear = NULL;
}
