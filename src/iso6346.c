#include <unistd.h>
#include <stdio.h>

#include "../include/iso6346.h"

#define BUFFER_SIZE  (ISO6346_CONTAINER_NUM_LEN + 2)	/* Required length of input 
							   for reading in from fgets */

int main(void)
{

	char buffer[BUFFER_SIZE];	// padding for '\0'

	while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
		int validate_code = iso6346_validate(buffer);
		switch (validate_code) {
		case ISO6346_OK:
			fprintf(stdout, "%s", buffer);
			break;
		case ISO6346_EVALID:
			fprintf(stderr, "%s", buffer);
			break;
		}
	}
	return 0;
}
