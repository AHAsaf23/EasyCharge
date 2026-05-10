cmake_minimum_required(VERSION 3.20)
project(C_Final_Project)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_Final_Project
        "Source Files/car.c"
        "Header FIles/car.h"
        "Source Files/carQueue.c"
        "Header FIles/carQueue.h"
        "Source Files/date.c"
        "Header FIles/date.h"
        "Source Files/files.c"
        "Header FIles/files.h"
        "Source Files/helper_functions.c"
        "Header FIles/helper_functions.h"
        "Source Files/main.c"
        "Source Files/menu.c"
        "Header FIles/menu.h"
        "Source Files/menu_functions.c"
        "Header FIles/menu_functions.h"
        "Source Files/port.c"
        "Header FIles/port.h"
        "Source Files/Station.c"
        "Header FIles/Station.h"
        "Source Files/types.c"
        "Header FIles/types.h")
