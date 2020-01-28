#include <ctype.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "../include/iso6346.h"

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
int iso6346_validate(const char *container_number,
		     const struct iso6346_validate_opts *opts)
{
	assert(container_number != NULL);
	assert(opts != NULL);
	assert(container_number[0] != '\0');

	if (opts->check_length &&
	    strlen(container_number) != ISO6346_CONTAINER_NUM_LEN) {
		return ISO6346_EVALID;
	}

	unsigned short int sum = 0;	// sum used for validation

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
				if (iso6346_category_map[j] != letter) {
					continue;
				}
				has_category = true;
				break;
			}
			if (!has_category) {
				return ISO6346_EVALID;
			}
		}
		// validate recording and transmission accuracies
		// of the owner code and serial number.
		unsigned char letter_code = letter = letter - 'A' + 10;
		for (; letter > 10; letter -= 10) {
			letter_code++;
		}
		sum += letter_code << i;
	}

	/* validate serial number */
	for (unsigned char i = ISO6346_PREFIX_LEN;
	     i < ISO6346_CONTAINER_NUM_LEN - 1; i++) {
		unsigned char number = container_number[i];
		if (!isdigit(number)) {
			return ISO6346_EVALID;
		}
		sum += (number - '0') << i;
	}

	// verify last char is check digit 
	unsigned char check_digit =
	    container_number[ISO6346_CONTAINER_NUM_LEN - 1];
	if (!isdigit(check_digit)) {
		return ISO6346_EVALID;
	}
	check_digit -= '0';
	unsigned char calculated_check_digit = (sum % 11) % 10;

	return check_digit == calculated_check_digit ?
	    ISO6346_OK : ISO6346_EVALID;

}
