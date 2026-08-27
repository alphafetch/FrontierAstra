#include "shader.hpp"

#include <iostream>

#include "constants.hpp"

using std::cerr, std::endl;

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);

    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[LOG_LEN];
        glGetShaderInfoLog(shader, LOG_LEN, nullptr, infoLog);
        cerr << "Shader compilation failed:" << NEWLINE << infoLog << endl;
    }

    return shader;
}

GLuint createShaderProgram(GLuint vert, GLuint frag) {
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[LOG_LEN];
        glGetProgramInfoLog(shaderProgram, LOG_LEN, nullptr, infoLog);
        cerr << "Shader linking failed:" << NEWLINE << infoLog << endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return shaderProgram;
}