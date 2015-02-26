/*
 * string.c - routines for use with strings.
 */

#include "string.h"

/**
 * uint8_t strcmp(uint8_t *str1, uint8_t *str2):
 *
 * str1 - string to compare
 * str2 - string to compare
 *
 * returns 1 if str1 == str2, 0 if not.
 */
uint8_t strcmp(uint8_t *str1, uint8_t *str2){
	uint8_t match = 1;
	while( 1 ){
		//Check if *str1 and *str2 don't match or are null
		if(*str1 != *str2){
			return 0;
		}
		else if(*str1 == '\0' || *str2 == '\0'){
			if(*str1 == '\0' && *str2 == '\0'){
				return 1;
			}
			else{
				return 0;
			}
		}
		//Since *str1 and *str2 are both the same and non-null
		//move to the next characters
		str1++;
		str2++;
	}
}

/*
 * Converts integer i to string representation
 * i - integer value
 */
uint8_t* i2a(uint32_t i){
	uint8_t a;
	uint8_t *ptr;
	ptr = &a;

	//Handle the edge case 0
	if(i == 0){
		ptr = "0\0";
		return ptr;
	}

	uint32_t temp = i;
	//Shift pointer to the end of the string
	while(temp>0){
		temp = temp / 10;
		ptr++;
	}

	//Null-terminate the string
//	ptr++;
	*ptr = '\0';
//	ptr--;

	//Fill a with the correct values
	while(i > 0){
		ptr--;
		*ptr = (i % 10) + 48;
		i = i / 10;
//		ptr--;
 	}

	return ptr;
}

/**
 * Returns the number of bytes of the given null-terminated string
 */
uint32_t length(uint8_t *str){
	uint32_t counter = 0;
	while(str != '\0'){
		counter ++;
		str++;
		//Handle errors of non-null-terminated strings
		if(counter >= 4294967296-2){
			return 0;
		}
	}
	return counter;
}

