/* 
 * File:   customer.c
 * Author: Siqi Li
 *
 * Created on February 4, 2015, 4:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customer.h"
#include "mydefs.h"

/**
 * Print an error message and quit the program.
 * @param attr the name of the attribute of the customer that caused the error
 * @param s the whole string that caused the error
 */
static void customer_error(char *attr, char *s) {
    fprintf(stderr, "Failed to get customer %s in line: %s\n", attr, s);
    exit(EXIT_FAILURE);
}

/**
 * Initialize a customer.
 * @param id the id of the customer.
 * @param arr_time 10 times the arrival time of the customer in seconds.
 * @param ser_time 10 times the service time of the customer in seconds.
 * @param prio priority of the customer.
 * @return 
 */
Customer customer_init(int id, int arr_time, int ser_time, int prio) {
    Customer c;
    c.id = id;
    c.arr_time = arr_time;
    c.ser_time = ser_time;
    c.prio = prio;
    return c;
}

/**
 * Initialize a customer with a string. The string should be in format 
 * "[id]:[arr_time],[ser_time],[priority]", where arr_time is 
 * 10 times the arrival time in seconds, and ser_time is 10 times the service 
 * time in seconds.
 * @param s
 * @return 
 */
Customer customer_init_with_string(char *s) {
    Customer c;
    char *token, line[MAX_LEN];
    /*
     * Construct the delimiter, which includes all the whitespace characters 
     * plus ':' and ','.
     */
    char *delimiter = WHITESPACE":,";
    /* Duplicate the string parameter in case it's not mutable */
    strcpy(line, s);
    if ((token = strtok(line, delimiter)) == NULL) {
        customer_error("id", s);
    }
    c.id = atoi(token);
    if ((token = strtok(NULL, delimiter)) == NULL) {
        customer_error("arrival time", s);
    }
    c.arr_time = atoi(token);
    if ((token = strtok(NULL, delimiter)) == NULL) {
        customer_error("service time", s);
    }
    c.ser_time = atoi(token);
    if ((token = strtok(NULL, delimiter)) == NULL) {
        customer_error("priority", s);
    }
    c.prio = atoi(token);
    return c;
}

/**
 * Get the real arrival time, since the arr_time is 10 times the real 
 * arrival time.
 * @param c
 * @return the real arrival time as a float.
 */
float customer_get_arr_time(Customer c) {
    return ((float)c.arr_time) / 10;
}

/**
 * Get the real service time, since the ser_time is 10 times the real 
 * arrival time.
 * @param c
 * @return the real arrival time as a float.
 */
float customer_get_ser_time(Customer c) {
    return ((float)c.ser_time) / 10;
}

/**
 * Convert a customer to a string. The string does not contain the 
 * customer's id.
 * @param dest the string destination
 * @param c
 */
void customer_to_string_without_id(char *dest, Customer c) {
    sprintf(dest, "arrival time %.1fs, service time %.1fs, priority %d.",
            customer_get_arr_time(c), customer_get_ser_time(c), c.prio);
}

/**
 * Get an array of customers from a file.
 * @param fp
 * @param count the number of customers in the file
 * @param cs an array to store the customers
 */
void customer_retrieve_from_file(FILE *fp, int *count, Customer *cs) {
    char line[MAX_LEN];
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (DEV_MODE) {
            printf("[dev] line %d in file: %s", i, line);
        }
        /* The first line is customer count */
        if (++i == 1) {
            *count = atoi(line);
            if (DEV_MODE) {
                printf("[dev] customer count as int: %d\n", *count);
            }
        } else {
            cs[i - 2] = customer_init_with_string(line);
        }
    }
    if (DEV_MODE) {
        printf("[dev] file has %d lines\n", i);
    }
    /* At this point, i is the number of lines in fp. */
    if (i - 1 != *count) {
        fprintf(stderr, "Incorrect file format\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Compare 2 customers according to the rules.
 * @param c1
 * @param c2
 * @return a positive integer if c1 has higher priority than c2, or a negative 
 * integer if c2 has higher priority than c1. Theoretically it will never 
 * return 0.
 */
int customer_compare(Customer c1, Customer c2) {
    int diff;
    if ((diff = c1.prio - c2.prio) != 0) {
        return diff;
    } else if ((diff = c2.arr_time - c1.arr_time) != 0) {
        return diff;
    } else if ((diff = c2.ser_time - c1.ser_time) != 0) {
        return diff;
    } else if ((diff = c2.id - c1.id) != 0) {
        return diff;
    } else {
        return 0;
    }
}

/**
 * Sort an array of customers based on the rules. Customers with higher 
 * priority are put to the front.
 * @param count the number of customers in the array
 * @param cs the array of customers to be sorted
 */
void customer_q_sort(int count, Customer *cs) {
    int i, finished = 0;
    Customer temp;
    /* Bubble sort */
    while (!finished) {
        finished = 1;
        for (i = 0; i < count - 1; i++) {
            if (customer_compare(cs[i], cs[i + 1]) < 0) {
                temp = cs[i];
                cs[i] = cs[i + 1];
                cs[i + 1] = temp;
                finished = 0;
            }
        }
    }
}

/**
 * Push a new customer onto a customer array that acts as a queue.
 * @param size the number of customers in the original array
 * @param cs the customer array
 * @param c the new customer to be pushed
 */
void customer_q_push(int *size, Customer *cs, Customer c) {
    cs[(*size)++] = c;
}

/**
 * Pop the first customer from a customer array that acts as a queue.
 * @param size the number of customers in the original array
 * @param cs the customer array
 * @return 
 */
Customer customer_q_pop(int *size, Customer *cs) {
    Customer first;
    int i;
    if (*size == 0) {
        fprintf(stderr, "Trying to pop from an empty queue. "
                "Empty customer returned.\n");
        return first;
    }
    (*size)--;
    first = cs[0];
    for (i = 0; i < *size; i++) {
        cs[i] = cs[i + 1];
    }
    return first;
}

/**
 * Remove a customer in an array by its id.
 * @param size The size of the array
 * @param cs The array of customers
 * @param id
 */
void customer_q_remove_by_id(int *size, Customer *cs, int id) {
    int i, found = 0;
    for (i = 0; i < *size; i++) {
        if (cs[i].id == id) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return;
    }
    (*size)--;
    for (; i < *size; i++) {
        cs[i] = cs[i + 1];
    }
}
