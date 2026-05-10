#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "../Header FIles/Station.h"
#include "../Header FIles/menu_functions.h"
#include "../Header FIles/car.h"
#include "../Header FIles/helper_functions.h"
#include "../Header FIles/carQueue.h"
#include "../Header FIles/files.h"
#include "../Header FIles/date.h"

#define LICENSE_SIZE 9
#define MAX_ST_NAME 100

//ACTION 1
void locNearSt(const StationBST *stationTree) {
    //Find nearest station by coordinates
    Coord userCoord = {0,0};
    while (1) {
        printf("Enter coordinates(x,y) separated by commas: ");
        if (scanf("%lf,%lf", &userCoord.x, &userCoord.y) == 2) break;
        printf("Invalid input, Please enter two numbers seperated by commas\n");

        while (getchar() != '\n');
    }
    printf("\n");
    Station* nearest = searchStation(stationTree,2,&userCoord);// search station by coordinates.
    if (!nearest) {
        displayError(3);
        return;
    }
    //calculates the distance from input loc to the near st loc
    double distance = calculateDistance(userCoord,nearest->coord);

    //ports details at nearest station.
    int totalPorts = nearest->nPorts;
    int activePorts = 0;
    int busyPorts = 0;
    int slow = 0, mid = 0,fast = 0;
    int waiting = nearest->nCars;

    Port* temp = nearest->portsList->head;
    //counting amount of ports that not faulty and the ports in use.
    while (temp) {
        if (temp->status != OUT_OF_ORDER) {
            activePorts++;
            if (temp->status == OCCUPIED) {
                busyPorts++;
            }
        }
        //counting amount of port of certain type.
        switch (temp->portType) {
            case SLOW: slow++; break;
            case MID: mid++; break;
            case FAST: fast++; break;
            default: break;
        }
        temp = temp->next;
    }

    printf("The nearest station: %s\n",nearest->name);
    printf("Distance: %0.2f\n",distance);
    printf("Total ports: %d\n",totalPorts);
    printf("Active ports: %d\n", activePorts);
    printf("Busy ports (with car): %d\n", busyPorts);
    printf("Cars waiting in queue: %d\n", waiting);
    printf("Port types:\n");
    printf("  SLOW: %d\n", slow);
    printf("  MID : %d\n", mid);
    printf("  FAST: %d\n", fast);
}
//Action 2
void chargeCar(StationBST* stationTree, CarBST* carTree) {
    if (!stationTree || !carTree) {
        displayError(7);
        return;
    }
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;

    //input of car license
    char license[50];
    Car* car = NULL;

    while (!car) {
        printf("Enter license(8 characters) or 'exit' to cancel: ");
        scanf(" %49s", license);
        if (strcmp(license, "exit") == 0) return;

        if (strlen(license) != 8) {
            displayError(19);
            continue;
        }
        //checks if the license inputs is valid and only digits.
        int valid = 1;
        for (int i = 0; i < strlen(license); i++) {
            if (!isdigit(license[i])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            displayError(19);
            continue;
        }

        car = searchCar(carTree,license);
        if (!car) {
            printf("Car %s not found. Please enter its details:\n",license);

            char typeStr[10];
            portType type;
            //creation of car that's not exists
            do {
                printf("Enter port type(SLOW/MID/FAST): ");
                scanf("%9s", typeStr);
                type = portTypeFromStr(typeStr);
                if (type == -1)
                    printf("Invalid port type. Try again.\n");
            }while (type == -1);
            car = createCar(license,type);
            if (!car) {
                printf("Failed to create car. Try again\n");
                continue;
            }
            insertCar(carTree,car);
            printf("Car %s added to system.\n", license);
        }
    }
    //if car isn't connected to port.
    if (car->pPort) {
        displayError(9);
        return;
    }
    //if car is in queue.
    if (car->inqueue == 1) {
        displayError(10);
        return;
    }

    //if available port isn't found in station
    Port* port = findAvailablePortInStation(station,car->portType);
    if (!port) {
        displayError(11);
        enqueueCar(station->carQueue,car);
        car->inqueue = 1;
        station->nCars++;
        printf("Car %s has entered the queue at station %s.\n",car->nLicense,station->name);
        return;
    }
    //assign car to port and updating time of charging.
    if (assignCar2port(car, port)) {
        port->tin = getCurrentDate();
        printf("Car %s is now charging at port %d, in station %s.\n",car->nLicense,port->num,station->name);
        return;
    }else {
        displayError(12);
    }
}
//Action 3
void checkCarStatus(StationBST* stationTree, CarBST* carTree) {
    if (!stationTree || !carTree) {
        displayError(7);
        return;
    }
    char license[LICENSE_SIZE];
    printf("Enter license(8 characters) or 'exit' to cancel: ");
    scanf(" %8s", license);
    if (strcmp(license, "exit") == 0) return;
    Car* car = searchCar(carTree,license);
    if (!car) {
        displayError(5);
        return;
    }

    int stId = -1;
    //in case car is connected to port.
    if (car->pPort) {
        stId = findStationIdByPort(stationTree,car->pPort);
        Station* station = searchStation(stationTree,0,&stId);
        if (!station) {
            printf("Car is charging, but station data is missing.\n");
            return;
        }
        //calculates the charge time in minutes.
        Date now = getCurrentDate();
        int totalMinutes = totalChargeMinutes(car,now);
        if (totalMinutes < 0) totalMinutes = 0;

        printf("Car %s is charging at station \"%s\", port #%d.\n",car->nLicense,station->name, car->pPort->num);
        printf("Charging time: %d minutes.\n",totalMinutes);
        return;
    }

    //handles the situation when the car is in queue.
    if (car->inqueue == 1){
        Station* QueueStation = findStationByCarInQueue(stationTree->root,car);
        if (!QueueStation) {
            printf("Car %s marked in queue but Station was not found.\n", license);
            return;
        }
        int pos = getQueuePosition(QueueStation->carQueue, car);
        if (pos != -1) {
            printf("Car %s is in queue at station \"%s\"\n",car->nLicense,QueueStation->name);
            printf("Position in %s charging queue: %d.\n", portTypeToStr(car->portType), pos);
        }else {
            printf("Car %s marked in queue but not found in station queue.\n",car->nLicense);
        }
        return;
    }
    printf("Car %s is neither charging nor in queue.\n",car->nLicense);
}
//Action 4
void stopCharge(StationBST* stationTree, CarBST* carTree) {
    if (!stationTree || !carTree) {
        displayError(7);
        return;
    }
    char license[LICENSE_SIZE];
    printf("Enter license(8 characters) or 'exit' to cancel: ");
    scanf(" %8s", license);
    if (strcmp(license, "exit") == 0) return;
    Car* car = searchCar(carTree,license);
    if (!car) {
        displayError(5);
        return;
    }
    if (!car->pPort) {
        displayError(13);
        return;
    }
    //searching for the station the car has found at.
    int stId = findStationIdByPort(stationTree,car->pPort);
    Station* station = searchStation(stationTree,0,&stId);
    if (!station) {
        displayError(3);
        return;
    }
    Date now = getCurrentDate();
    //calculates total time car is charged in minutes.
    int totalMinutes = totalChargeMinutes(car, now);
    if (totalMinutes < 0) totalMinutes = 0;

    //determines the payment based on the time.
    double price = totalMinutes * 1.2;
    car->totalPayed += price;

    printf("Car %s finished charging at station \"%s\", port #%d.\n", car->nLicense,station->name, car->pPort->num);
    printf("Charging time: %d minutes. price to pay: %.2lf ILS.\n",totalMinutes,price);

    //disconnecting car from port
    car->pPort->tin = createDate(0,0,0,0,0);
    car->pPort->status = FREE;
    car->pPort->p2car = NULL;
    car->pPort = NULL;
    station->nCars--;

    //handles if there's a matching car to the port type in queue.
    Car* nextCar = getNextCarInQueue(station->carQueue, car->portType);
    if (nextCar) {
        Port* port = findAvailablePort(station->portsList, nextCar->portType);
        if (port) {
            nextCar->pPort = port;
            port->p2car = nextCar;
            port->status = OCCUPIED;

            nextCar->pPort->tin = getCurrentDate();
            nextCar->inqueue = 0;
            dequeueCar(station->carQueue);

            printf("Car %s moved from queue to charging at port #%d (%s).\n",nextCar->nLicense,port->num,portTypeToStr(nextCar->portType));
        }else {
            printf("No available port for next car in queue.\n");
        }
    }else {
        printf("No car in queue for port type %s.\n", portTypeToStr(car->portType));
    }
}
//Action 5
void dispAllSt(StationBST* stationTree) {
    //printing station in order with summary details.
    inorderPrintStations(stationTree);
}

//Action 6
void dispCarsAtSt(StationBST* stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;

    Date now = getCurrentDate();
    Port* temp = station->portsList->head;

    printf("Cars at station \"%s\": \n",station->name);
    printf("----------------------------------------\n");
    printf("InCharge Cars:\n");
    printf("----------------------------------------\n");
    while (temp) {
        if (temp->status == OCCUPIED && temp->p2car) {
            //check if theirs a car connected to the port and check if the status matches.
            Car* car = temp->p2car;
            int minutes = DateToMinutes(now) - DateToMinutes(temp->tin);
            if (minutes < 0) minutes = 0;

            printf("License: %s ,", car->nLicense);
            printf("Type: %s\n",portTypeToStr(car->portType));
            printf("Charging Time: %d minutes\n", minutes);
        }
        temp = temp->next;
    }
    printf("----------------------------------------\n");
    printQueueCars(station->carQueue);
    printf("\n");
}

//Action 7
void reportStStat(StationBST* stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;

    int totalPorts = station->nPorts;
    int outOfOrderPorts = 0;
    int occupiedPorts = 0;

    Port* temp = station->portsList->head;
    while (temp) {
        if (temp->status == OCCUPIED)
            occupiedPorts++;
        if (temp->status == OUT_OF_ORDER)
            outOfOrderPorts++;
        temp = temp->next;
    }

    double utilizationRate = 0.0, DefectiveRatio = 0.0, loadRatio = 0.0;

    //calculates the ports that in use and the faulty ones in precentages.
    if (totalPorts > 0) {
        utilizationRate = 100.0 * occupiedPorts / totalPorts;

        DefectiveRatio = 100.0 *  outOfOrderPorts / totalPorts;
    }
    printf("\n--- Station Statistics Report for \"%s\" ---\n", station->name);
    printf("Utilization Rate: %.2f%%\n", utilizationRate);
    printf("Faulty Ports Ratio: %.2f%%\n", DefectiveRatio);

    if (station->nCars > 0) {
        loadRatio = (double)occupiedPorts / station->nCars;
    }
    //classified the load levels.
    printf("Load Level: ");
    if (station->nCars == 0)
        printf("No Load\n");
    else if (loadRatio > 1.0)
        printf("Very Heavy Load\n");
    else if (loadRatio >= 0.9)
        printf("Heavy Load\n");
    else if (loadRatio > 0.2)
        printf("Balanced Load\n");
    else
        printf("Very Light Load\n");
}
//Action 8
void dispTopCustomers(const CarBST* CarTree) {
    if (!CarTree || !CarTree->root) return;
//counts recursively the car nodes in the tree.
    int Num_Of_Cars = countCars(CarTree);
    if (Num_Of_Cars == 0)
        return;

    int index = 0;
    //allocate memory for array pointers that will point at the cars.
    Car** topCars = (Car**)malloc(Num_Of_Cars *(sizeof(Car*)));
    if (topCars == NULL) {
        displayError(1);
        return;
    }

    saveCarsInOrder(CarTree->root, topCars, &index);//saving cars to the array.
    qsort(topCars, Num_Of_Cars, sizeof(Car*), compareByPayed);// sorting by total payments.
    printf("top 5 customers with highest total Payment:\n");
    //printing the top 5 cars with most total payments.
    int top = Num_Of_Cars < 5 ? Num_Of_Cars : 5;
    for (int i = 0; i < top; i++) {
        printf("%d. ", i + 1);
        printCar(topCars[i]);
        printf("\n");
    }
    free(topCars); //freeing the pointers array memory.
}

//Action 9
void addNewPort(StationBST* stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;

    int portType;
    printf("Enter port type FAST - 0,MID - 1,SLOW - 2 : ");
    scanf("%d", &portType);
    if (portType != FAST && portType != MID && portType != SLOW) {
        displayError(14);
        return;
    }
    Port* newPort = createPort(station->nPorts + 1,portType);
    if (!newPort) {
        displayError(15);
        return;
    }
    addPortToStation(station,newPort);//adding port to station.

    carNode* curr = station->carQueue->front;
    carNode* prev = NULL;

    //handles the case of connecting cars in line to available ports
    while (curr) {
        if (curr->p2car->portType == newPort->portType) {
            if (prev == NULL) {
                station->carQueue->front = curr->next;
                if (station->carQueue->rear == curr)
                    station->carQueue->rear = NULL;
            } else {
                prev->next = curr->next;
                if (station->carQueue->rear == curr)
                    station->carQueue->rear = prev;
            }

            Car* matchingCar = curr->p2car;
            assignCar2port(matchingCar, newPort);

            free(curr);
            printf("Car %s from queue now charging at port %d (%s).\n",
                   matchingCar->nLicense, newPort->num, portTypeToStr(newPort->portType));
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("No car in queue requires a %s port right now.\n", portTypeToStr(newPort->portType));
}

//Action 10
void releasePorts(StationBST* stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;
    Date now = getCurrentDate();
    Port* port = station->portsList->head;
    int disconnected = 0;
    int connected = 0;

    while (port) {
        if (port->status == OCCUPIED && port->p2car) {
            int minutes = DateToMinutes(now) - DateToMinutes(port->tin);
            if (minutes >= 600) {
                Car* car = port->p2car;
                printf("Car \"%s\" disconnected. charging time: %d minutes)\n", car->nLicense, minutes);

                //disconnecting car from port
                car->pPort = NULL;
                port->p2car = NULL;
                port->tin = createDate(0, 0, 0, 0, 0);
                port->status = FREE;
                disconnected++;
                //trying to move the line
                Car* nextCar = dequeueCar(station->carQueue);
                if (nextCar) {
                    port->p2car = nextCar;
                    port->tin = now;
                    port->status = OCCUPIED;
                    nextCar->pPort = port;
                    nextCar->inqueue = 0;
                    connected++;
                }
            }
        }
        port = port->next;
    }
    printf("\n");
    printf("Connected and disconnected cars in \"%s\":\n",station->name);
    printf("%d disconnected cars that is been charge more than 10 hours.\n",disconnected);
    printf("%d connected cars from queue.\n",connected);
}

//Action 11
void remOutOrderPort(StationBST* stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;

    int stId = station->id;
    Port* port = station->portsList->head;
    int port_num = 0;
    printf("Out of order ports at station #%d:\n", stId);

    int out_of_order = 0;
    //counts and prints out of order  ports
    while (port) {
        if (port->status == OUT_OF_ORDER) {
            out_of_order++;
            printf("port #%d, type: %s\n", port->num, portTypeToStr(port->portType));
        }
        port = port->next;
    }
    if (out_of_order == 0) {
        displayError(18);
        return;
    }

    //creating dynamic array for invalid ports numbers.
    int index = 0;
    int* validPorts = (int*)malloc(sizeof(int) * out_of_order);
    if (validPorts == NULL) {
        displayError(1);
        return;
    }
    //adding the out of order ports numbers.
    port = station->portsList->head;
    while (port) {
        if (port->status == OUT_OF_ORDER) {
            validPorts[index++] = port->num;
        }
        port = port->next;
    }
    //loop for deleting the port.
    while (true) {
        printf("\nWhich port would you like to delete?\n");
        printf("(Enter port number, or -1 to cancel): ");
        scanf("%d", &port_num);

        if (port_num == -1) {
            printf("Operation cancelled. Returning to menu.\n");
            break;
        }

        //verify the selection
        int isValid = 0;
        for (int i = 0; i < out_of_order; i++) {
            if (validPorts[i] == port_num) {
                isValid = 1;
                break;
            }
        }

        //if the input is invalid asks for another chance.
        if (!isValid) {
            printf("Port #%d is not listed as out-of-order.\n", port_num);
            printf("Would you like to try again? (1 = YES / 0 = NO): ");
            int retry;
            scanf("%d", &retry);
            if (retry != 1) {
                printf("Returning to menu.\n");
                break;
            }
            continue;
        }

        if (removePortFromStation(station, port_num)) {
            printf("Port #%d was successfully removed\n", port_num);
            break;
        }else {
            displayError(17);
            printf("Would you like to try again? (1 = YES / 0 = NO): ");
            int retry;
            scanf("%d", &retry);
            if (retry != 1) {
                printf("Returning to menu.\n");
                break;
            }
        }
    }
    //free dynamic array memory.
    free(validPorts);
}

//Action 12
void remCustomer(CarBST* CarTree) {
    if (!CarTree){
        displayError(7);
        return;
}
    char license[LICENSE_SIZE];
    printf("Enter license(8 characters) or 'exit' to cancel: ");
    scanf(" %8s", license);

    if (strcmp(license, "exit") == 0) {
        printf("returning to menu\n\n");
        return;
    }
    //saving the amount of cars before removing
    int before = countCarNodes(CarTree->root);
    //trying to remove the car based on license.
    CarTree->root = removeCarNode(CarTree->root, license);
    //check if car removed from the tree.
    int after = countCarNodes(CarTree->root);

    //if there's less cars the removal worked.
    if (after < before) {
        CarTree->totalCars--;
        printf("Car %s was successfully removed\n", license);
    }
}

//Action 13
void closeSt(StationBST *stationTree) {
    Station* station = getStationByIdOrName(stationTree);
    if (!station) return;
    
    int stId = station->id;
    //check if the cars in queue or in charge process.
    if (station->nCars > 0 || countCarsChargingInStation(station) > 0) {
        displayError(4);
        return;
    }
    //saving the name of the station before removal.
    char closedName[MAX_ST_NAME];
    strcpy(closedName, station->name);

    //removing the station from tree.
    stationTree->root = removeStationNode(stationTree->root, stId);
    printf("Station \"%s\" (ID %d) was successfully closed.\n", closedName, stId);
}

//Action 0
void exitSys(StationBST* stationTree,CarBST* carTree) {
    if (!stationTree || !carTree) {
        displayError(7);
        return;
    }
    //update files and free the memory in the needed order.
    updateFiles(stationTree,carTree);
    freeStationBst(stationTree);
    freeCarsTree(carTree);

    printf("System closed successfully.\n");
}