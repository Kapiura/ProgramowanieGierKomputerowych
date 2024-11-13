#ifndef RENDER_HPP
#define RENDER_HPP

#include "ShaderProgram.hpp"
#include "glad/glad.h"

class Render {
public:
    Render();
    ~Render();

    void render(const ShaderProgram& shader) const;

private:
    GLuint vbo, vao;
    void createTriangle();
};

#endif // RENDER_HPP
