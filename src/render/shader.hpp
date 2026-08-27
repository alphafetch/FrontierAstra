#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/gl.h>

GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(GLuint vert, GLuint frag);

#endif