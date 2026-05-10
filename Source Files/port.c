#include "../Header FIles/port.h"
#include "../Header FIles/car.h"
#include "../Header FIles/date.h"
#include "../Header FIles/types.h"
#include <stdio.h>
#include <stdlib.h>

#include "../Header FIles/Station.h"

Port* createPort(int num, portType type) {
    Port * port = (Port*)malloc(sizeof(Port));// allocate memory for port.
    if (port == NULL) {
        return NULL;
    }
    //initializes with the given variables
    port->num = num;
    port->portType = type;
    port->status = FREE;
    port->p2car = NULL;
    port->tin = createDate(0,0,0,0,0);
    port->next = NULL;
return port; //returning the port that we just created.
}

void addPort(PortList* list, Port* newPort) {
    //if the list is empty, we put the new port in the head of the list.
    if (list -> head == NULL) {
        list -> head = newPort;
        //going over th linked list.
    } else {
        Port* temp = list-> head; // temporary pointer to head of list.
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPort; //adding new port the linked list if temp->next points to null
    }
    list->count++; //updating the amount of ports in portlist.
}
Port* findAvailablePort(const PortList* list, int portType) {
    Port* temp = list->head;//temporary pointer to head of list.
    //running over the linked list.
    while (temp != NULL) {
        //in case the port type match and the status is free it return the available port.
        if (temp->portType == portType && temp->status == FREE) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; //return NULL in Case port isn't available.
}
void printPortsList(const PortList* list) {
    Port* temp = list -> head;
    printf("Ports:\n");
    while (temp != NULL) {
        printf("   port #%d ", temp->num); //prints port number.
        //prints type and status of the port.
        printf("Type: %s, Status: %s\n",portTypeToStr(temp->portType),portStatusToStr(temp->status));

        temp = temp->next;
    }
}

bool removePort(PortList *list, int numToRemove) {
    if (list == NULL || list->head == NULL) {
        return false;; //no option to remove from empty list
    }
    Port* temp = list->head; //temporary pointer to the current port in the list.
    Port* prev = NULL; // pointer to the previous port.
    //checking if the first port is the one that needs to be removed
    if (temp->num == numToRemove) {
        list->head = temp->next;//update the head of the list.
        free(temp);// freeing the deleted port.
        list->count--;//update the amount of ports in the portlist.
        return true;//removal succeeded
    }
    // Trying to find the port to remove
    while (temp != NULL && temp->num != numToRemove) {
        prev = temp;
        temp = temp->next;
    }
    // If port was found and we have a valid prev node
    // free and remove the node
    if (temp != NULL && prev != NULL ) {
        prev->next = temp->next;
        free(temp);
        list->count--;
        return true;
    }
    return false;
}

Port* findPortByNumber(PortList* list, int portNum) {
    Port* temp = list -> head;
    //running over the linked list
    while (temp != NULL) {
        if (temp->num == portNum) {
            return temp; // Found the port
        }
        temp = temp->next;
    }
    return NULL; // Port not found
}

bool assignCar2port(Car* car, Port* port) {
    // validate input parameters
    if (!car || !port) return false;
    // linking car to the port
    car->pPort = port;
    car->inqueue = 0;
    // linking port to car
    port->p2car = car;
    port->status = OCCUPIED;
    port->tin = getCurrentDate();
    return true;
}

int isPortInList(const PortList* list, const Port* isinthelist) {
    Port* temp = list -> head;
    //running over the linked list comparing port pointers
    while (temp!=NULL) {
        if (temp == isinthelist) return 1; // Found the port
        temp = temp->next;
    }
    return 0; // port not found
}

void freePortsList(PortList* list) {
    // validate list pointer
    if (!list) return;
    Port* temp = list->head;
    //running over the linked list and free each port
    while (temp) {
        Port* toFree = temp;
        temp = temp->next;
        // clean up car assignment if port has a car
        if (toFree->p2car) {
            toFree->p2car->pPort = NULL;
            toFree->p2car->inqueue = 0;
            toFree->p2car = NULL;
        }
        free(toFree);
    }
    // reset list to empty state
    list->head = NULL;
    list->count = 0;
}