#include <GL/gl.h> 
#include "log_view.h" 
#include "../settings.h"
#include "../log.h"

void log_open(STATE st) {
	st->flags |= FLG_VALID_VIEW;
	st->flags &= ~FLG_SHOW_LOG;
} 
 
void log_close(STATE st) {
	st->flags &= ~FLG_VALID_VIEW;
} 
 
void log_update(STATE st, int key) {
	glColor3f(0.8, 0.4, 0.4);
	draw_text(st->font, FONT_X_SPACING, FONT_Y_SPACING, LOG_VIEW_TITLE, 0);

	log_render_view(st->font);
}

