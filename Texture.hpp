#pragma once

#include <stdexcept>

#include <string_view>
#include <glad/glad.h>

#include <stb_image.h>


class Texture
{
public:
	Texture(const char* filename);
	void bind();
	void unbind();

private:
	unsigned char* loadFromFile(const char* filename);
	void setupGL();

private:
	unsigned int textureID;
	unsigned char* imageData;
	int width;
	int height;
	int nrChannels;
};

Texture::Texture(const char* filename)
{
	imageData = loadFromFile(filename);
	// TODO: fix error check
	// imageData won't be null if there were errors parsing the image file
	if (imageData == nullptr)
		throw std::runtime_error{ "Error when loading texture file " + std::string{filename} };
	else
		setupGL();
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setupGL()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// we assume data has been correctly loaded from file into the imageData pointer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);


	// free image memory after sending to GPU
	stbi_image_free(imageData);

	// I guess unbind so we don't accidentally use the wrong texture elsewhere, and also
	// so others who need to bind the texture get errors if *they* forget to bind?
	glBindTexture(GL_TEXTURE_2D, 0);
}



unsigned char* Texture::loadFromFile(const char* filename)
{
	return stbi_load(filename, &width, &height, &nrChannels, 0);
}
