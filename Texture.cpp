#include"Texture.h"

Texture::Texture(const char* image, const char * texType, GLuint slot, GLenum format, GLenum pixelType,std::string mipmapType)
{
	type = texType;
	//Texture
	int imageWidth, imageHeight, ColorChannels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* image_data = stbi_load(image, &imageWidth, &imageHeight, &ColorChannels, 0);
	if (!image_data) {
		std::cerr << "CANNOT LOAD TEXTURE FROM :" << image<< std::endl;
	}
	std::cout << "Image"<<image<<" , Loaded : Width = " << imageWidth << " Height = " << imageHeight << " ColorChannels = " << ColorChannels << std::endl;

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0+slot);
	unit = slot;

	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (mipmapType == "standard") {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, pixelType, image_data);
	}

	if (mipmapType == "bilinear") {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, imageWidth /= 2, imageHeight /= 2, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, imageWidth /= 4, imageHeight /= 4, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, imageWidth /= 4, imageHeight /= 8, 0, format, pixelType, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	if(mipmapType == "trilinear"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, imageWidth /= 2, imageHeight /= 2, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, imageWidth /= 4, imageHeight /= 4, 0, format, pixelType, image_data);
		glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, imageWidth /= 4, imageHeight /= 8, 0, format, pixelType, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_set_flip_vertically_on_load(false);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}