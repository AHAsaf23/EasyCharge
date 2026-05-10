#ifndef DATE_H
#define DATE_H

typedef struct {
    int Year;//4 digit year in range 1990-2025
    int Month;// 1-12
    int Day;//1-31 depending on the month
    int Hour;//0-23(24 hours format)
    int Min;//0-59
}Date;

/**
 * Creates and returns a Date object with specified values.
 * no validation performed; use isValidDate to check.
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param min
 * @return
 */
Date createDate(int year, int month, int day, int hour, int min);

/**
 * Returns the current system date and time as a Date struct.
 * @return Date representing the current time
 */
Date getCurrentDate();


/**
 * Converts the date to minutes.
 * @param date The Date to convert to minutes
 * @return The number of minutes
 *
 * This function multiply the: (years, months, days, hours) by the amount of minutes in them
 * and sum up them.
 */
int DateToMinutes(Date date);

#endif //DATE_H