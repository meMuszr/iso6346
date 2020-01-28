#ifndef ISO6346_H
#define ISO6346_H
#include <stdbool.h>

/* Container Constants */

#define ISO6346_CONTAINER_NUM_LEN (11)	/* Required length of a container number */
#define ISO6346_PREFIX_LEN (4)	/* Number of chars in Owner Code + Category prefix */
#define ISO6346_NUM_CATEGORIES (3)	/* Number of container categories */
#define ISO6346_OK (0)		/* Return code for valid container number */
#define ISO6346_EVALID (1)	/* Error code for invalid container number */

/* Container categories */

enum iso6346_category {
	U = 'U',		/* identifier for all freight containers */
	J = 'J',		/* identifier for detachable freight container-related equipment */
	Z = 'Z'			/* identifier for trailers and chassis */
};

struct iso6346_validate_opts {
	bool check_length;
	bool case_sensitive;
};

static const enum iso6346_category iso6346_category_map[] = { U, J, Z };

/* Container Number Processing Functions */

int iso6346_validate(const char *, const struct iso6346_validate_opts *);

#endif
