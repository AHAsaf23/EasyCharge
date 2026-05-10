#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "Station.h"
#include "car.h"

/**
 * load files to ADT'S
 * @param CarTree Pointer to car BST
 * @param stationTree Pointer to station BST
 */
void loadFiles(CarBST* CarTree, StationBST* stationTree);

/**
 * Load station.txt file information to station BST
 * @param stationTree  Pointer to station BST
 */
void loadStations(StationBST* stationTree);
/**
 * Load station.txt file information to station BST
 * @param stationTree  Pointer to station BST
 */
void loadPorts(CarBST* CarTree, StationBST* stationTree);

/**
 * Load port.txt file information to one way linked ports lists
 * @param CarTree Pointer to car BST
 * @param stationTree Pointer to station BST
 */
void loadCars(CarBST* CarTree, StationBST* stationTree);

/**
 *Load car.txt file information to car BST
 * @param stationTree Pointer to station BST
 * @param CarTree Pointer to car BST
 */
void loadQueue(StationBST* stationTree, CarBST* CarTree);;


/**
 * Updates all files from ADT'S.
 * @param stationTree Pointer to the Station BST.
 * @param CarTree  Point to the Car BST.
 * Update the files after every action taken. Handles every ADT has it should.
 */
void updateFiles(StationBST* stationTree, CarBST* CarTree);

/**
 * Writes the Cars details to the given file.
 * @param filename Pointer to the name of the Cars file.
 * @param CarTree Pointer to the Car BST
 * @param stationTree Pointer to the Station BST.
 *
 *This function iterates in the Car tree and write the details of each Car in the asked order.
 */
void saveCarsToFile(const char* filename, CarBST* CarTree, StationBST* stationTree);

/**
 * Writes the Stations details to the given file.
 * @param filename Pointer to the name of the Stations file.
 * @param stationTree Pointer to the Station BST.
 *
 * This function iterates in the Station tree and write the details of each Station,in the asked order.
 */
void saveStationsToFile(const char* filename, StationBST* stationTree);

/**
 * Writes the Ports details to the given file.
 * @param filename Pointer to the name of the Ports file.
 * @param stationTree Pointer to the Station BST.
 *
 * This function iterates in the station tree, and every station is pointing to the portlist field,
 * and runs on the portlist linked list and writes the details of each Port in the Portlist, in the asked order.
 */
void savePortsToFile(const char* filename, StationBST* stationTree);

/**
 * Writes the details od the Cars waiting in queue.
 * @param filename Pointer to the name of the Queue file.
 * @param stationTree Pointer to the Station BST.
 *
 * This function iterates in the station tee, and every station is pointing to carQueue field,
 * and runs on the queue from front to back and writes the details of each Car in queue.
 */
void saveQueuesToFile(const char* filename, StationBST* stationTree);

#endif 