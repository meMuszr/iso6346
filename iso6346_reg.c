#define PCRE2_CODE_UNIT_WIDTH 8
#include <unistd.h>
#include <pcre2.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

static const unsigned char container_number_length = 11;
static const unsigned char input_length = container_number_length + 2;	// padding for \n\0

int process(unsigned char container_number[container_number_length], pcre2_code * regex,
	    pcre2_match_data * match)
{
	if (container_number[container_number_length] != '\n') {
		return false;
	}
	PCRE2_SPTR subject = (PCRE2_SPTR) container_number;
	int rc =
	    pcre2_match(regex, subject, container_number_length, 0, 0, match,
			NULL);
	if (rc < 0) {
		return false;
	}
	unsigned short int sum = 0;
	const unsigned char letters_length = 4;
	for (unsigned char i = 0; i < letters_length; i++) {
		unsigned char letter = container_number[i];
		letter = letter - (isupper(letter) ? 'A' : 'a') + 10;
		unsigned char cloned_letter = letter;
		while (cloned_letter > 10) {
			cloned_letter -= 10;
			letter++;
		}
		sum += letter << i;
	}
	for (unsigned char i = letters_length; i < container_number_length - 1;
	     i++) {
		unsigned char number = container_number[i];
		sum += (number - '0') << i;
	}
	unsigned char given_check_digit = container_number[10] - '0';
	unsigned char calculated_check_digit = (sum % 11) % 10;
	return given_check_digit == calculated_check_digit;
}

int main(void)
{
	int error_number;
	PCRE2_SIZE error_offset;
	PCRE2_SPTR pattern = (PCRE2_SPTR) "\\w{3}[UuJjZz]\\d{7}";
	pcre2_code *re =
	    pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &error_number,
			  &error_offset, NULL);
	pcre2_match_data *match_data =
	    pcre2_match_data_create_from_pattern(re, NULL);

	unsigned char buffer[input_length];	// padding for NUL char
	while (fgets(buffer, input_length, stdin) != NULL) {
		fprintf(process(buffer, re, match_data) ?
			stdout : stderr, "%s",
			buffer);
	}
	return 0;
}
