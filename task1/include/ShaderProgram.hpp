#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "glad/glad.h"
#include <string>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    void use() const;

private:
    GLuint programId;
    GLuint createShader(const GLchar* source, GLenum type);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif // SHADERPROGRAM_HPP
