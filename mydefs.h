/* 
 * File:   mydefs.h
 * Author: Siqi Li
 *
 * Created on February 4, 2015, 4:14 PM
 */

#ifndef MYDEFS_H
#define	MYDEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * flag for dev mode. If dev mode is enabled (1), a lot of logs 
 * starting with "[dev]" will be printed to stdout.
 */
#define DEV_MODE 0
/** standard max length for this project. */
#define MAX_LEN 1024
/** a string containing all the whitespace characters */
#define WHITESPACE " \t\n\v\f\r"


#ifdef	__cplusplus
}
#endif

#endif	/* MYDEFS_H */

