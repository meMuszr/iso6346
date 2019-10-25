#ifndef ISO6346_H
#define ISO6346_H

/* Container Constants */

#define CONTAINER_NUM_LEN  (11)  /* Required length of a container number */
#define PREFIX_LEN         (4)   /* Number of chars in Owner Code + Category prefix */
#define NUM_CATEGORIES     (3)   /* Number of container categories */
#define VALIDATION_ERROR   (0)   /* Error code for invalid container code */

/* Container categories */

enum categories {U = 'U',   /* identifier for all freight containers */
                 J = 'J',   /* identifier for detachable freight container-related equipment */
                 Z = 'Z'};  /* identifier for trailers and chassis */

static const enum categories category_map[] = {U, J, Z};

/* Container Number Processing Functions */

int validate(unsigned char *);

#endif
