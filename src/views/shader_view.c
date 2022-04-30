#include <GL/glew.h> 

#include "shader_view.h"
#include "../my_gl_util.h"
#include "../shader.h"

#include <stdio.h>

void _attempt_compile(STATE st) {
	if(st->shader_program > 0) {
		glDeleteProgram(st->shader_program);
		st->shader_program = 0;
	}


	st->shader_program = create_shader(st->shader_path);
}


void shader_open(STATE st) {
	st->flags |= FLG_VALID_VIEW;
}
 
void shader_close(STATE st) {
	st->flags &= ~FLG_VALID_VIEW;
} 
 
void shader_update(STATE st, int key) {
	if(key != SHADER_VIEW) { return; }

	_attempt_compile(st);

	if(st->shader_program > 0) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(st->shader_program);
		
		glUniform2f(glGetUniformLocation(st->shader_program, "SIZE"), (float)st->win_w, (float)st->win_h);
		my_gl_util_box(-1.0, -1.0, 1.0, 1.0);
		glUseProgram(0);
	}
	else {
		st->flags |= FLG_SHOW_LOG;
	}
} 

