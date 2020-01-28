#include  <assert.h>

#include "../include/iso6346.h"

int main(void)
{
	const struct iso6346_validate_opts opts = {
		.check_length = false,
		.case_sensitive = true
	};
	assert(iso6346_validate("TEST", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("HELLO", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("MSKU2666542", &opts) == ISO6346_OK);
	assert(iso6346_validate("HLXU1234567", &opts) == ISO6346_EVALID);
	assert(iso6346_validate("NYKU3086856", &opts) == ISO6346_OK);
	assert(iso6346_validate("NYKU3086856", &opts) == ISO6346_OK);
	assert(iso6346_validate("NyKu3086856", &opts) == ISO6346_EVALID);
	return 0;
}
