/*
 * MUTEX.S - Mutual Exclusion Handler
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

	.global mutex_lock
	.global mutex_unlock
	.global mutex_init

   	.equ UNLOCKED, 0
   	.equ LOCKED, 1

	.text

/**
 * mutex_init:
 *
 * Grabs memory and initializes the value of the mutex to unlocked.
 *
 */
mutex_init:
	push {r1-r3,lr}
	//Allocate memory for the mutex
   	.lcomm MUTEX 4
   	.align 2

	//Set the value of MUTEX to unlocked initially
	ldr r0, =UNLOCKED
	ldr r1, =MUTEX
	str r0, [r1]
	ldr r0, =MUTEX
	pop {r1-r3,pc}


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
mutex_lock:
	push {r1-r3,lr}
	ldr r1, =LOCKED
mutex_lock_check:
//	ldr r0, =MUTEX //Comment out this line to enable passing in a mutex.
	ldrex r2, [r0]
	//Test if the mutex is locked
	cmp r2, r1
	//if locked, return 0 for failure
	beq mutex_lock_failed
	//Not locked, so lock it
	strex r2, r1, [r0]
	cmp r2, #1 //will return 1 if it failed to store
	//It failed, check again since it only just changed.
	beq mutex_lock_check // If this causes problems, just jump to end.
	//The lock was acquired, execute sequentially and return 1
	mov r0, #1
	dmb
	b mutex_lock_end
mutex_lock_failed:
	mov r0, #0
mutex_lock_end:
	pop {r1-r3,pc}

/* mutex_unlock:
 *
 * Unlocks the mutex, returning in r0:
 * 	1 - the mutex was modified
 * 	0 - the mutex was not modified
 *
 * r0 is the location of the mutex lock.
 *
 */
mutex_unlock:
	push {r1-r3,lr}
	//Load in the unlocked value
	ldr r1, =UNLOCKED
	//Load the address of the mutex
//	ldr r0, =MUTEX //Comment out this line to enable passing in a mutex
	//Execute sequentially (required before strex)
	dmb
	//Unlock the mutex
	str r1, [r0]
	//Signal that the mutex was unlocked
//	sev
	//end
	pop {r1-r3, pc}

/* mutex_was_locked:
 *
 * Checks if the mutex is locked.
 * Called after mutex_locked. Returns:
 * 	1 - the mutex was locked
 * 	0 - the mutex was not locked
 *
 */
mutex_was_locked:
	push {r1-r2,lr}
	//Unlock the mutex to see if it was modified
	bl mutex_unlock
	bl mutex_lock
	//Check if the mutex was modified
	cmp r0, #0
	beq mutex_was_modified_end
	//If the mutex was modified, access it to restore state
	ldr r2,=MUTEX
mutex_was_modified_end:
	pop {r1-r2, pc}
	.end


