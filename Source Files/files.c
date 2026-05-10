#define _CRT_SECURE_NO_WARNINGS
#include "../Header FIles/files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header FIles/Station.h"
#include "../Header FIles/car.h"
#include "../Header FIles/carQueue.h"
#include "../Header FIles/helper_functions.h"
#include "../Header FIles/types.h"

#define LINE_SIZE 256
#define CAR_LINE_SIZE 128
#define NAME_SIZE 100 //assuming the max length of a station name is 100
#define NO_LICENSE "-1"


void loadStations(StationBST*  stationTree) {
    FILE* fp = fopen("stations.txt", "r");// open file to read from
    if (fp == NULL) {// if file is doesn't exist return;
        printf("Error opening file\n");
        return;
    }
    char line[LINE_SIZE];
    fgets(line, sizeof(line), fp); // Skip header
    //starting to go over the file.
    while (fgets(line, sizeof(line), fp)) {
        int id, numPorts;
        char name[NAME_SIZE];
        double x,y;
        // reading information in order of what's written into matching variables
        int count = sscanf(line, "%d,%99[^,],%d,%lf,%lf", &id,name,&numPorts, &x, &y);
        //in case it was loading fine we create a station with the information we loaded.
        if (count == 5) {
            Coord coord = {x, y};
            Station* newStation = createStation(id, name, coord);
            //if the creation was smooth we can insert the station to the station tree.
            if (newStation) {
                insertStation(stationTree, newStation);
            }
        }else {
            printf("Parsing error: %s", line);
        }
    }
    fclose(fp); //closing the file.
}


void loadCars(CarBST* CarTree, StationBST* stationTree) {
    FILE* fp = fopen("Cars.txt", "r"); //open file for reading.
    if (fp == NULL) {
        printf("couldn't open Cars.txt\n");
        return;
    }
    char line[LINE_SIZE];
    fgets(line, sizeof(line), fp);// skip header

//starting to go over the file.
    while (fgets(line, sizeof(line), fp)) {
        char license[9];
        char typeStr[9];
        double totalPayed;
        int stId,portNum,inQueue;

        // reading information in order of what's written into matching variables
        int count = sscanf(line, "%[^,],%[^,],%lf,%d,%d,%d",license,typeStr,&totalPayed,&stId,&portNum,&inQueue);

        //if it didn't load all of the information.
        if (count != 6) {
            printf("format error: %s", line);
            continue;
        }

        portType type = portTypeFromStr(typeStr); //converting type string to portType enum.

        //creating a car with the information we loaded.
        Car* newCar = createCar(license,type);
        if (!newCar) continue;

        //initializes rest of the information.
        newCar->totalPayed = totalPayed;
        newCar->inqueue = inQueue;
        newCar->pPort = NULL;

        //inserting the new car to Car tree.
        insertCar(CarTree, newCar);
    }
    fclose(fp);//closing the file
}

void loadPorts(CarBST* CarTree, StationBST* stationTree){
    FILE* fp = fopen("Ports.txt", "r");// opening file for reading
    if (fp == NULL) {
        printf("couldn't open Ports.txt\n");
        return;
    }
    char line[LINE_SIZE];
    fgets(line, sizeof(line), fp);//skip header.
//starting to go over the file.
    while (fgets(line, sizeof(line), fp)){
        int stId,portNum,status;
        char license[9];
        char typeStr[9];
        Date tin = {0};

        // reading information in order of what's written into matching variables
        int count = sscanf(line," %d,%d,%[^,],%d,%d,%d,%d,%d,%d,%8s",&stId,&portNum,typeStr,&status,&tin.Year,&tin.Month,&tin.Day,&tin.Hour,&tin.Min,license);

        //if it didn't load all of the information
        if (count != 10) {
            printf("format error: %s", line);
            continue;
        }
        portType type = portTypeFromStr(typeStr);//converting string type to portType enum.
        if (type == -1){
            printf("invalid port type: %s\n",license);
            continue;
        }

        //creating port with the information we loaded
        Port* newPort = createPort(portNum, type);
        if (!newPort) continue;

        //initializes rest of the information.
        newPort->status = status;
        newPort->tin = tin;

        //if the license we loaded is different from '-1'
        if (strcmp(license,NO_LICENSE) != 0) {
            //searching car and connecting it to port from both sides(port to car, car to port).
            Car* c = searchCar(CarTree, license);
            if (c) {
                newPort->p2car = c;
                c->pPort = newPort;
            }
        }
        //after creating the port we adds it to the station, searching by station id.
        Station* st = searchStationById(stationTree,stId);
        if (st) {
            addPortToStation(st, newPort);// adding port to station...
        }
        else {
            //in case the station was not found, print error and freeing the new port
            printf("Station %d not found\n", stId);
            free(newPort);
        }
    }
    fclose(fp);//closing the file.
}


