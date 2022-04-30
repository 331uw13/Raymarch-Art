#include "views.h"

#include "menu_view.h"
#include "files_view.h"
#include "shader_view.h"
#include "log_view.h"
#include "functions_view.h"

#include "../util.h"
#include "../shader.h"

void update_view(STATE st, int c) {
	if(st->flags & FLG_VALID_VIEW && st->view_update) {
		st->view_update(st, c);
	}
}

void _setup_view(STATE st, u32 id,
	   	void(*open_ptr)(STATE),
	   	void(*close_ptr)(STATE),
	   	void(*update_ptr)(STATE, int)) {
	
	if(open_ptr == NULL || close_ptr == NULL || update_ptr == NULL) {
		st->view_open = NULL;
		st->view_close = NULL;
		st->view_update = NULL;
		st->flags &= ~FLG_VALID_VIEW;
	}
	else {

		if(st->view_close && st->flags & FLG_VALID_VIEW) {
			st->view_close(st);
		}

		st->view_open = open_ptr;
		st->view_close = close_ptr;
		st->view_update = update_ptr;

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		st->current_view = id;

		st->view_open(st);
	}
}

void set_view(STATE st, u32 id) {
	if(st->current_view != id) {
		switch(id) {	
			case MENU_VIEW:
				_setup_view(st, id, menu_open, menu_close, menu_update);
				break;
			
			case FILE_VIEW:
				_setup_view(st, id, files_open, files_close, files_update);
				break;
	
			case SHADER_VIEW:
				_setup_view(st, id, shader_open, shader_close, shader_update);
				break;

			case LOG_VIEW:
				_setup_view(st, id, log_open, log_close, log_update);
				break;
			
			case FUNCTIONS_VIEW:
				_setup_view(st, id, functions_open, functions_close, functions_update);
				break;
			
			case COMPILE_VIEW:
				if(st->shader_program > 0) {
					glDeleteProgram(st->shader_program);
				}
				if(!(st->shader_program = create_shader(st->shader_path))) {
					st->flags |= FLG_SHOW_LOG;
				}
				break;
		
			default:break;
		}
	}
}




