#include "strsep.h"
#include <string.h>
char* mystrsep(char** stringp, const char* delim)
{
    char* start = *stringp, * p = start ? strpbrk(start, delim) : NULL;

    if (!p) {
        *stringp = NULL;
    }
    else {
        *p = 0;
        *stringp = p + 1;
    }

    return start;
}