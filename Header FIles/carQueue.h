#ifndef CARQUEUE_H
#define CARQUEUE_H

#include "car.h"
#include "types.h"

struct Car;
typedef struct Car Car;

typedef struct carNode {
    Car* p2car;
    struct carNode* next;
}carNode;

typedef struct {
    carNode* front;
    carNode* rear;
}qCar;

/**
 * Initializes an empty car queue
 * sets both front and rear pointers to NULL
 * @param q Pointer to the car queue to initialize
 */
void initQueue(qCar* q);

/**
 * Inserts a new car at the rear of the queue
 * @param q Pointer to the car queue
 * @param car Pointer to the Car to be added to the queue
 */
void enqueueCar(qCar* q, Car* car);

/**
 * Removes and returns the car at the front of the queue
 * @param q Pointer to the car queue
 * @return Pointer to the Car at the front, or NULL if queue's empty
 */
Car* dequeueCar(qCar* q);

/**
 * check whether the queue's empty
 * @param q Pointer to the car queue
 * @return 1 if the queue is empty, 0 otherwise
 */
int isQueueEmpty(const qCar* q);

/**
 * Prints all cars currently in the queue in order
 * @param q Pointer to the car queue
 */
void printQueueCars(const qCar* q);

/**
 * Returns the position of a specific car in queue(first car is at position 1)
 * @param q Pointer to the car Queue
 * @param car the Pointer to the car for search
 * @return The car's position in the queue, or -1 if not found
 */
int getQueuePosition(const qCar* q, Car* car);

/**
 * Retrieves the next car in the queue that matches the specific type.
 * @param carQueue Ponter to the car queue struct.
 * @param type Desired port type to match
 * @return Pointer to a Car object if found, or NULL if:
 * - the queue is empty
 * - No car in the queue matches the given port type.
 *
 * This function iterated through the queue from front to back,
 * checking each car's port type. If a car matching the desired type
 * is found, a pointer to it is returned, If none match, NULL is returned.
 */
Car* getNextCarInQueue(qCar* carQueue, portType type);

#endif //CARQUEUE_H