#ifndef RENDER_SHADER_HPP
#define RENDER_SHADER_HPP

#include <glad/gl.h>

GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(GLuint vert, GLuint frag);

#endif