#ifndef STRING_H
#define STRING_H

#include "typedefs.h"
#include "settings.h"

// used for log. see log.c and log.h for more details...

struct string_t {
	//TODO: //char new; // has the user seen it yet?
	char data[MAX_STRING_LENGTH];
	u64  size;
};

#define STRING_T struct string_t

#endif
