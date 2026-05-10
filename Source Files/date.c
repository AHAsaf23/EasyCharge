#include "../Header FIles/date.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

Date createDate(int year, int month, int day, int hour, int min) {
    Date date;
    date.Year = year;
    date.Month = month;
    date.Day = day;
    date.Hour = hour;
    date.Min = min;
    return date;
}

Date getCurrentDate() {
    Date date;
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    date.Year = tm_info->tm_year + 1900;
    date.Month = tm_info->tm_mon + 1;
    date.Day = tm_info->tm_mday;
    date.Hour = tm_info->tm_hour;
    date.Min = tm_info->tm_min;

    return date;
}

int DateToMinutes(Date date) {
    int years_to_minutes = date.Year * 525600; //multiply years by minutes in a year
    int months_to_minutes = date.Month * 43800; //multiply months by minutes in a month
    int days_to_minutes = date.Day * 1440; // multiply days by minutes in a day
    int hours_to_minutes = date.Hour * 60; // multiply hours by minutes in an hour
    return years_to_minutes + months_to_minutes + days_to_minutes + hours_to_minutes + date.Min;
}
