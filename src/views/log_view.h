#ifndef LOG_VIEW_H 
#define LOG_VIEW_H 
#include "../state.h" 
 
void log_open(STATE st); 
void log_close(STATE st); 
void log_update(STATE st, int key); 
 
#endif
