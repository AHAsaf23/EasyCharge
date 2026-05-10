#define _CRT_SECURE_NO_WARNINGS
#include "../Header FIles/Station.h"
#include "../Header FIles/port.h"
#include "../Header FIles/helper_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Station* createStation(int id, const char* name, Coord coord) {
    // allocate memory for the new station.
    Station* newStation = (Station*)malloc(sizeof(Station));
    if (newStation == NULL) {
        displayError(1);
        return NULL;
    }
    // Set station ID
    newStation->id = id;
    // Allocate and copy station name
    newStation->name = (char*)malloc(strlen(name) + 1);
    if (newStation->name == NULL) {
        displayError(1);
        free(newStation);
        return NULL;
    }
    strcpy(newStation->name,name);

    newStation->coord = coord;
    newStation->nPorts = 0;
    // Initialize ports list
    newStation->portsList = (PortList*)malloc(sizeof(PortList));
    if (newStation->portsList == NULL) {
        displayError(1);
        free(newStation->name);
        free(newStation);
        return NULL;
    }
    newStation->portsList->head = NULL;
    newStation->portsList->count = 0;
    // Initialize car queue
    newStation->carQueue = (qCar*)malloc(sizeof(qCar));
    if (newStation->carQueue == NULL) {
        displayError(1);
        free(newStation->name);
        free(newStation);
        return NULL;
    }
    initQueue(newStation->carQueue);
    newStation->nCars = 0;
    // Initialize BST pointers
    newStation->left = NULL;
    newStation->right = NULL;

    return newStation;
}
void initStationBst(StationBST * tree) {
    if (tree) {
        tree->root = NULL;
        tree->count = 0;
    }
}

void freeStation(Station* s) {
    if (s == NULL)
        return;
    //freeing name of the station
    if (s->name != NULL)
        free(s->name);
    //freeing port list
    freePortsList(s->portsList);
    //freeing cars in queue.
    carNode* curr = s->carQueue->front;
    while (curr!= NULL) {
        carNode* toFree = curr;
        curr = curr->next;
        free(toFree);
    }
    // Free the car queue structure
    free(s->carQueue);
    //free the station
    free(s);
}

void freeStationBst(StationBST* tree) {
    if (tree == NULL) return;
    freeStationBstRecursive(tree->root);
    tree->root = NULL;
    tree->count = 0;
}
void freeStationBstRecursive(Station* root) {
    if (root == NULL) return;
    freeStationBstRecursive(root->left);
    freeStationBstRecursive(root->right);
    freeStation(root);
}
Station* removeStationNode(Station* station, int id) {
    if (!station) return NULL;
    // Search for the node to remove
    if (id < station->id) {
        station->left = removeStationNode(station->left, id);
    }
    else if (id > station->id) {
        station->right = removeStationNode(station->right, id);
    }
    else {
        // Check if the station has cars in it - cannot remove if it does
        if (station->nCars > 0 || countCarsChargingInStation(station) > 0) {
            displayError(4);
            return station;
        }
        // Node with only right child or no child
        if (!station->left) {
            Station* temp = station->right;
            freeStation(station);
            return temp;
        //Node with only left child
        } else if (!station->right) {
            Station* temp = station->left;
            freeStation(station);
            return temp;
        }
        // Node with two children find successor
        Station* succParent = station;
        Station* succ = station->right;
        while (succ->left) {
            succParent = succ;
            succ = succ->left;
        }
        // Remove successor from its current position
        if (succParent != station)
            succParent->left = succ->right;
        else
            succParent->right = succ->right;

        // Replace station with successor
        succ->left = station->left;
        succ->right = station->right;

        freeStation(station);
        return succ;
    }
    return station;
}


void insertStation(StationBST* tree, Station* newSt) {
    if (!tree || !newSt) return;
    tree->root = insertStationRecursive(tree->root, newSt);
    tree->count++;
}

Station* insertStationRecursive(Station* root, Station* newSt) {
    // Validate the new station pointer
    if (newSt == NULL) {
        fprintf(stderr,"invalid station pointer\n");
        return root;
    }
    // If we reached an empty spot in the tree insert there
    if (root == NULL) {
        return newSt;
    }
    // Compare station IDs to decide where to insert
    if (newSt->id < root->id) {
        root->left = insertStationRecursive(root->left, newSt);
    }else if (newSt->id > root->id) {
        root->right = insertStationRecursive(root->right, newSt);
    }else {
        // ID already exist skip
        printf("id already in the system\n");
    }
    return root; // Return the root after insertion
}

Station* searchStationById(const StationBST* tree, int id) {
    // Validate tree and root existence
    if (tree == NULL || tree->root == NULL)
        return NULL;

    Station *temp = tree->root;
    // Iterative search through the BST
    while (temp != NULL) {
        if (id == temp->id) {
            return temp; // found the station
        } else if (id < temp->id) {
            temp = temp->left;
        } else {
            temp = temp->right;
        }
    }
    return NULL; // Station not found
}
Station* searchStationByName(const StationBST* tree, const char* name) {
    // Validate input parameters
    if (tree == NULL || tree->root == NULL || name == NULL)
        return NULL;
    return searchStationByNameRecursive(tree->root, name);
}
Station* searchStationByNameRecursive(const Station* root, const char* name) {
    // reached null node or invalid name
if ( !root || !name ) return NULL;
    // Check if current station matches the name
    if (strcmp(root->name , name) == 0) return (Station*)root;
    // Search in left subtree first
    Station* left = searchStationByNameRecursive(root->left, name);
    if (left) return left;
    // search in right if not found in left
    return searchStationByNameRecursive(root->right, name);
}

