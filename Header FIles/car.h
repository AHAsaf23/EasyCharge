#ifndef CAR_H
#define CAR_H

#include "port.h"
#include "types.h"
#include <stdbool.h>


typedef struct Car {
    char nLicense[9];
    portType portType; //type of charging
    double totalPayed;//total amount paid for charging
    struct Port* pPort;// pointer to the port the car is connected to;
    int inqueue;// 1 if car is inqueue, 0 otherwise
}Car;

typedef struct tCar {
    Car* p2car;// pointer to car
    struct tCar* right;// right child
    struct tCar* left;// left child
}tCar;

/**
 * manager for car BST
 */
typedef struct {
    tCar* root;  //root of the BST
    int totalCars; //total number of cars in BST
}CarBST;


//function prototypes
//---------------------
/**
 * Creates and initializes a Car
 * @param license Car license string
 * @param portType Charging type(FAST,MID,SLOW)
 * @return pointer to new Car
 */
Car* createCar(const char* license, int portType);

/**
 * Prints the car information
 */
void printCar(const Car* c);

/**
 * Returns 1 if car is not connected and not inqueue-safe to delete.
 */
int isCarAvailableForDeletion(const Car* c);

/**
 * Creates and intializes an empty CarBST
 */
CarBST* createCarBST();

/**
 * inserting a car into the BST by license
 * @return 1 if successful, 0 if duplicate or error
 */
int insertCar(CarBST* CarTree, Car* newCar);

/**
 *Searches for a ca by license.
 * @return Pointer to the car if found, NULL otherwise
 */
Car* searchCar(CarBST* CarTree, const char* license);

int countCarNodes(const tCar* root);
/**
 * counts the number of cars in th BST=
 */
int countCars(const CarBST* CarTree);

/**
 * Frees all memory used by the car BST
 */
void freeCarsTree(CarBST* CarTree);


/**
 * Removes a carNode from the car tree based on the license
 * @param node pointer to the root node of the car tree
 * @param license string of the car to be removed
 * @return Pointer to the updated root of the car tree
 *
 * The function searches for a node with matching license. if found,it removes
 * the node while keeping the bst as he should be, and free its memory.
 * if not matching node is found, the original tree remains unchanged.
 */
tCar* removeCarNode(tCar* node, const char* license);

/**
 * Calculates the total charging time in minutes for a car since it was plugged in.
 * @param car Pointer to the Car struct that has the charging port information.
 * @param now_time The current date and time.
 * @return The total number of minutes the car has been charging.
 *
 * The function converts both the car's plug-in time and the current time into minutes,
 * then returns their difference.
 */
int totalChargeMinutes(Car* car, Date now_time);

/**
 * Save Cars to dynamic array in order from the car tree
 * @param root Pointer to the root of the car tree
 * @param arr Pointer to a dynamically allocated array of the car pointers to store.
 * @param index Pointer to an integer that track the current index of the array
 *
 * The function performs an in-order recursively on the car tree,
 * and populates the array with pointers to car structs in sorted order.
 */
void saveCarsInOrder(const tCar* root, Car** arr, int* index);

/**
 * Compare function for sorting Car pointers by total amount paid.
 * @param a Pointer to a pointer to the first Car object
 * @param b Pointer to a pointer to the second Car object.
 * @return An integer indicating the comparison result:
 * -Returns a positive value if c2->totalPayed > c1->totalPayed
 * -Returns a negative value if c2->totalPayed < c1->totalPayed
 * -Returns 0 if both have equal totalPayed
 *
 * this function was written for use with function 'qsort',
 * comparing two Car* elements stored in an array.
 * It handles NULL pointers by returning 0.
 */
int compareByPayed(const void* a, const void* b);



#endif //CAR_H
