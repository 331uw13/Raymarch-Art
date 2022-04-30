#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <string.h>

#include "log.h"
#include "string.h"


static STRING_T g_log[MAX_LOG_LINES];
static int      g_log_lines;



void log_clear() {
	g_log_lines = 0;
}

void log_render_view(FONT f) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.74, 0.42, 0.3);
	draw_text(f, 1.0, 3.0, LOG_VIEW_TITLE, 0);

	float y = f->header.height+5.0;

	glColor3f(0.8, 0.75, 0.7);
	for(int i = 0; i < g_log_lines; i++) {
		draw_text(f, 1.0, y, g_log[i].data, g_log[i].size);
		y += f->header.height+2;
	}
}

int log_new_line() {
	int res = 0;
	if(g_log_lines < MAX_LOG_LINES) {
		g_log_lines++;
		res = 1;
	}

	return res;
}

void log_add_text(char* text, u64 size) {
	if(g_log_lines > MAX_LOG_LINES) { return; }

	STRING_T* s = &g_log[g_log_lines];
	s->size = 0;
	int j = 0;

	if(size == 0) {
		size = strlen(text);
	}

	for(u64 i = 0; i < size; i++) {
		char c = text[i];
		int start_new = (j > (MAX_STRING_LENGTH-16) && c == 0x20) || (j > MAX_STRING_LENGTH);

		if(c == 0xA || start_new) {
			if(!log_new_line()) {
				return;
			}

			j = 0;
			s = &g_log[g_log_lines];
			s->size = 0;

			if(c == 0x20) {
				continue;
			}
		}
		
		if(c <= 0x1F || c >= 0x7F) {
			continue;
		}

		s->size++;
		s->data[j] = c;
		j++;
	}

	log_new_line();
}