double calculateDistance(Coord a, Coord b) {
    double xDistance = a.x - b.x;
    double yDistance = a.y - b.y;
    return sqrt(xDistance * xDistance + yDistance * yDistance);
}

Station* searchStationByDistance(const StationBST* tree, Coord targetCoord) {
    // Validate tree structure
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }
    return searchStationByDistanceRecursive(tree->root, targetCoord);
}
Station* searchStationByDistanceRecursive(const Station* root, Coord location) {
    if (!root) return NULL;
    // Initialize current station as closest and calculate its distance
    Station * closestStation = (Station*)root;
    double minDistance = calculateDistance(root->coord, location);
    // Check left for closer stations
    Station* left = searchStationByDistanceRecursive(root->left, location);
    if (left) {
        double leftDistance = calculateDistance(left->coord, location);
        if (leftDistance < minDistance) {
            minDistance = leftDistance;
            closestStation = left;
        }
        // Check right for closer stations
    }    Station* right = searchStationByDistanceRecursive(root->right, location);
    if (right) {
        double rightDistance = calculateDistance(right->coord, location);
        if (rightDistance < minDistance) {
            minDistance = rightDistance;
            closestStation = right;
        }
    }
    return closestStation;
}



void inorderPrintStations(const StationBST* tree) {
    if (tree == NULL) return;
    inorderPrintStationsRecursive(tree->root);
}

void inorderPrintStationsRecursive(Station* root) {
    if (root == NULL) return;
    inorderPrintStationsRecursive(root->left);
    printStation(root);
    inorderPrintStationsRecursive(root->right);
}
void printStation(Station* station) {
    // Validate station pointer
    if (station == NULL) {
        printf("station is NULL\n");
        return;
    }
    int waiting = station->nCars; // Cars waiting in queue
    int inCharge = countCarsChargingInStation(station); // Cars currently charging
    printf("======================================\n");
    printf("Station name: %s \n",  station->name);
    printf("Station id: %d \n",  station->id);
    printf("Station Coordinates: %0.2f, %0.2f \n",  station->coord.x , station->coord.y);
    printPortsList(station->portsList); // Print ports in station
    printf("Amount of cars in queue: %d\n", waiting);
    printf("Amount of cars in charge %d\n",inCharge);
    printf("======================================\n\n");
}

int countStations(Station* root) {
    if (!root) return 0;
    // Count current node + left subtree + right subtree
    return 1 + countStations(root->left) + countStations(root->right);
}


void addPortToStation(Station* s, Port* p) {
    // Validate input parameters
    if (!s || !p) return;
    // Add port to the station's port list
    addPort(s->portsList,p);
    s->nPorts++;
}


Port* findAvailablePortInStation(const Station* s, portType type) {
    // Validate station pointer
    if (s == NULL)
        return NULL;
    Port* temp = s->portsList->head;
    while (temp != NULL) {
        // Check if port is free and matches requested type
        if (temp->status == FREE && temp->portType == type) {
            return temp; // Found matched port
        }
        temp = temp->next; // Move to the next port
    }
    return NULL;
}


bool removePortFromStation(Station *s, int portNum) {
    // Validate station pointer
    if (s == NULL)
        return false;
    // remove port from the station's port list
    if (removePort(s->portsList, portNum)) {
        s->nPorts--;
        return true;
    }
    return false;
}


int findStationIdByPort(const StationBST* tree, const Port* port) {
    // Validate input parameters
    if (!tree || !port) return 0;
    return findStationIdByPortRecursive(tree->root, port);
}
int findStationIdByPortRecursive(const Station* root, const Port* port) {
    if (!root || !port) return 0;
    // Check if current station contains the port
    if (isPortInList(root->portsList, port))
        return root->id;
    // Search in left
    int left = findStationIdByPortRecursive(root->left, port);
    if (left) return left;
    // Search in right if not found in left
    int right = findStationIdByPortRecursive(root->right, port);
    return right;
}


Station* findStationByCarInQueue(Station* root, Car* car) {
    // Validate input parameters
    if (!root || !car) return NULL;
    // Search in left first
    Station* left = findStationByCarInQueue(root->left, car);
    if (left) return left;
    // Check current station's car queue
    carNode* temp = root->carQueue->front;
    while (temp) {
        if (temp->p2car == car)
            return root;// Found car in this station's queue
        temp = temp->next;
    }
    // Search in right if not found
    return findStationByCarInQueue(root->right, car);
}

int countCarsChargingInStation(Station* station) {
    // Validate station pointer
    if (!station) return 0;
    int InCharge = 0;
    Port* temp = station->portsList->head;
    while (temp) {
        // Count ports that are currently occupied
        if (temp->status == OCCUPIED) {
            InCharge++;
        }
        temp = temp->next;
    }
    return InCharge;
}