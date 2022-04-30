#include <stdlib.h>
#include <zlib.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "text.h"
#include "util.h"


FONT create_font() {
	FONT font = malloc(sizeof *font);
	if(!font) {
		goto giveup;
	}

	gzFile f = gzopen(FONT_FILE, "r");
	if(f == NULL) {
		goto giveup;
	}

	const u64 size = gzfread(&font->header, sizeof font->header, 1, f);

	if(gzeof(f)) {
		goto giveup;
	}

	if(size > 0) {
		font->data_size = 256*font->header.height;
		font->data = malloc(font->data_size);
	
		gzfread(font->data, font->data_size, 1, f);
	}

giveup:
	if(f) {
		gzclose(f);
	}

	return font;
}

void destroy_font(FONT f) {
	if(f) {
		if(f->data) {
			free(f->data);
			f->data = NULL;
			f->data_size = 0;
		}
		free(f);
		f = NULL;
	}
}

void draw_char(FONT f, float x, float y, char c) {
	const float ox = x;
	if(c < 0x20 || c > 0x7E) { return; }

	glBegin(GL_POINTS);
	for(u32 i = 0; i < f->header.height; i++) {
		int g = f->data[c*f->header.height+i];
		for(int j = 0; j < 8; j++) {
			if(g & 0x80) {
				glVertex2f(
						map(x, 0.0, WINDOW_WIDTH, -1.0, 1.0),
						map(y, 0.0, WINDOW_HEIGHT, 1.0,-1.0)
						);
			}

			g = g << 1;
			x += 1.0;
		}
		y += 1.0;
		x = ox;

	}
	glEnd();
}


void draw_text(FONT f, float x, float y, char* t, int length) {
	if(t) {
		glUseProgram(0);
		if(length > 0) {
			for(int i = 0; i < length; i++) {
				draw_char(f, x, y, t[i]);
				x += f->header.width+FONT_X_SPACING;
			}
		}
		else {
			int i = 0;
			while(1) {
				char c = t[i];

				if(c == '\0') {
					break;
				}

				draw_char(f, x, y, c);

				x += f->header.width+FONT_X_SPACING;
				i++;
			}
		}
	}
}





