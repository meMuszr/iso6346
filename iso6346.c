#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

static const unsigned char container_number_length = 11;
static const unsigned char input_length = container_number_length + 2;	// padding for \n\0
static const unsigned char category_ids[3] = { 'U', 'J', 'Z' };

int process(unsigned char container_number[container_number_length])
{
	if (container_number[container_number_length] != '\n') {
		return false;
	}
	unsigned char given_check_digit = container_number[10];
	if (!isdigit(given_check_digit)) {
		return false;
	}
	given_check_digit -= '0';
	unsigned short int sum = 0;
	const unsigned char letters_length = 4;
	for (unsigned char i = 0; i < letters_length; i++) {
		unsigned char letter = container_number[i];
		if (!isalpha(letter)) {
			return false;
		}
		if (islower(letter)) {
			letter = toupper(letter);
		}
		if (i == letters_length) {
			bool has_category = false;
			for (int i = 0; i < 3; i++) {
				if (category_ids[i] != letter) {
					continue;
				}
				has_category = true;
				break;
			}
			if (!has_category) {
				return false;
			}
		}

		letter = letter - 'A' + 10;
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
		if (!isdigit(number)) {
			return false;
		}
		sum += (number - '0') << i;
	}
	unsigned char calculated_check_digit = (sum % 11) % 10;
	return given_check_digit == calculated_check_digit;
}

int main(void)
{
	unsigned char buffer[input_length];	// padding for NUL char
	while (fgets(buffer, input_length, stdin) != NULL) {
		fprintf(process(buffer) ? stdout : stderr, "%s", buffer);
	}
	return 0;
}
