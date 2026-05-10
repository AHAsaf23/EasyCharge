#define _CRT_SECURE_NO_WARNINGS
#include "../Header FIles/helper_functions.h"
#include "../Header FIles/Station.h"
#include <stdio.h>
#include <string.h>

#define MAX_ST_NAME 100

typedef Station* (*SearchFunc)(const StationBST*, void*);

Station* searchById(const StationBST* tree, void* data) {
int id = *(int*)data;
    return searchStationById(tree,id);
}
Station* searchByName(const StationBST* tree, void* data) {
    return searchStationByName(tree,(char*)data);
}
Station* searchByCoordinate(const StationBST* tree, void* data) {
    return searchStationByDistance(tree,*(Coord*)data);
}
//array of pointers to search station function.
SearchFunc searchFunctions [3] = {searchById, searchByName, searchByCoordinate};

//function that gets the type of search station and searching in match of it.
Station* searchStation(const StationBST* tree, int searchType, void* data) {
    if (!tree || !data) return NULL;
    if (searchType < 0 || searchType > 2)return NULL;
    return searchFunctions[searchType](tree, data);
}
Station* getStationByIdOrName(StationBST* stationTree) {
    if (!stationTree) {
        displayError(7);
        return NULL;
    }
    int choice;
    Station* station = NULL;
    do {
        printf("Search station by: ID-1, Name- 2? : ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. please enter 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }
        if (choice != 1 && choice != 2)
            printf("Invalid input. please enter 1 or 2.\n");
        } while (choice != 1 && choice != 2);
    do {
        if (choice == 1) {
            int stId;
            printf("Enter station ID or -1 to cancel: ");
            if (scanf("%d", &stId) != 1) {
                printf("Invalid station ID. please enter an ID\n");
                while (getchar() != '\n');
                continue;
            }
            if (stId == -1) return NULL;
            station = searchStation(stationTree, 0, &stId);

        }
        else {
            char name[MAX_ST_NAME];
            printf("Enter station name or exit to cancel: ");
            scanf(" %[^\n]", name);
            if (strcmp(name, "exit") == 0) return NULL;
            station = searchStation(stationTree, 1, name);
        }
        if (!station) {
            displayError(3);
            return NULL;
        }
    } while (!station);

    return station;
}
Station* getStationById(StationBST* stationTree) {
    if (!stationTree) {
        displayError(7);
        return NULL;
    }
    int stId;
    Station* station = NULL;
    do {
        printf("Enter station ID or -1 to cancel: ");
        if (scanf("%d", &stId) != 1) {
            printf("Invalid input. please enter a number\n");
            while (getchar() != '\n');
            continue;
        }
        station = searchStation(stationTree, 0, &stId);
        if (!station) {
            displayError(3);
            return NULL;
        }
    } while (!station);
    return station;
}


//prints errors by code number
void displayError(int errorCode) {

    switch (errorCode) {
        case 1:
            printf("Error: Memory allocation failed.\n");
            break;
        case 2:
            printf("Error: Failed to open file..\n");
            break;
        case 3:
            printf("Error: Station not found.\n");
            break;
        case 4:
            printf("Error: Station cannot be removed (cars charging or in queue).\n");
            break;
        case 5:
            printf("Error: Car not found.\n");
            break;
        case 6:
            printf("Error: Invalid input format.\n");
            break;
        case 7:
            printf("Error: Tree not initialized.\n");
            break;
        case 8:
            printf("Error: Invalid choice.\n");
            break;
        case 9:
            printf("Error: Car already charging.\n");
            break;
        case 10:
            printf("Error: Car already in queue.\n");
            break;
        case 11:
            printf("Error: No available port of required type.\n");
            break;
        case 12:
            printf("Error: Assignment failed - car could not be connected.\n");
            break;
        case 13:
            printf("Error: Car is not currently charging.\n");
            break;
        case 14:
            printf("Error: Invalid port type.\n");
            break;
        case 15:
            printf("Error: Failed to create new port.\n");
            break;
        case 16:
            printf("Error: Failed to exit system properly.\n");
            break;
        case 17:
            printf("Error: Failed to remove port.\n");
            break;
        case 18:
            printf("Error: No out-of-order ports found.\n");
            break;
        case 19:
            printf("Error: License must be exactly 8 digits.\n");
            break;
        default:
            printf("Unknown error.\n");
            break;
    }
}