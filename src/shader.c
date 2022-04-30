#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "log.h"

#define VERTEX_SHADER_SOURCE \
	"#version 330\n"\
	"in vec2 pos;"\
	"void main() {"\
		"gl_Position = vec4(pos, 0.0, 1.0);"\
	"}"

#define FRAGMENT_SHADER_SOURCE \
	"#version 330\n"\
	"#define PI 3.14159\n"\
	"#define PI_2 6.28318\n"\
	"#define PI_R 0.01745\n"\
	"vec2 UV;"\
	"vec3 RAY_COLOR;"\
	"vec3 CAMERA_POS;"\
	"uniform vec2 SIZE;"\
	""\
	"void shader_main();"\
	"vec2 map(vec3 p);"\
	"void main() {"\
		"CAMERA_POS = vec3(0.0, 0.0, 0.0);"\
		"RAY_COLOR = vec3(0.0, 0.0, 0.0);"\
		"UV = gl_FragCoord.xy-SIZE*0.5;"\
		"shader_main();"\
		"gl_FragColor = vec4(RAY_COLOR, 1.0);"\
	"}"\
	"vec3 RAYDIR(float fov) {"\
		"float hf = tan((90.0-fov*0.5)*PI_R);"\
		"return normalize(vec3(UV, SIZE.y*0.5*hf));"\
	"}\n"\
	"vec4 RAYMARCH(vec3 pos, vec3 dir, float treshold, float max_length) {"\
		"float rl = 0.0;"\
		"vec4 r = vec4(pos, -1.0);\n"\
		"for(int i = 0; i < 64; i++) {"\
			"r.xyz = pos+dir*rl;"\
			"vec2 m = map(r.xyz);"\
			"if(m.x < treshold*rl) {"\
				"r.w = m.y;"\
				"break;"\
			"}"\
			"else if(rl > max_length) { break; }"\
			"rl += m.x;"\
		"}"\
		"return r;"\
	"}\n"\
	"vec3 NORMAL(vec3 p) {"\
		"vec2 s = vec2(0.01, 0.0);"\
		"vec3 v = vec3(map(p-s.xyy).x, map(p-s.yxy).x, map(p-s.yyx).x);"\
		"return normalize(map(p).x-v);"\
	"}\n"\
	"vec3 ADD_LIGHT(vec3 p, vec3 light_pos, vec3 color, float brightness) {"\
		"vec3 norm = NORMAL(p);"\
		"vec3 ldir = normalize(light_pos-p);"\
		"vec3 diffuse = max(dot(norm, ldir), 0.0)*color;"\
		"vec3 ambient = diffuse*0.2+0.43;"\
		"vec3 vdir = normalize(CAMERA_POS-p);"\
		"vec3 rdir = normalize(ldir+vdir);"\
		"vec3 specular = 0.6*pow(max(dot(norm, rdir), 0.0), 16)*color;"\
		"return clamp(brightness*(ambient+diffuse+specular), vec3(0.0), vec3(1.0));"\
	"}\n"\
	"mat2 ROTATE2(float a) {"\
		"float c = cos(a*PI_R);"\
		"float s = sin(a*PI_R);"\
		"return mat2(c, -s, s, c);"\
	"}"\
	"vec2 MIN_SDF(vec2 a, vec2 b) {"\
		"return (a.x < b.x) ? a : b;"\
	"}"\
	"float BOX_SDF(vec3 p, vec3 s, float r) {"\
		"p = abs(p)-s;"\
		"return length(max(p, 0.0))+min(max(p.x, max(p.y, p.z)), 0.0)-r;"\
	"}\n"\
	"float SPHERE_SDF(vec3 p, float r) {"\
		"return length(p)-r;"\
	"}"\
	"float HEXAGON_SDF(vec3 p, vec2 h) {"\
		"const vec3 k = vec3(-0.8660254, 0.5, 0.57735);"\
		"p = abs(p);"\
		"p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;"\
		"vec2 d = vec2(length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),"\
		"p.z-h.y );"\
		"return min(max(d.x,d.y),0.0) + length(max(d,0.0));"\
	"}"\
	"\0"

int compile(int shader) {
	int res = 0;
	if(shader > 0) {
		glCompileShader(shader);

		char* log = NULL;
		int log_size = 0;
		int compiled = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if(log_size > 0) {
			if((log = malloc(log_size))) {
				glGetShaderInfoLog(shader, log_size, NULL, log);
				log_add_text(log, log_size);
				free(log);
			}
			else {
				fprintf(stderr, "failed to allocate memory for log\n");
			}
		}

		/*
		if(!compiled) {
			int shader_type = 0;
			glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);
		}
		*/

		res = compiled;
	}

	return res;
}

int create_shader(char* path) {
	log_clear();
	int program = 0;
	int fd = 0;
	struct stat sb;
	char* address = NULL;

	if(access(path, O_RDONLY) != F_OK) {
		log_add_text("failed to open file", 20);
		goto giveup;
	}

	if((fd = open(path, O_RDONLY)) == -1) {
		perror("open");
		goto giveup;
	}
	
	if((fstat(fd, &sb)) == -1) {
		perror("fstat");
		goto close_giveup;
	}

	if(sb.st_size == 0) {
		log_add_text("the file doesnt have any content\0", 0);
		goto close_giveup;
	}

	if(!(address = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)))  {
		perror("mmap");
		goto close_giveup;
	}


	int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const u64 len = strlen(FRAGMENT_SHADER_SOURCE);
	char* merged_src = NULL;
	merged_src = malloc(sb.st_size+len+1);

	if(!merged_src) {
		goto close_giveup;
	}

	memmove(merged_src, FRAGMENT_SHADER_SOURCE, len);
	memmove(merged_src+len, address, sb.st_size);

	merged_src[len+sb.st_size] = '\0';


	glShaderSource(fragment, 1, (const char* const*)&merged_src, NULL);
	munmap(address, sb.st_size);
	free(merged_src);

	
	if(!compile(fragment)) {
		goto close_giveup;
	}

	int vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* vrtx_src = VERTEX_SHADER_SOURCE;
	glShaderSource(vertex, 1, &vrtx_src, NULL);

	if(!compile(vertex)) {
		goto close_giveup;
	}

	program = glCreateProgram();
	if(!program) {
		log_add_text("failed to create shader program\0", 0);
		goto close_giveup;
	}

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	int linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	/*
	if(!linked) {
		char* log;
		int log_size = 0;
		
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		if((log = malloc(log_size))) {
			glGetProgramInfoLog(program, log_size, NULL, log);
			log_add_text(log, 0);
			free(log);
		}

		if(program) {
			glDeleteProgram(program);
			program = 0;
		}
	}
	*/

	// TODO: check for linking errors!

close_giveup:
	
	if(fd >= 0) {
		close(fd);
	}

	if(fragment) {
		glDeleteShader(fragment);
	}
	
	if(vertex) {
		glDeleteShader(vertex);
	}


giveup:
	return program;
}

