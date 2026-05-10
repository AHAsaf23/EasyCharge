#include "../Header FIles/menu.h"
#include <stdio.h>
#include <stdlib.h>

void Menu() {
    printf("--Welcome to EasyCharge management platform--\n");
    printf("=====MainMenu=====\n");
    printf("1.Locate nearest station\n");
    printf("2.Charge car\n");
    printf("3.Check car status\n");
    printf("4.Stop charge\n");
    printf("5.Display all stations\n");
    printf("6.Display cars at station\n");
    printf("7.Report of stations' statistics\n");
    printf("8.Display Top customers\n");
    printf("9.Add new port\n");
    printf("10.Release Charging Ports\n");
    printf("11.Remove out of order ports\n");
    printf("12.Remove customer\n");
    printf("13.Close station\n");
    printf("0.Exit System\n");
    printf("Enter your choice: ");
}

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}