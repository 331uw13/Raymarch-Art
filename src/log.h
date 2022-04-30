#ifndef LOG_H
#define LOG_H

#include "state.h"
#include "typedefs.h"

int  log_new_line();
void log_add_text(char* text, u64 size);

void log_clear();
void log_render_view(FONT f);

#endif
