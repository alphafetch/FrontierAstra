#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/gl.h>

GLuint generate2DTexture();
int upload2DTexture(std::string texture_path);

#endif