#pragma once

// This file holds all global constants and file names used in the program
// It includes limits, file paths and system values like charging rate

// ===== Search and Port Types =====
typedef enum { SEARCH_BY_ID, SEARCH_BY_NAME, SEARCH_BY_COORDS } SearchType;           // enum for station search modes (used by searchStation wrapper)
typedef enum { SLOW, MID, FAST } portType;                                             // Types of charging speed
typedef enum { OCCUPIED = 1, FREE = 2, OUT_OF_ORDER = 3 } statusType;                  // Port status: in use, free or broken
typedef enum { PORT_FOUND, PORT_TYPE_NOT_FOUND, PORTS_BUSY } PortSearchResult;        // Port search results

// ===== Limits and Sizes =====
#define MAX_LICENSE_LEN 9      // Max length for car license string
#define MAX_NAME_LEN    100    // Max length for station name

// ===== File Names =====
#define FILE_STATIONS      "Stations.txt"     // File with station data
#define FILE_PORTS         "Ports.txt"        // File with port data
#define FILE_CARS          "Cars.txt"         // File with cars data
#define FILE_LINE_OF_CARS  "LineOfCars.txt"   // File with queues (cars in line)

// ===== Payment and Timing =====
#define PAY_PER_MINUTE 1.2                    // Charging cost per minute (¤)
#define MAX_CHARGE_MINUTES (10 * 60)          // Max allowed charging time (10 hours)


// ===== Error Codes =====
// used by displayError() , (SystemCoreFunc.c)

// --- general errors ---
#define ERR_INVALID_OPTION          1
#define ERR_INVALID_INPUT           2
#define ERR_NULL_POINTER            3

// --- file errors ---
#define ERR_FILE_OPEN              10
#define ERR_FILE_FORMAT            11

// --- memory allocation errors ---
#define ERR_MEMORY_ALLOC_GENERAL        20
#define ERR_MEMORY_ALLOC_STATION        21
#define ERR_MEMORY_ALLOC_CAR            22
#define ERR_MEMORY_ALLOC_PORT           23
#define ERR_MEMORY_ALLOC_PORTLIST       24
#define ERR_MEMORY_ALLOC_STATION_NODE   25
#define ERR_MEMORY_ALLOC_CAR_NODE       26
#define ERR_MEMORY_ALLOC_REPLACE_CAR    27
#define ERR_MEMORY_ALLOC_ENQUEUE        28

// --- logical errors ---
#define ERR_STATION_NOT_FOUND       30
#define ERR_CAR_NOT_FOUND           31
#define ERR_NO_STATIONS_AVAILABLE   32
#define ERR_NO_MATCHING_PORT_TYPE   33
#define ERR_CAR_ALREADY_CHARGING    34
#define ERR_NO_FREE_PORT            35
#define ERR_REMOVE_ACTIVE_CAR       36

