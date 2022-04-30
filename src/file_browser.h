#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include <sys/unistd.h>
#include <sys/stat.h>

#include "state.h"


int  glsl_filename_ending(FILE_T* f);
void init_files(STATE st);
void craft_permission_string(FILE_T* f, mode_t mode);
void scan_directory(STATE st);



#endif
