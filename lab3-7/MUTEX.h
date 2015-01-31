/*
 * MUTEX.h - routines for mutual exclusion.
 */
#pragma once

#include "stdint.h"       /* define uint16_t, etc... */

/**
 * mutex_init:
 *
 * Grabs memory and initializes the value of the mutex to unlocked.
 *
 */
uint32_t mutex_init(void);

/* mutex_lock
 *
 * Locks the mutex for use by a subroutine
 *
 * r0 is the location of the mutex lock
 *
 * Returns 1 in r0 if the lock was successful
 * Returns 0 in r0 if the lock was not acquired.
 *
 */
uint32_t mutex_lock(uint32_t mutex);


/* mutex_unlock:
 *
 * Unlocks the mutex, returning in r0:
 * 	1 - the mutex was modified
 * 	0 - the mutex was not modified
 *
 * r0 is the location of the mutex lock.
 *
 */
uint32_t mutex_unlock(uint32_t mutex);
