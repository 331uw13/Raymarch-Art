#include <GL/gl.h> 
#include "menu_view.h" 
#include "../util.h"

#define HELP_MESSAGE_LINES 7
static char* help_messages[HELP_MESSAGE_LINES] = {
	"<b>[TAB]:<q>     this view\0",
	"<b>[SPACE]:<q>   compile and render the shader\0",
	"<b>[C]:<q>       compile the shader\0",
	"<b>[E]:<q>       view log messages\0",
	"<b>[T]:<q>       open file browser\0",
	"<b>[F]:<q>       view useful functions\0",
	"<b>[A]:<q>       add texture\0",
};


void menu_open(STATE st) {
	st->flags |= FLG_VALID_VIEW;
} 
 
void menu_close(STATE st) { 
	st->flags &= ~FLG_VALID_VIEW;
} 
 
void menu_update(STATE st, int key) { 
	glColor3f(0.2,0.3,0.23);
	draw_text(st->font, 1.0, 3.0, "https://github.com/331uw13/Raymarch-Art\0", 0);

	float y = render_message_formats(st, 2, help_messages, HELP_MESSAGE_LINES);


	// TODO: add useful info here...
} 

