#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

u32 create_texture(char* path) {

	u32 texture = 0;

	int width = 0;
	int height = 0;
	int num_channels = 0;
	int format = GL_RGB;
	unsigned char* data = NULL;

	data = stbi_load(path, &width, &height, &num_channels, 0);
	if(data == NULL) {
		goto giveup;
		//VMessage(VMSG_ERROR, "Failed to load texture from file \"%s\"", filename);
		//VMessage(VMSG_ERROR, "Reason: \"%s\"", stbi_failure_reason());
	}

	switch(num_channels) {
		case 1: format = GL_RED;   break;
		case 2: format = GL_RG;    break;
		case 3: format = GL_RGB;   break;
		case 4: format = GL_RGBA;  break;
		default:
			goto giveup;
			break;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(texture, GL_TEXTURE_2D);

	glTexParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	
giveup:
	return texture;

}

void destroy_texture(u32 t) {
	glDeleteTextures(1, &t);
}



