/* 
 * File:   simulation.c
 * Author: Siqi Li
 *
 * Created on February 7, 2015, 11:09 PM
 */

#define _BSD_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "customer.h"
#include "mydefs.h"
#include "myutils.h"

static pthread_mutex_t mutex;
static pthread_cond_t cond;

/** The number of served customer at the moment */
static int served_customer_count = 0;

static Customer queue[MAX_LEN];
static int queue_size = 0;
static Customer current_customer;
static Customer next_customer;

static double start_time;

/** Whether the clerk is busy at the moment. */
static volatile int clerk_busy = 0;

static void *simulation_runner(void *param) {
    /*int i;*/
    Customer c = *((Customer *)param);
    if (DEV_MODE) {
        printf("[dev] new thread created for customer %d.\n", c.id);
    }
    /* wait for arrival time */
    usleep(c.arr_time * 1E5);
    pthread_mutex_lock(&mutex);
    /* Add the customer to the queue after arrival time */
    customer_q_push(&queue_size, queue, c);
    /* Sort the queue */
    customer_q_sort(queue_size, queue);
    /*
     * Here, next_customer is the first customer to be served, which 
     * should be the first one in the queue.
     */
    next_customer = queue[0];
    /*
     * If the the customer in this thread is the first customer to be 
     * served, then change current_customer to c. This will only happen 
     * one time in the entire simulation.
     */
    if (!(served_customer_count++) && c.id == next_customer.id) {
        current_customer = c;
    }
    pthread_mutex_unlock(&mutex);
    /* Print the message on arrival */
    printf("Customer %d arrives: arrival time (%.2f), service time (%.2f), "
            "priority (%d).\n", c.id, get_current_time() - start_time,
            ((float)c.ser_time) / 10, c.prio);
    pthread_mutex_lock(&mutex);
    /* If the clerk is busy when a customer arrives, print the wait message */
    if (clerk_busy && c.id != current_customer.id) {
        printf("customer %d waits for the finish of customer %d.\n",
                c.id, current_customer.id);
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&mutex);
    while (clerk_busy || next_customer.id != c.id) {
        pthread_cond_wait(&cond, &mutex);
    }
    /* The wait is over at this point */
    customer_q_remove_by_id(&queue_size, queue, c.id);
    clerk_busy = 1;
    current_customer = c;
    /* Just to be safe. The queue doesn't really need to be sorted here. */
    customer_q_sort(queue_size, queue);
    /* The next customer should be the front of the queue */
    next_customer = queue[0];
    pthread_mutex_unlock(&mutex);
    printf("The clerk starts serving customer %d at time %.2f.\n",
            c.id, get_current_time() - start_time);
    pthread_mutex_lock(&mutex);
    /*
     * According to the sample output, the next for loop doesn't seem 
     * necessary. I think it would be nicer to include it.
     */
    /*for (i = 0; i < queue_size; i++) {
        if (queue[i].id != current_customer.id) {
            printf("customer %d waits for the finish of customer %d.\n",
                    queue[i].id, current_customer.id);
        }
    }*/
    pthread_mutex_unlock(&mutex);
    /* Wait for service time */
    usleep(c.ser_time * 1E5);
    printf("The clerk finishes the service to customer %d at time %.2f.\n",
            c.id, get_current_time() - start_time);
    /* The service is finished, so the clerk is not busy anymore */
    pthread_mutex_lock(&mutex);
    clerk_busy = 0;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
    return NULL;
}

/**
 * Run the simulation.
 * @param customer_count the number of customers in the array.
 * @param customers the array of customers to be simulated.
 */
void simulation_run(int customer_count, Customer *customers) {
    int i;
    pthread_t threads[customer_count];
    if (pthread_mutex_init(&mutex, NULL)) { /* returns 0 on success */
        perror("pthread_mutex_init error");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&cond, NULL)) { /* returns 0 on success */
        perror("pthread_cond_init error");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&mutex);
    start_time = get_current_time();
    current_customer = customer_init(-1, 0, 0, 0);
    pthread_mutex_unlock(&mutex);
    for (i = 0; i < customer_count; i++) {
        if (pthread_create(&threads[i], NULL, simulation_runner,
                (void *)(customers + i))) { /* returns 0 on success */
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < customer_count; i++) {
        pthread_join(threads[i], NULL);
    }
    if (pthread_mutex_destroy(&mutex)) { /* returns 0 on success */
        perror("pthread_mutex_destroy error");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_destroy(&cond)) { /* returns 0 on success */
        perror("pthread_cond_destroy error");
        exit(EXIT_FAILURE);
    }
}
