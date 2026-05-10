#ifndef STATION_H
#define STATION_H

#include "car.h"
#include "port.h"
#include "carQueue.h"

typedef struct {
    double x;
    double y;
}Coord;

typedef struct Station{
    int id;
    char* name;
    int nPorts;
    Coord coord;
    PortList* portsList;
    int nCars;
    qCar* carQueue;
    struct Station* right;
    struct Station* left;
}Station;

typedef struct {
    Station* root;
    int count;
}StationBST;

/**
 * creates a new station
 * @param id Station id
 * @param name Station name
 * @param coord station coordinates
 * @return Pointer to the new station, NULL if failed
 */
Station* createStation(int id, const char* name, Coord coord);

/**
 * Initializes a StationBST
 * @param tree Pointer to the StationBst
 */
void initStationBst(StationBST* tree);

/**
 * Frees memory allocated for a single station
 */
void freeStation(Station* s);

/**
 * free the station BST.
 */
void freeStationBst(StationBST* tree);

/**
 * Recursively frees station BST nodes
 */
void freeStationBstRecursive(Station* root);

/**
 * Remove a station node from BST by id only if its empty
 * @param station Root of a subtree
 * @param id ID of the station to remove
 * @return Updated root after removal
 */
Station* removeStationNode(Station* station, int id);

/**
 * Inserts a station into the BST
 * @param tree Pointer to the BST
 * @param newSt Pointer to the station to insert
 */
void insertStation(StationBST* tree, Station* newSt);

/**
 * @brief Recursively inserts station into BST
 * @param root Current root node
 * @param newSt Station to insert
 * @return Updated root after insertion
 */
Station* insertStationRecursive(Station* root, Station* newSt);


/**
 * Prints all stations in BST in order (by ID)
 * @param tree Pointer to BST
 */
void inorderPrintStations(const StationBST* tree);

/**
 * Recursively prints stations in order
 * @param root Current node
 */
void inorderPrintStationsRecursive(Station* root);

/**
 * Prints information about a station
 * @param station Pointer to station to print
 */
void printStation(Station* station);

/**
 * Counts total number of stations in BST
 * @param root Root of BST
 * @return Total number of stations
 */
int countStations(Station* root);

// Port management

/**
 * Adds a port to a station
 * @param s Pointer to station
 * @param p Pointer to port to add
 */
void addPortToStation(Station* s, Port* p);

/**
 * Finds an available port of specified type in station
 * @param s Pointer to station
 * @param type Type of port needed
 * @return Pointer to available port, NULL if none available
 */
Port* findAvailablePortInStation(const Station* s, portType type);

/**
 * Removes a port from station by port number
 * @param s Pointer to station
 * @param portNum Port number to remove
 */
bool removePortFromStation(Station *s, int portNum);

//Car management
/**
 * Counts number of cars currently charging in station
 * @param station Pointer to station
 * @return Number of cars charging
 */
int countCarsChargingInStation(Station* station);

// Search functions
/**
 * Finds station ID that contains a specific port
 * @param tree Pointer to BST
 * @param port Pointer to port to search for
 * @return Station ID containing the port, 0 if not found
 */
int findStationIdByPort(const StationBST* tree, const Port* port);
/**
 * Recursively searches for station containing a port
 * @param root Current node
 * @param port Port to search for
 * @return Station ID containing the port, 0 if not found
 */
int findStationIdByPortRecursive(const Station* root, const Port* port);
/**
 * Searches for station by ID
 * @param tree Pointer to BST
 * @param id Station ID to search for
 * @return Pointer to the station, NULL if not found
 */
Station* searchStationById(const StationBST* tree, int id);
/**
 * Searches for station by name
 * @param tree Pointer to BST
 * @param name Station ID to search for
 * @return Pointer to the station, NULL if not found
 */
Station* searchStationByName(const StationBST* tree, const char* name);
/**
 * Recursively searches for station by name
 * @param root Current node
 * @param name Name to search for
 * @return Pointer to found station, NULL if not found
 */
Station* searchStationByNameRecursive(const Station* root, const char* name);
/**
 * Calculates distance between two coordinates
 * @param a First coordinate
 * @param b Second coordinate
 * @return Distance between the coordinates
 */
double calculateDistance(Coord a, Coord b);
/**
 * Recursively finds closest station by distance
 * @param root Current node
 * @param location Target location
 * @return Pointer to the closest station
 */
Station* searchStationByDistanceRecursive(const Station* root, Coord location);
/**
 * Finds closest station
 * @param tree Pointer to BST
 * @param targetCoord Target coordinates
 * @return Pointer to the closest station, NULL if tree is empty
 */
Station* searchStationByDistance(const StationBST* tree, Coord targetCoord);
/**
 * Finds station where a specific car is waiting in queue
 * @param root Root of BST to search
 * @param car Pointer to car to find
 * @return Pointer to the station with the car, NULL if not found
 */
Station* findStationByCarInQueue(Station* root, Car* car);

#endif //STATION_H
