/* 
 * File:   customer.h
 * Author: Siqi Li
 *
 * Created on February 4, 2015, 4:19 PM
 */

#ifndef CUSTOMER_H
#define	CUSTOMER_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct customer_t {
        int id;
        int arr_time;
        int ser_time;
        int prio;
    } Customer;
    
    Customer customer_init(int id, int arr_time, int ser_time, int prio);
    Customer customer_init_with_string(char *s);
    float customer_get_arr_time(Customer c);
    void customer_to_string_without_id(char *dest, Customer c);
    void customer_retrieve_from_file(FILE *fp, int *count, Customer *cs);
    int customer_compare(Customer c1, Customer c2);
    void customer_q_sort(int count, Customer *cs);
    void customer_q_push(int *size, Customer *cs, Customer c);
    Customer customer_q_pop(int *size, Customer *cs);
    void customer_q_remove_by_id(int *size, Customer *cs, int id);


#ifdef	__cplusplus
}
#endif

#endif	/* CUSTOMER_H */

