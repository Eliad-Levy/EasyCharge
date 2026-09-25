# EasyCharge
### EV Charging Station Management System

A console application developed in C as the final project for the
Advanced Topics in C course at Ruppin Academic Center.

EasyCharge manages charging stations, vehicles, charging ports,
waiting queues, and simulated billing.

## Screenshots

### Main Menu
![EasyCharge main menu](screenshots/main-menu.png)

### Nearest Station Search
Searching with coordinates (32.79, 34.99) returns Haifa Port, with its charging port availability and queue status.

![Nearest station search showing Haifa Port](screenshots/nearest-station.png)


## Features

- Search for stations by ID, name, or proximity to input coordinates.
- Assign vehicles to compatible charging ports.
- Manage waiting queues and advance compatible vehicles when ports become available.
- Track charging status and calculate charges based on elapsed time.
- Display station statistics and top customers.
- Add charging ports, remove faulty ports, and close stations.
- Save and reload system state using text files.

## Implementation

- Binary search trees for stations and vehicles.
- Linked lists for charging ports.
- Linked queues for waiting vehicles.
- Dynamic memory allocation.
- Modular C source and header files.
- CSV-formatted text files for persistence.

## Build and Run

Tested on Windows using Visual Studio Community 2022.

Requirements:
- Desktop development with C++ workload.
- MSVC v143 build tools.
- Windows SDK.

Steps:
1. Download and extract the repository, or clone it.
2. Open `C_project.sln` in Visual Studio 2022.
3. Select `Debug` and `x64`.
4. Select **Build > Rebuild Solution**.
5. Press **Ctrl+F5** to run.

The working directory must be `C_project`, where the four data files
are located. If necessary, set the project's Debugging > Working
Directory to `$(ProjectDir)`.

## Quick Example

Choose option `1` and enter:
- X: `32.79`
- Y: `34.99`

The application should return Haifa Port with a distance of `0.00`.

Choose option `5` to display all stations.
Choose option `0` to save and exit.

## Data Files

The application uses:
- `Stations.txt`
- `Cars.txt`
- `Ports.txt`
- `LineOfCars.txt`

The fictional sample data were supplied as part of the course.
The included files reflect the state after maintenance testing.

Successful operations can update these files. Keep a backup if you
want to repeat a demonstration from the same starting state.

## Scope and Limitations

- This is an academic simulation, with no connection to physical
  chargers or payment services.
- Distance is calculated directly from coordinates, not road routes.
- Historic charging timestamps in the sample data can produce large
  elapsed times and simulated charges.
- File validation is not exhaustive, and saving multiple files is
  not an atomic transaction.

## Maintenance and Testing

The portfolio version includes fixes made after the course submission.
See [CHANGES.md](CHANGES.md) for details and
[TESTING.md](TESTING.md) for verification scope.

## Developer

Eliad Levy
Electrical and Electronics Engineering student, Ruppin Academic Center.
