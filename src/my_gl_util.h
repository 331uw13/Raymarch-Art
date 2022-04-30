#ifndef MY_GL_UTIL_H
#define MY_GL_UTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MY_GL_UTIL_INIT_FLOATING   0
#define MY_GL_UTIL_INIT_FULLSCREEN 1

#define MY_GL_UTIL_NUM_INIT_MODES 2



#define CONTEXT_VERSION_MAJOR 3
#define CONTEXT_VERSION_MINOR 0


GLFWwindow* my_gl_util_initialize(
		char* title, 
		int width,
	   	int height,
	   	unsigned int init_mode
		);


void my_gl_util_box(float x, float y, float w, float h);
void my_gl_util_box_n(float x, float y, float w, float h);



#endif
