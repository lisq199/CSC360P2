/* 
 * File:   myutils.c
 * Author: Siqi Li
 *
 * Created on February 4, 2015, 4:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "customer.h"
#include "mydefs.h"

/**
 * Print an array of customers if dev mode is enabled.
 * @param count the number of customers to be printed
 * @param cs the array of customers to be printed
 */
void print_customers_if_dev(int count, Customer *cs) {
    int i;
    char s[MAX_LEN];
    if (!DEV_MODE) {
        return;
    }
    printf("[dev] customer count: %d\n", count);
    for (i = 0; i < count; i++) {
        customer_to_string_without_id(s, cs[i]);
        printf("[dev] #%d: id %d, %s\n", i, cs[i].id, s);
    }
}

/**
 * Get the current time as a double
 * @return 
 */
double get_current_time() {
    double t;
    struct timeb now;
    unsigned short millis;
    ftime(&now);
    millis = now.millitm / 100 * 100;
    t = now.time + ((double)millis) / 1000;
    if (DEV_MODE) {
        printf("[dev] current time: %lf\n", t);
    }
    return t;
}
