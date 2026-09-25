#pragma once
// This file defines all structs and enums used in the system,
// including cars, stations, ports, queues, and dates.

#include <stdbool.h>
#include "Consts.h"

// ===== Date Struct =====
typedef struct date
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
} Date; // Used for charge time and logs


// ===== Forward Declaration =====
struct port;
typedef struct port Port;

// ===== Car Struct =====
typedef struct car
{
	char nLicense[MAX_LICENSE_LEN];  // License number string
	portType portType;                // Desired charging type
	double totalPayed;                // Total amount paid by car
	int stationId;                    // ID of the station where the car is (if any)
	Port* pPort;                      // Pointer to the port it's connected to (if any)
	bool inqueue;                     // Whether the car is waiting in line
} Car;

// ===== Car BST Node =====
typedef struct tcar
{
	Car* pCar;
	struct tcar* right;
	struct tcar* left;
} tCar;

// ===== Car Queue Node =====
typedef struct carnode
{
	Car* pCar;
	struct carnode* next;
} carNode;

// ===== Car Queue Manager =====
typedef struct qcar
{
	carNode* front;
	carNode* rear;
} qCar;

// ===== Charging Port =====
typedef struct port
{
	int num;               // Port number in the station
	portType portType;     // Charging type
	statusType status;     // Port status
	Car* p2car;            // Pointer to the connected car
	Date tin;              // Start time of charging
	struct port* next;     // Next port in list
} Port;

// ===== Coordinate Struct =====
typedef struct coord
{
	double x;
	double y;
} Coord;

// ===== Port List Wrapper =====
typedef struct portList
{
	Port* head;
} PortList;

// ===== Station Struct =====
typedef struct station
{
	int id;                    // Station ID
	char* name;                // Dynamic name string
	int nPorts;                // Number of ports
	Coord coord;               // Location
	PortList* portsList;       // Pointer to list of ports
	int nCars;                 // Number of cars in queue
	qCar carQueue;             // Queue manager
	struct station* right;     // BST right child
	struct station* left;      // BST left child
} Station;

// ===== Car Manager (BST Root) =====
typedef struct
{
	tCar* root;
} CarsADT;

// ===== Station Manager (BST Root) =====
typedef struct
{
	Station* root;
} StationsADT;
