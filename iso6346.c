#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "iso6346.h"

#define BUFFER_SIZE  (CONTAINER_NUM_LEN + 2)  /* Required length of input 
																								 for reading in from fgets */ 

/*
 * Validates shipping container number.
 * 
 * This function checks if a shipping container number has a valid Check Digit, 
 * a valid Category Identifier, a valid Owner Code, and a valid Serial Number.
 * 
 * WARNING: this function assumes string is null-terminated
 * 
 * Input: null terminated string containing a container number (char *)
 * 
 * Returns: 1 (true) if valid, VALIDATION_ERROR if invalid
 */
int validate(unsigned char *container_number) {
	
	assert(container_number != NULL);			// string exists
	assert(container_number[0] != '\0');	// string not empty

	// copy at most CONTAINER_NUM_LEN chars to avoid buffer overflow
	// and null-terminate
  unsigned char *new_container_num = malloc(sizeof(char) * (CONTAINER_NUM_LEN + 1));
	strncpy(new_container_num, container_number, CONTAINER_NUM_LEN);
	new_container_num[CONTAINER_NUM_LEN] = '\0';

	// strncpy pads dest string with '\0''s until n is reached
	// so we need to still check len of copied string
	if (strlen(new_container_num) != CONTAINER_NUM_LEN) {
		return VALIDATION_ERROR;
	}
	
	/* varify last char is check digit */
	unsigned char check_digit = new_container_num[CONTAINER_NUM_LEN - 1];
	if (!isdigit(check_digit)) {
		return VALIDATION_ERROR;
	}
	
	unsigned short int sum = 0; // sum used for validation
	
	/* check first four chars in container number */
	for (unsigned char i = 0; i < PREFIX_LEN; i++) {
		unsigned char letter = new_container_num[i];
		if (!isalpha(letter)) {
			return VALIDATION_ERROR;
		}
		if (islower(letter)) {
			letter = toupper(letter);
		}
		if (i == (PREFIX_LEN - 1)) {
			
			/* check for valid category identifier */
			bool has_category = false;
			for (int i = 0; i < NUM_CATEGORIES; i++) {
				if (category_map[i] != letter) {
					continue;
				}
				has_category = true;
				break;
			}
			if (!has_category) {
				return VALIDATION_ERROR;
			}
		}

		// validate recording and transmission accuracies
		// of the owner code and serial number.
		check_digit -= '0';
		unsigned short int sum = 0;
		letter = letter - 'A' + 10;
		unsigned char cloned_letter = letter;
		while (cloned_letter > 10) {
			cloned_letter -= 10;
			letter++;
		}
		sum += letter << i;
	}

	/* validate serial number */
	for (unsigned char i = PREFIX_LEN; i < CONTAINER_NUM_LEN - 1; i++) {
		unsigned char number = new_container_num[i];
		if (!isdigit(number)) {
			return VALIDATION_ERROR;
		}
		sum += (number - '0') << i;
	}

	unsigned char calculated_check_digit = (sum % 11) % 10;

	free(new_container_num);
	new_container_num = NULL;
	
  return check_digit == calculated_check_digit;

} /* validate() */

int main(void) {
	
	unsigned char buffer[BUFFER_SIZE];	// padding for '\0'
	unsigned char *trimmed_buffer = malloc(sizeof(char) * (CONTAINER_NUM_LEN + 1));
	
	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
		
		/* remove "\n\0" from string read in */	
		strncpy(trimmed_buffer, buffer, CONTAINER_NUM_LEN); // padding for '\0'
		trimmed_buffer[CONTAINER_NUM_LEN] = '\0';           // null-terminate

		fprintf(process(trimmed_buffer) ? stdout : stderr, "%s", trimmed_buffer);
	}
	free(trimmed_buffer);
	
	return 0;
}