void loadQueue(StationBST* stationTree, CarBST* CarTree){
    FILE* fp = fopen("LineOfCars.txt", "r"); // open file to read.
    if (fp == NULL) {
        printf("couldn't open LineOfCars.txt\n");
        return;
    }
    char line[LINE_SIZE];
    fgets(line, sizeof(line), fp); //skip header
//starting to go over the file.
    while (fgets(line, sizeof(line), fp)) {
        int stId;
        char license[9];
        //reading information in order of what's written into matching variables.
        int count = sscanf(line, "%8[^,],%d", license, &stId);

        if (count != 2) {
            printf("format error: %s", line);
            continue;
        }
        //looking if the car is in the car tree
        Car* c = searchCar(CarTree, license);
        if (!c) {
            printf("Car %s not found\n", license);
            continue;
        }
        //looking for the station by id that was loaded.
        Station* st = searchStationById(stationTree,stId);
        if (!st) {
            printf("Station %d not found\n", stId);
            continue;
        }
        //enqueueing the car into the station car queue.
        //updating nCars field in the station and that the car is now in queue.
        enqueueCar(st->carQueue, c);
        c->inqueue = 1;
        st->nCars++;

    }
    fclose(fp);
}

//takes all the load function and use them.
void loadFiles(CarBST* CarTree, StationBST* stationTree) {
    loadStations(stationTree);
    loadCars(CarTree, stationTree);
    loadPorts(CarTree, stationTree);
    loadQueue(stationTree, CarTree);
}

static void writeCarToFile(FILE* fp, tCar* node, StationBST* stationTree) {
    if (!node) return;
    //writing recursively to file(car BST).
    writeCarToFile(fp, node->left, stationTree);

    Car* car = node->p2car; //initialize a temporary car for each iteration in the tree.
    int stId = 0;
    int portNum = 0;

    //pPort can be NULL
    if (car->pPort) {
        //if it isn't NULL, we are searching the information by the port the car plug to.
        stId = findStationIdByPort(stationTree, car->pPort);
        portNum = car->pPort->num;
    }
    //check if car is not connected but sits in station queue
    else if (car->inqueue == 1) {
        Station* station = findStationByCarInQueue(stationTree->root, car);
        if (station) {
            stId = station->id;
        }
    }
    //write the information to the file in order.
    fprintf(fp, "%s,%s,%.2lf,%d,%d,%d\n",car->nLicense, portTypeToStr(car->portType),car->totalPayed, stId, portNum,car->inqueue);

    writeCarToFile(fp, node->right, stationTree);
}

void saveCarsToFile(const char* filename, CarBST* CarTree, StationBST* stationTree) {
    FILE* fp = fopen(filename, "w"); //deleting the previous file and opening a new one for read & write.
    if (fp == NULL) {
        printf("couldn't open file %s\n", filename);
        return;
    }
    //writing header.
    fprintf(fp, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");
    writeCarToFile(fp, CarTree->root, stationTree); //writing car information into file.
    fclose(fp);
}

static void writeStationToFile(FILE* fp, Station* node) {
    if (!node) return;
    //going over the station tree recursively
    //and write information to file in every node iteration.
    writeStationToFile(fp, node->left);
    //writing in order of the file.(like loading from...)
    fprintf(fp, "%d,%s,%d,%.2lf,%.2lf\n",node->id, node->name, node->nPorts, node->coord.x, node->coord.y);
    writeStationToFile(fp, node->right);
}

void saveStationsToFile(const char* filename, StationBST* stationTree) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("couldn't open file %s\n", filename);
        return;
    }
    //writing header.
    fprintf(fp, "ID,StationName,NumOfPorts,CoordX,CoordY\n");
    writeStationToFile(fp, stationTree->root);//^
    fclose(fp);
}

static void writePortsToFile(FILE* fp, Station* node) {
    if (!node) return;
    writePortsToFile(fp, node->left);
    //going over the station tree and in every iteration station point to portlist.
    Port* temp =node->portsList->head;
    //running on the linked list portlist.
    while (temp) {
        char license[9] = "-1";
        if (temp->p2car)
            strcpy(license, temp->p2car->nLicense);
        //prints the port information that should be in the txt file.
        fprintf(fp,  "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",node->id,temp->num,portTypeToStr(temp->portType),temp->status
            ,temp->tin.Year,temp->tin.Month,temp->tin.Day,temp->tin.Hour,temp->tin.Min,license);
        temp = temp->next;
    }
    writePortsToFile(fp, node->right);
}

void savePortsToFile(const char* filename, StationBST* stationTree) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("couldn't open file %s\n", filename);
        return;
    }
    // writing header
    fprintf(fp, "StationID,PortNumber,Type,Status,Year,Month,Day,Hour,Min,License\n");
    writePortsToFile(fp, stationTree->root);
    fclose(fp);
}

//same as write ports but just the station points to carQueue field.
static void writeQueuesToFile(FILE* fp, Station* node){
    if (!node) return;
    writeQueuesToFile(fp, node->left);
    carNode* temp =node->carQueue->front;
    //running on the queue and print the information.
    while (temp) {
        fprintf(fp, "%s,%d\n",temp->p2car->nLicense,node->id);
        temp=temp->next;
    }
    writeQueuesToFile(fp, node->right);
}
void saveQueuesToFile(const char* filename, StationBST* stationTree) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("couldn't open file %s\n", filename);
        return;
    }
    //writing header.
    fprintf(fp, "License,StationID\n");
    writeQueuesToFile(fp, stationTree->root);
    fclose(fp);
}
//takes all the update files and use them.
void updateFiles(StationBST* stationTree, CarBST* CarTree) {
    saveStationsToFile("Stations.txt", stationTree);
    saveCarsToFile("Cars.txt",CarTree, stationTree);
    savePortsToFile("Ports.txt", stationTree);
    saveQueuesToFile("LineOfCars.txt", stationTree);
}