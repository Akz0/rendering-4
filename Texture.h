#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<gl/glew.h>
#include "./external packages/stb/stb_image.h"

#include"Shader.h"

class Texture
{
public:
	GLuint ID;
	const char * type;
	GLuint unit;

	Texture(const char* image, const char * texType, GLuint slot, GLenum format, GLenum pixelType, std::string mipmapType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif