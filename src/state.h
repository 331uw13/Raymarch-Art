#ifndef STATE_H
#define STATE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "text.h"

#define STATE struct state_t*
#define FILE_T struct file_t

#define FLG_RENDER_SHADER 0x1
#define FLG_VALID_VIEW    0x4
#define FLG_SHOW_LOG      0x8

#define MENU_VIEW     GLFW_KEY_TAB
#define FILE_VIEW     GLFW_KEY_T
#define SHADER_VIEW   GLFW_KEY_SPACE
#define COMPILE_VIEW  GLFW_KEY_C
#define LOG_VIEW        GLFW_KEY_E
#define FUNCTIONS_VIEW  GLFW_KEY_F


struct file_t {
	char perm_str[12];
	char name[FILE_T_NAME_LENGTH];
	u32  length;

	char can_exec;
	char is_dir;

	// name color  0.0 - 1.0
	float r;
	float g;
	float b;
};


struct state_t {
	int flags;
	int win_w;
	int win_h;
	
	u32 cursor_y;
	u32 cursor_max_y;
	u32 scroll_y;
	u32 max_scroll_y;

	u32 current_view;

	int   shader_program;
	char  shader_path[1024];
	u32   shader_path_size;

	int size_uniform;

	char    current_dir[1024];
	FILE_T* files;
	FILE_T* current_file_ptr;
	u64     files_size;
	u32     num_files;
	u32     max_num_files;
	u64     total_num_files;

	float char_width;
	float char_height;

	FONT font;
	GLFWwindow* win;

	void(*view_open)(STATE);
	void(*view_close)(STATE);
	void(*view_update)(STATE, int);

};


#endif
