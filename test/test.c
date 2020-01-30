#include  <assert.h>
#include <stdio.h>

#include "../include/iso6346.h"

int main(void)
{
	struct iso6346_validate_opts opts = {
		.check_length = false,
		.case_sensitive = true
	};
	char container_number[12];
	assert(iso6346_validate("TEST", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("HELLO", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("MSKU2666542", &opts) == ISO6346_OK);
	assert(iso6346_validate("HLXU1234567", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("NYKU3086856", &opts) == ISO6346_OK);
	assert(iso6346_validate("NYKU3086856", &opts) == ISO6346_OK);
	assert(iso6346_validate("NyKu3086856", &opts) == ISO6346_EVALID);
	opts.case_sensitive = false;
	assert(iso6346_validate("NyKu3086856", &opts) == ISO6346_OK);
	assert(iso6346_validate("NyKu3086856123", &opts) == ISO6346_OK);
	opts.check_length = true;
	assert(iso6346_validate("NyKu3086856123", &opts) == ISO6346_EVALID);
	for (unsigned int i = 0; i < 1000; i++) {
		iso6346_generate_random(container_number);
		assert(iso6346_validate(container_number, &opts) == ISO6346_OK);
	}
	return 0;
}
