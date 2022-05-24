#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureComponent.h"

TextureComponent::TextureComponent(const std::string& fileName)
{
	int width, height, bpp;
	stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
		0, //level
		GL_RGBA, //internal format
		width, //width
		height, //height
		0, //border
		GL_RGBA, //data format
		GL_UNSIGNED_BYTE, //data type
		data); //data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
}

void TextureComponent::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
