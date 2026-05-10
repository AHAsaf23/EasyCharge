#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "Station.h"

Station* searchStation(const StationBST* tree, int searchType, void* data);
/**
 * select a charging station by either ID or name.
 * @param stationTree Pointer to the station BST 
 * @return Pointer to the selected Station if found
 *  NULL if user cancels, station not found, or stationTree is NULL.
 */
Station* getStationByIdOrName(StationBST* stationTree);
/**
 * select a charging station by ID
 * @param stationTree Pointer to the station BST
 * @return Pointer to the selected Station if found
 *  NULL if user cancels, station not found, or stationTree is NULL.
 */
Station* getStationById(StationBST* stationTree);
void  displayError(int errorCode);


#endif //HELPER_FUNCTIONS_H