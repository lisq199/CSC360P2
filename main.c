/* 
 * File:   main.c
 * Author: Siqi Li
 *
 * Created on February 4, 2015, 3:33 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "customer.h"
#include "mydefs.h"
#include "myutils.h"
#include "simulation2.h"

int main(int argc, char** argv) {
    FILE *fp;
    Customer customers[MAX_LEN];
    int customer_count;
    if (argc < 2) {
        fprintf(stderr, "usage: PQS [filename]\n");
        return (EXIT_SUCCESS);
    }
    /*
     * If there are multiple args, then use the first one and ignore 
     * the rest.
     */
    if (DEV_MODE) {
        printf("[dev] the first arg: %s\n", argv[1]);
    }
    if ((fp = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        return (EXIT_FAILURE);
    }
    /*
     * What the program does is pretty much self-explanatory by the name of 
     * the functions.
     */
    customer_retrieve_from_file(fp, &customer_count, customers);
    if (DEV_MODE) {
        puts("[dev] The original order of customers:");
        print_customers_if_dev(customer_count, customers);
    }
    simulation_run(customer_count, customers);
    fclose(fp);
    return (EXIT_SUCCESS);
}
