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

   	.lcomm MUTEX 4
   	.align 2

   	.equ UNLOCKED, 0
   	.equ LOCKED, 1

	.text

/* mutex_lock
 *
 * Locks the mutex for use by a subroutine
 *
 */
mutex_lock:
	push {r0-r3,lr}
	ldr r1, =LOCKED
mutex_lock_check:
	ldr r0, =MUTEX
	ldrex r2, [r0]
	//Test if the mutex is locked
	cmp r2, r1
	//if locked, wait for unlock
	beq mutex_lock_wait
	//Not locked, so lock it
	strex r2, r1, [r0]
	cmp r2, #1 //will return 1 if it failed to store
	//It failed, try again.
	beq mutex_lock_check
	//The lock was acquired, execute sequentially
	dmb
	b mutex_lock_end
mutex_lock_wait:
	//wait for signal that the mutex was unlocked
	wfe
	//Try to lock out the mutex.
	b mutex_lock_check
mutex_lock_end:
	pop {r0-r3,pc}

/* mutex_unlock:
 *
 * Unlocks the mutex, returning in r0:
 * 	1 - the mutex was modified
 * 	0 - the mutex was not modified
 *
 */
mutex_unlock:
	push {r0-r3,lr}
	//Load in the unlocked value
	ldr r1, =UNLOCKED
	//Load the address of the mutex
	ldr r0, =MUTEX
	//Execute sequentially (required before strex)
	dmb
	//Unlock the mutex
	str r1, [r0]
	//Signal that the mutex was unlocked
	sev
	//end
	pop {r0-r3, pc}

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


