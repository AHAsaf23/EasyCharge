#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Header FIles/car.h"
#include "../Header FIles/Station.h"
#include "../Header FIles/files.h"
#include "../Header FIles/menu.h"
#include "../Header FIles/menu_functions.h"

#define EXIT 0

int main() {
	int User_Choice;
	int result = 0;
	StationBST stationTree;
	initStationBst(&stationTree); // intializes station bst
	CarBST* carTree = createCarBST();  //creating car BST
	loadFiles(carTree, &stationTree);
	do {
		Menu();
		result = scanf("%d", &User_Choice);
		if (result != 1) {
			printf("Invalid Choice, Please enter a valid number.\n\n");
			while (getchar() != '\n')continue;
			continue;
		}
		clear();
		switch (User_Choice) {
		case 1: {
			locNearSt(&stationTree);
			break;
		}
		case 2: {
			chargeCar(&stationTree, carTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 3: {
			checkCarStatus(&stationTree, carTree);
			break;
		}
		case 4: {
			stopCharge(&stationTree, carTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 5: {
			dispAllSt(&stationTree);
			break;
		}
		case 6: {
			dispCarsAtSt(&stationTree);
			break;
		}
		case 7: {
			reportStStat(&stationTree);
			break;
		}
		case 8: {
			dispTopCustomers(carTree);
			break;
		}
		case 9: {
			addNewPort(&stationTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 10: {
			releasePorts(&stationTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 11: {
			remOutOrderPort(&stationTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 12: {
			remCustomer(carTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 13: {
			closeSt(&stationTree);
			updateFiles(&stationTree, carTree);
			break;
		}
		case 0: {
			exitSys(&stationTree, carTree);
		}
		default: {
			break;
		}
		}
		printf("\n");
	} while (User_Choice != EXIT);
	return 0;
}
