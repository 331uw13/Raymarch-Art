#include <GL/gl.h>
#include "util.h"


float normalize(float t, float min, float max) {
	return (t-min)/(max-min);
}

float lerp(float t, float min, float max) {
	return (max-min)*t+min;
}

float map(float t, float s_min, float s_max, float d_min, float d_max) {
	return lerp(normalize(t, s_min, s_max), d_min, d_max);
}

float render_message_formats(STATE st, int start_y_mult, char** ptr, u32 ptr_size) {
	float y = 0.0;
	float x = 3.0+FONT_X_SPACING;
	float o = 0.0;

	if(ptr) {
		y = 3.0+st->char_height*start_y_mult;
		glColor3f(0.8, 0.8, 0.8);

		const u32 m = st->max_num_files+st->scroll_y;
		ptr_size = (ptr_size > m) ? m : ptr_size;

		for(u64 i = st->scroll_y; i < ptr_size; i++) {
			char* s = ptr[i];
			o = (float)(i%ptr_size);

			while(s) {
				if(*s == '\0') {
					y += st->char_height;
					x = 3.0+FONT_X_SPACING;
					break;
				}

				if(*s == '<') {
					s++;
					switch(*s) {
						case 'f':
							glColor3f(0.8, 0.35, 0.3);
							break;
						
						case 'p':
							glColor3f(0.7, 0.2, 0.3);
							break;
						
						case 'd':
							glColor3f(0.4, 0.4, 0.4);
							break;
						
						case 't':
							glColor3f(0.4, 0.6, 0.8);
							break;
						
						case 'a':
							glColor3f(0.33, 0.46, 0.6);
							break;
						
						case 'b':
							glColor3f(0.43, 0.3, 0.22);
							break;
						
						case 'q':
							glColor3f(0.73-(o*0.12), 0.6, 0.42+(o*0.052));
							break;

						case 'n':
						default:
							glColor3f(0.8, 0.8, 0.8);
							break;
					}
					s += 2;
				}

				draw_char(st->font, x, y, *s);
				x += st->char_width;


				s++;
			}

		
		}
	}

	return y;
}

