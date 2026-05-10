#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H
#include "Station.h"

/**
 *Finds the closest station based on user input coordinates.
 *
 * @param stationTree Pointer to station tree.
 *
 * Asks for location input (x and y) and find the closest station.
 * Prints: station name, ID, distance from the location input, number of ports and port details (type and status)
 * and the number of cars waiting in line.
 */
void locNearSt(const StationBST *stationTree);

/**
 * Connecting a car to a charger at selected station.
 *
 * @param stationTree Pointer to the station tree.
 * @param carTree  Pointer to the car tree.
 *
 * The function lets user choose station by ID or name, then input the car license.
 * if the car isn't found in the system,the user can enter its details and add it to the tree.
 * if there's a port available, the car begins charging.Otherwise, it's added to the station queue.
 */
void chargeCar(StationBST* stationTree, CarBST* carTree);

/**
 * Displays current status of a car: charging, in queue, or idle.
 *
 * @param stationTree Pointer to the station tree.
 * @param carTree Pointer to the car tree.
 *
 * Asks the use to enter a car license and searches for it in the car tree.
 * if the car is charging, displays the station, port, and time elapsed.
 * if the car is waiting in queue, shows the station and its queue position.
 * Otherwise, informs the user the car is not charging or in queue.
 */
void checkCarStatus(StationBST* stationTree, CarBST* carTree);

/**
 * Stops charging for a specific car and assigns the next car from queue if available.
 *
* @param stationTree Pointer to the station tree.
 * @param carTree Pointer to the car tree.
 *
 * Calculates the cost based on charging duration, Update car and port status.
 */
void stopCharge(StationBST* stationTree, CarBST* carTree);

/**
 * Prints akk station with summary details.
 *
 * @param stationTree Pointer to the station tree.
 *
 * going through the station tree and displays:
 * name, ID, list of ports, number of cars charging and num of cars in queue.
 *
 */
void dispAllSt(StationBST* stationTree);

/**
 * Display all cars at a specific station.
 *
 * @param stationTree Pointer to the station tree.
 *
 *Asks for station name or ID, then shows all charging and queued cars with relevant details.
 */
void dispCarsAtSt(StationBST* stationTree);

/**
 * generates report on a selected station.
 *
 * Shows utilization rate %, faulty port ratio %, and load balance.
 *
 * @param stationTree Pointer to the station tree.
 */
void reportStStat(StationBST* stationTree);

/**
 * Displays top 5 cars with highest total payments.
 *
 * Traverses the car tree, collects all cars into an array,
 * and sorts them by their totalPayed field.
 * Prints license and full details of the top 5 paying customers.
 *
 * Allocates memory for sorting process and frees it afterward.
 * shows an error if memory allocation fails or if no cars exist.
 *
 * @param CarTree Pointer to the car tree.
 */
void dispTopCustomers(const CarBST* CarTree);

/**
 * Adds a new charging port to a specific station.
 *
 * Asks user to choose station and port type.
 * creates new port and connects a car from queue if available.
 *
 * @param stationTree Pointer to the station tree.
 */
void addNewPort(StationBST* stationTree);

/**
 * Releases any car that has been charging for over 10 hours at a given station.
 *
 * Disconnects overdue cars, calculates payment, and assigns queued cars to free ports if possible.
 *
 * @param stationTree
 */
void releasePorts(StationBST* stationTree);

/**
 * Removes "out-of-order" charging port from selected station.
 *
 * Lists all the faulty ports in station, then removes based on user input.
 *
 * @param stationTree Pointer to the station tree.
 */
void remOutOrderPort(StationBST* stationTree);

/**
 * Deletes a car from the system.
 *
 * Car can only be removed if it's not in queue or currently charging, free memory.
 * @param CarTree Pointer to the car tree.
 */
void remCustomer(CarBST* CarTree);

/**
 * Closes a selected station permanently.
 * Station can only be removed if there's no cars in queue or in charge int the station.
 * Removes station from tree and frees memory.
 *
 * @param stationTree Pointer to the station tree.
 */
void closeSt(StationBST *stationTree);

/**
 * Safely exits the system and saves all data.
 *
 * Saves current data to files(stations, ports, cars, queue) Free all memory and confirms shutdown.
 *
 * @param stationTree Pointer to the station tree.
 * @param carTree Pointer to the car tree.
 */
void exitSys(StationBST* stationTree,CarBST* carTree);

#endif //MENU_FUNCTIONS_H