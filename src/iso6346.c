#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

#include "../include/iso6346.h"

#define PROGRAM_NAME "iso6346"

#define AUTHORS \
  "Mustafa Abdul-Kader"

#define BUFFER_SIZE  (ISO6346_CONTAINER_NUM_LEN + 2)	/* Required length of input 
							   for reading in from fgets */

static int check_length_flag = 0, truncate_length_flag = 0,
    case_sensitive_flag = 0, generate_random_flag = 0, help_flag = 0;

static const struct option long_options[] = {
	{"check-length", no_argument, &check_length_flag, true},
	{"truncate-length", no_argument, &truncate_length_flag, true},
	{"case-sensitive", no_argument, &case_sensitive_flag, true},
	{"generate-random", required_argument, &generate_random_flag, true},
	{"help", no_argument, &help_flag, true},
	{0, 0, 0, 0}
};

int main(int argc, char **argv)
{
	int c;
	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "lstg:h", long_options,
				&option_index);
		if (c < 0) {
			break;
		}
		switch (c) {
		case 0:
			break;
		case 'l':
			check_length_flag = true;
			break;
		case 's':
			case_sensitive_flag = true;
			break;
		case 't':
			truncate_length_flag = true;
			break;
		case 'g':
			generate_random_flag = true;
			break;
		case 'h':
			help_flag = true;
			break;
		default:	/* '?' */
			fprintf(stderr,
				"Try 'iso6346 --help' for more information.\n");
			exit(EXIT_FAILURE);
		}
		if (help_flag) {
			printf("\
Usage: iso6346 [OPTION] <<< [CONTAINER_NUMBER]\n\
Prints supplied container nubmers to standard output if they are ISO6346 compliant, otherwise to standard error.\n\n  \
-l, --check-length	  Require the string is a valid length for a container number\n  \
-t, --truncate-length	  Truncate the string to the valid length for a container number\n  \
-s, --case-sensitive	  Validate the string with case sensitivity (fails validation on lowercase letters in prefix)\n  \
-g, --generate-random AMOUNT\n\
			  Generates the specified amount of valid container numbers and prints to standard output\n  \
-h, --help		  Print help dialog\n");

			exit(EXIT_SUCCESS);
		}
		if (generate_random_flag) {
			int amount = atoi(optarg);
			char container_number[12];
			for (int i = 0; i < amount; i++) {
				iso6346_generate_random(container_number);
				printf("%s\n", container_number);

			}
			exit(EXIT_SUCCESS);

		}
	}
	struct iso6346_validate_opts opts = {
		.check_length = check_length_flag,
		.case_sensitive = case_sensitive_flag
	};
	ssize_t nread;
	size_t len = 0;
	char *line = NULL;
	while ((nread = getline(&line, &len, stdin)) != -1) {
		if (truncate_length_flag) {
			memset(line + ISO6346_CONTAINER_NUM_LEN, '\0',
			       len - ISO6346_CONTAINER_NUM_LEN);
		}
		line[nread - 1] = '\0';
		int validate_code = iso6346_validate(line, &opts);
		switch (validate_code) {
		case ISO6346_OK:
			fprintf(stdout, "%s\n", line);
			break;
		case ISO6346_EVALID:
			fprintf(stderr, "%s\n", line);
			break;
		}
	}
	free(line);
	exit(EXIT_SUCCESS);
}
