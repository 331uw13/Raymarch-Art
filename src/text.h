#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include "settings.h"
#include "typedefs.h"


struct psf2header_t {
	char magic[4];
	u32 version;
	u32 headersize;
	u32 flags;
	u32 length;
	u32 charsize;
	u32 height;
	u32 width;
};

struct font_t {
	struct psf2header_t header;
	char* data;
	u64 data_size;
};

#define FONT struct font_t*

FONT create_font();
void destroy_font(FONT f);

void draw_char(FONT f, float x, float y, char c);
void draw_text(FONT f, float x, float y, char* t, int length);


#endif
