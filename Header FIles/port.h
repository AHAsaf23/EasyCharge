#ifndef PORT_H
#define PORT_H

#include "date.h"
#include "types.h"
#include <stdbool.h>

typedef struct Car Car;


typedef struct Port {
    int num; //num of port
    portType portType;// charging type(FAST,MID,SLOW)
    portStatus  status;// Port status(OCCUPIED, FREE, OUT OF ORDER)
    struct Car* p2car;// Pointer to car
    Date tin;// time of charging
    struct Port* next;// Pointer to next port
}Port;

//Portlist one way linked list
typedef struct {
    Port* head;// pointer to the head of the list
    int count;// totalPorts in the linked lists
}PortList;


/**
 * create and initializes new port
 * @param num of the port
 * @param type of port(FAST,MID,SLOW)
 * @return Pointer to the new port
 */
Port* createPort(int num, portType type);

/**
 * add a new port to the linked list of ports.
 * the new port is appended to the end of the list
 * @param list Pointer to PortList(linked list head)
 * @param newPort Pointer to the new port to add
 */
void addPort(PortList* list, Port* newPort);

/**
 *Searches for available port in the list with the given charging type
 *the port must be free and not out of order
 * @param list Pointer to portlist
 * @param portType desired charging type
 * @returnPointer to an available port or NULL of none found
 */
Port* findAvailablePort(const PortList* list, int portType);

/**
 * print the detailes of all the ports in the lost
 * @param list Pointer to the Portlist
 */
void printPortsList(const PortList* list);

/**
 * removes a port in the list by number
 * @param list Pointer to portlist
 * @param numToRemove Number of the port to remove
 */
bool removePort(PortList *list, int numToRemove);

/**
 * Finds a port in the list by its number
 * @param list Pointer to portlist
 * @param portNum Pointer to the port to find
 * @return Pointer to tje port if found, or NULL otherwise
 *
 * This function removing a port from certain portlist by its number.

 */
Port* findPortByNumber(PortList* list, int portNum);

/**
 * Assigns a car to a port and links the port to the car.
 * updates all relevant fields:port status, tin(start time), and pointers
 * @param car Pointer to the car to assign
 * @param port Pointer to the available port to connect the car
 */
bool assignCar2port(Car* car, Port* port);

/**
 * Check whether a specific port exists in the list.
 * @param list Pointer to portlist
 * @param isinthelist Pointer to the port to check
 * @return 1 if the port exists in the list, 0 otherwise
 */
int isPortInList(const PortList* list, const Port* isinthelist);

/**
 * free s the entire list
 * @param list
 */
void freePortsList(PortList* list);


#endif //PORT_H
