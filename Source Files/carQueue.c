#include <stdio.h>
#include <stdlib.h>
#include "../Header FIles/car.h"
#include "../Header FIles/carQueue.h"

void initQueue(qCar* q) {
    //initializes queue ADT.
    if (q != NULL) {
        q->front = NULL;
        q->rear = NULL;
    }
}

void enqueueCar(qCar* q, Car* car) {
    carNode* newCar = (carNode*)malloc(sizeof(carNode));//allocate memory for car node in queue
    if (newCar == NULL) {
        printf("Memory allocation failed in enqueue()\n");
        return;
    }
    newCar->p2car = car;
    newCar->next = NULL;

    //in case rear queue is NULL(means the queue is empty) both front and rear points to the new car.
    if (q->rear == NULL) {
        q->front = q->rear = newCar;
    }
    //else there's cars in the queue so adding the node to rear of queue.
    else {
        q->rear->next = newCar; // linking the previous node to the new node.
        q->rear = newCar;// updating the rear pointer to the new node.
    }
    car->inqueue = 1;//car is in queue.
}

Car* dequeueCar(qCar* q) {
    //if queue is empty or not initialize.
    if (q == NULL || q->front == NULL || q->rear == NULL) return NULL;

    Car* temp = q->front->p2car;// temporary pointer to the car in the front.
    carNode* NodeToFree = q->front;// pointer to the node that we will free.

    q->front = q->front->next;

    //if the queue empty after dequeue the rear=NULL.
    if (q->front == NULL) {
        q->rear = NULL;
    }
    temp->inqueue = 0;//car is out of queue.

    free(NodeToFree); // freeing the node.

    return temp; // returning a pointer to the dequeued car.
}

int isQueueEmpty(const qCar* q) {
    return (q == NULL || q->front == NULL);
}

//printing car that in queue.
void printQueueCars(const qCar* q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    carNode* temp = q->front;
    int pos = 1;
    printf("Cars in queue:\n");
    printf("----------------------------------------\n");

    while (temp != NULL) {
        Car* car = temp->p2car;
        printf("Car %d: ", pos);
        printf("License: %s ,", car->nLicense);
        printf("Type: %s \n",portTypeToStr(car->portType));
        pos++;
        temp = temp->next;
    }
    printf("----------------------------------------\n");
}

int getQueuePosition(const qCar* q, Car* car) {
    if (q == NULL || q->front == NULL|| car == NULL) {
        return -1;
    }
    carNode* temp = q->front;
    int pos = 1;
//running in the queue from front to back and when the given car found returns her position.
    while (temp != NULL) {
        if (temp->p2car == car) {
            return pos;
        }
        temp = temp->next;
        pos++;
    }
    return -1;
}

Car* getNextCarInQueue(qCar* carQueue, portType type) {
    if (isQueueEmpty(carQueue))return NULL;// checks if queue is empty
    carNode* temp = carQueue->front; //temporary pointer to front of the queue
    //passing on all the
    while (temp) {
        //if p2car isn't NULL and the port type of the car in queue equal to type, then it returns the car and connect it to port
        if (temp->p2car && temp->p2car->portType == type) {
            return temp->p2car;
        }
        temp = temp->next;
    }

    return NULL;//no compatible car inqueue
}