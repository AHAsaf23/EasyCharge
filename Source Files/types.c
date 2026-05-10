#include <stdio.h>
#include <stdlib.h>
#include "../Header FIles/types.h"
#include <string.h>

const char* portTypeToStr(enum portType type) {
    switch (type) {
        case FAST:
            return "FAST";

        case MID:
            return "MID";

        case SLOW:
            return "SLOW";
        default:
            return "Unknown Type";
    }
}

portType portTypeFromStr(const char* str) {
    if (strcmp(str, "FAST") == 0) return FAST;
    if (strcmp(str, "MID") == 0) return MID;
    if (strcmp(str, "SLOW") == 0) return SLOW;
    return -1;
}


const char* portStatusToStr(enum portStatus status) {
    switch (status) {
        case OCCUPIED:
            return "OCCUPIED";

        case FREE:
            return "FREE";

        case OUT_OF_ORDER:
            return "OUT_OF_ORDER";
        default:
            return "Unknown Type";
    }
}