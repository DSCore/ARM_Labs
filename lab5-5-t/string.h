#pragma once

#include "stdint.h"


/**
 * uint8_t strcmp(uint8_t *str1, uint8_t *str2):
 *
 * str1 - string to compare
 * str2 - string to compare
 *
 * returns 1 if str1 == str2, 0 if not.
 */
uint8_t strcmp(uint8_t *str1, uint8_t *str2);

/*
 * Converts integer i to string representation
 * i - integer value
 */
uint8_t* i2a(uint32_t i);
