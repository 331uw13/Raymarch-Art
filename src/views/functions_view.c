#include <GL/gl.h> 
#include "functions_view.h" 
#include "../util.h" 

#define FUNCTIONS_MESSAGE_LINES 32

static char* functions_messages[FUNCTIONS_MESSAGE_LINES] = {
	"<d>x,y,z are for ray position.\0",
	"<d>w is the id of the object it hit\0",
	"<d>it is -1.0 if it didnt hit anything\0",
	"<t>vec4 <f>RAYMARCH<p>(<a>vec3 <p>pos, <a>vec3 <p>dir,\0",

	" <a>float <p>treshold, <a>float<p> length);\0",
	"\0",
	"<d>compute normal of point\0",
	"<t>vec3 <f>NORMAL<p>(<a>vec3<p> pos);\0",
	"\0",
	"<d>get the ray direction for the view\0",
	"<t>vec3 <f>RAYDIR<p>(<a>float <p>fov);\0",
	"\0",
	"<t>mat2 <f>ROTATE2 <p>(<a>float <p>a);\0",
	"<t>mat3 <f>ROTATE3 <p>(<a>vec2 <p>a);\0",
	"<t>vec3 <f>REPEAT  <p>(<a>vec3 <p>ray_pos, <a>vec3<p> offsets);\0",
	"\0",
	"<t>void <f>ADD_LIGHT<p>(<a>vec3<p> ray_pos, <a>vec3<p> src_pos,\0",
	"               <a>vec3<p> color, <a>float<p> brightness);\0",
	"<t>vec3 <f>MATERIAL<p>(<a>float<p> roughness, <a>float<p> shininess,\0",
	"               <a>vec3<p> color, <a>float<p> reflectivity);\0",
	"\0",
	"<t>void  <f>ADD_SDF <p>(<a>float<p> d);\0",
	"<t>void  <f>CUT_SDF <p>(<a>float<p> d);\0",
	"<t>float <f>SPHERE_SDF <p>(<a>vec3<p> ray_pos, <a>float<p> radius);\0",
	"<t>float <f>BOX_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size, <a>float<p> roundness);\0",
	"<t>float <f>FRAME_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size, \0",
	"               <a>vec3<p> bar_size, <a>float<p> roundness);\0",
	
	"<t>float <f>_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size);\0",
	"<t>float <f>_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size);\0",
	"<t>float <f>_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size);\0",
	"<t>float <f>_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size);\0",
	"<t>float <f>_SDF <p>(<a>vec3<p> ray_pos, <a>vec3<p> size);\0",

};


void functions_open(STATE st) { 
	st->flags |= FLG_VALID_VIEW;
	st->max_scroll_y = FUNCTIONS_MESSAGE_LINES;
} 
 
void functions_close(STATE st) { 
	st->flags &= ~FLG_VALID_VIEW;
	st->scroll_y = 0;
} 
 
void functions_update(STATE st, int key) { 
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	float y = render_message_formats(st, 0, functions_messages, FUNCTIONS_MESSAGE_LINES);
} 

