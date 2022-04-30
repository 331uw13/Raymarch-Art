#include "files_view.h" 
#include "../settings.h"
#include "../file_browser.h"
#include "../my_gl_util.h"

#include <string.h>

void files_open(STATE st) { 
	st->flags |= FLG_VALID_VIEW;
	scan_directory(st);
} 
 
void files_close(STATE st) { 
	st->flags &= ~FLG_VALID_VIEW;
	st->scroll_y = 0;
	st->cursor_y = 0;
}

void files_update(STATE st, int key) { 
	
	st->current_file_ptr = &st->files[st->cursor_y-st->scroll_y];
	
	if(st->current_file_ptr) {
		FILE_T* f = st->current_file_ptr;
		switch(key) {

			case GLFW_MOUSE_BUTTON_RIGHT:
			case GLFW_KEY_BACKSPACE:
				if(chdir("..") >= 0) {
					getcwd(st->current_dir, 1024);
					scan_directory(st);
				}
				break;

			case GLFW_MOUSE_BUTTON_LEFT:
			case GLFW_KEY_ENTER:
				st->shader_path_size = 0;
				if(!f->can_exec) {
					if(f->is_dir) {
						if(chdir(f->name) >= 0) {
							getcwd(st->current_dir, 1024);
							st->cursor_y = 0;
							st->scroll_y = 0;
							scan_directory(st);
						}
					}
					else {
						memmove(st->shader_path, f->name, f->length);
						st->shader_path_size = f->length;
						st->shader_path[st->shader_path_size] = '\0';
					}
				}
				break;

			default:break;
		}
	}
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.3, 0.85, 0.3);
	draw_text(st->font, 1.0, 3.0, FILE_VIEW_TITLE, 0);


	float y = st->char_height+3.0;
	float x = FONT_X_SPACING+3.0;
	float yf = FONT_Y_SPACING/2.0;

	FILE_T* f = NULL;

	for(u32 i = 0; i < st->num_files; i++) {
		f = &st->files[i];
		if(!f) { break; }

		glColor3f(f->r, f->g, f->b);
		if(i+st->scroll_y == st->cursor_y) {
			float w = st->char_width*f->length+6.0;
			float h = st->char_height+yf;
			my_gl_util_box_n(0.0, y-yf, w, h);
			
			glColor3f(f->r*0.2, f->g*0.2, f->b*0.2);
			my_gl_util_box_n(w, y, (16+FILE_T_NAME_LENGTH-f->length)*st->char_width, h-yf*2);
			
			glColor3f(0.0, 0.0, 0.0);
			st->current_file_ptr = f;
		}
		
		draw_text(st->font, x, y, f->name, f->length);
		
		glColor3f(0.5, 0.5, 0.5);
		draw_text(st->font, 32*st->char_width, y, f->perm_str, 12);
		
		y += st->char_height;
	}
}


