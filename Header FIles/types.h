#ifndef TYPES_H
#define TYPES_H

typedef enum portType {
    FAST,
    MID,
    SLOW
}portType;

typedef enum portStatus{
    OCCUPIED = 1,
    FREE = 2,
    OUT_OF_ORDER =3
}portStatus;

const char* portTypeToStr(enum portType type);
const char* portStatusToStr(enum portStatus status);

portType portTypeFromStr(const char* str);

#endif //TYPES_H