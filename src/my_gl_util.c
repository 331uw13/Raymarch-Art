#include <stdio.h>
#include "my_gl_util.h"

static float g_window_width = 0.0;
static float g_window_height = 0.0;


GLFWwindow* my_gl_util_initialize(
		char* window_title,
		int width,
	   	int height,
		unsigned int init_mode) {

	g_window_width = 0.0;
	g_window_height = 0.0;

	GLFWwindow* win = NULL;

	if(init_mode > MY_GL_UTIL_NUM_INIT_MODES) {
		fprintf(stderr, "Invalid init mode!\n");
		goto giveup;
	}

	if(width*height <= 0) {
		fprintf(stderr, "Window width and height must be greater than 0!\n");
		goto giveup;
	}

	if(!glfwInit()) {
		const char* desc = NULL;
		int code = glfwGetError(&desc);
		if(desc) {
			fprintf(stderr, "(ERROR CODE=%i) %s\n", code, desc);
		}
		goto giveup;
	}

	int mon_x = 0;
	int mon_y = 0;
	int mon_w = 0;
	int mon_h = 0;

	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &mon_x, &mon_y, &mon_w, &mon_h);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	if(init_mode == MY_GL_UTIL_INIT_FLOATING) {
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);	
		if(width > mon_w) {
			width = mon_w;
		}
		if(height > mon_h) {
			height = mon_h;
		}
	}
	else if(init_mode == MY_GL_UTIL_INIT_FULLSCREEN) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		width = mon_w;
		height = mon_h;
	}

	win = glfwCreateWindow(width, height, window_title, NULL, NULL);
	if(!win) {
		fprintf(stderr, "Failed to create window!\n");
		goto giveup;
	}

	glfwMakeContextCurrent(win);
	
	if(init_mode == MY_GL_UTIL_INIT_FULLSCREEN) {
		glfwSetWindowPos(win, mon_x, mon_y);
	}

	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW!\n");
		glfwDestroyWindow(win);
		goto giveup;
	}

	int fb_w = 0;
	int fb_h = 0;
	glfwGetFramebufferSize(win, &fb_w, &fb_h);
	
	glViewport(0, 0, fb_w, fb_h);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glfwFocusWindow(win);

	g_window_width = (float)fb_w;
	g_window_height = (float)fb_h;

giveup:
	if(!win) {
		glfwTerminate();
	}

	return win;
}

void my_gl_util_box(float x, float y, float w, float h) {
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(w, y);
	glVertex2f(w, h);
	glVertex2f(x, h);
	glEnd();
}


void my_gl_util_box_n(float x, float y, float w, float h) {
	x = (x/(g_window_width*0.5)-1.0);
	y = 1.0-(y/(g_window_height*0.5));
	w = (w/(g_window_width*0.5));
	h = (h/(g_window_height*0.5));

	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y-h);
	glVertex2f(x, y-h);
	glEnd();

}



