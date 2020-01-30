#include <ctype.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../include/iso6346.h"

static unsigned char iso6346_calculate_check_digit(const char *container_number)
{
	unsigned short int sum = 0;	// sum used for validation
	for (unsigned char i = 0; i < ISO6346_PREFIX_LEN; i++) {
		unsigned char letter, letter_code;
		letter = letter_code = toupper(container_number[i]) - 'A' + 10;
		for (; letter > 10; letter -= 10) {
			letter_code++;
		}
		sum += letter_code << i;
	}
	for (unsigned char i = ISO6346_PREFIX_LEN;
	     i < ISO6346_CONTAINER_NUM_LEN - 1; i++) {
		sum += (container_number[i] - '0') << i;
	}
	return (sum % 11) % 10;
}

void iso6346_generate_random(char *container_number)
{
	for (unsigned char i = 0; i < ISO6346_OWNER_CODE_LEN; i++) {
		unsigned char letter = rand() % 26 + 65;
		container_number[i] = letter;
	}

	container_number[ISO6346_PREFIX_LEN - 1] =
	    iso6346_category_id_map[rand() % 3];

	for (unsigned char i = ISO6346_PREFIX_LEN;
	     i < ISO6346_CONTAINER_NUM_LEN - 1; i++) {
		unsigned char digit = rand() % 10 + 48;
		container_number[i] = digit;
	}
	container_number[ISO6346_CONTAINER_NUM_LEN - 1] =
	    iso6346_calculate_check_digit(container_number) + 48;
}

/*
 * Validates if a shipping container number is ISO6346 compliant.
 * 
 * This function checks if a shipping container number has a valid check digit, 
 * a valid category identifier, a valid owner code, and a valid serial number.
 * 
 * container_number - string containing a container number 
 * opts - options to supply to fine-tune the validation logic 
 * 
 * 
 * Returns: 0 (ISO6346_OK) if valid, non-zero (ISO6346_EVALID) if invalid
 */
unsigned char iso6346_validate(const char *container_number,
		     const struct iso6346_validate_opts *opts)
{
	assert(container_number != NULL);
	assert(opts != NULL);
	assert(container_number[0] != '\0');
	if (opts->check_length &&
	    strlen(container_number) != ISO6346_CONTAINER_NUM_LEN) {
		return ISO6346_EVALID;
	}
	// check first four chars in container number 
	for (unsigned char i = 0; i < ISO6346_PREFIX_LEN; i++) {
		unsigned char letter = container_number[i];
		if (!isalpha(letter)) {
			return ISO6346_EVALID;
		}
		if (islower(letter)) {
			if (opts->case_sensitive) {
				return ISO6346_EVALID;
			}
			letter = toupper(letter);
		}
		if (i == (ISO6346_PREFIX_LEN - 1)) {

			/* check for valid category identifier */
			bool has_category = false;
			for (unsigned char j = 0; j < ISO6346_NUM_CATEGORIES;
			     j++) {
				if (iso6346_category_id_map[j] == letter) {
					has_category = true;
					break;
				}
			}
			if (!has_category) {
				return ISO6346_EVALID;
			}
		}
	}

	/* validate serial number */
	for (unsigned char i = ISO6346_PREFIX_LEN;
	     i < ISO6346_CONTAINER_NUM_LEN - 1; i++) {
		unsigned char number = container_number[i];
		if (!isdigit(number)) {
			return ISO6346_EVALID;
		}
	}

	// verify last char is check digit 
	unsigned char check_digit =
	    container_number[ISO6346_CONTAINER_NUM_LEN - 1];
	if (!isdigit(check_digit)) {
		return ISO6346_EVALID;
	}
	check_digit -= '0';
	return check_digit == iso6346_calculate_check_digit(container_number) ?
	    ISO6346_OK : ISO6346_EVALID;
}
