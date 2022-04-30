#ifndef UTIL_H
#define UTIL_H

#include "state.h"


float normalize(float t, float min, float max);
float lerp(float t, float min, float max);
float map(float t, float s_min, float s_max, float d_min, float d_max);
float render_message_formats(STATE st, int start_y_mult, char** ptr, u32 ptr_size);

int attempt_compile_shader(STATE st);

#endif
