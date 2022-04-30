


// TODO: comment code 



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <string.h>

#include "text.h"
#include "my_gl_util.h"
#include "file_browser.h"
#include "shader.h"
#include "log.h"

#include "views/views.h"

void render_view_init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void user_update(GLFWwindow* win, int action, int i) {
	if(action == GLFW_PRESS) {
		STATE st = glfwGetWindowUserPointer(win);
		if(st) {
			set_view(st, i);
			update_view(st, i);
		}
	}
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	user_update(win, action, key);
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
	user_update(win, action, button);
}

void clamp(int* i, int min, int max) {
	if(i) {
		*i = (*i < min) ? min : (*i > max) ? max : *i;
	}
}

void scroll_callback(GLFWwindow* win, double x, double y) {
	STATE st = glfwGetWindowUserPointer(win);
	if(st) {
		int i = st->scroll_y-(int)y;
		clamp(&i, 0, st->max_scroll_y);
		st->scroll_y = i;
		scan_directory(st);
	}
}

void cursor_position_callback(GLFWwindow* win, double xpos, double ypos) {
	STATE st = glfwGetWindowUserPointer(win);
	if(st) {
		int cy = (ypos/st->char_height)-1;
		clamp(&cy, 0, st->max_scroll_y);

		st->cursor_y = st->scroll_y+cy;
	}
}

#define KEYDOWN(k) glfwGetKey(st->win, k)

void update_loop(STATE st) {
	const float file_path_y = WINDOW_HEIGHT-(st->char_height-3.0);

	while(!glfwWindowShouldClose(st->win)) {
		if(st->flags & FLG_SHOW_LOG) {
			set_view(st, LOG_VIEW);
			st->flags &= ~FLG_SHOW_LOG;
		}

		update_view(st, -1);
		
		if(st->current_view != SHADER_VIEW) {
			glColor3f(0.6, 0.6, 0.6);
			if(st->shader_path_size > 0) {
				draw_text(st->font, 3.0, file_path_y, 
						st->shader_path, st->shader_path_size);
			}
			else {
				draw_text(st->font, 3.0, file_path_y, 
						"No file selected.", 17);
			}
		}

		
		glfwSwapBuffers(st->win);
		glfwWaitEvents();
	}

	if(st->flags & FLG_VALID_VIEW && st->view_close) {
		st->view_close(st);
	}
}

int main() {
	const uid_t uid = getuid();
	if(uid == 0) {
		return -1;
	}

	STATE st = malloc(sizeof *st);
	if(st) {

		st->flags = 0;
		st->cursor_y = 0;
		st->scroll_y = 0;
		st->shader_program = 0;
		st->shader_path[0] = '\0';
		st->shader_path_size = 0;
		st->current_view = 0;
		st->current_file_ptr = NULL;
		st->max_scroll_y = 0;
		st->size_uniform = 0;
		log_clear();
		getcwd(st->current_dir, 1024);

		st->win = my_gl_util_initialize("Raymarch Art", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		st->font = create_font();

		int width = 0;
		int height = 0;

		glfwGetWindowSize(st->win, &width, &height);

		st->win_w = width;
		st->win_h = height;
	
		if(st->win && st->font) {
			st->char_width = st->font->header.width+FONT_X_SPACING;
			st->char_height = st->font->header.height+FONT_Y_SPACING;
			st->cursor_max_y = (width/2)/st->char_height;
	
			st->num_files = 0;
			st->max_num_files = (height/st->char_height)-4;
			st->files_size = st->max_num_files*sizeof *st->files;
			st->files = malloc(st->files_size);

			if(st->files) {
				init_files(st);

				glfwWaitEventsTimeout(1.0);
				glfwSetWindowUserPointer(st->win, st);
				glfwSetKeyCallback(st->win, key_callback);
				glfwSetScrollCallback(st->win, scroll_callback);
				glfwSetMouseButtonCallback(st->win, mouse_button_callback);
				glfwSetCursorPosCallback(st->win, cursor_position_callback);
				
				set_view(st, MENU_VIEW);
				update_loop(st);
	
				free(st->files);
			}
		}

		destroy_font(st->font);
		
		glfwDestroyWindow(st->win);
		glfwTerminate();
		
		free(st);
	}

	return 0;
}

