#include <SFML/Window.hpp>
#include "../include/glad/glad.h"
#include "../include/ShaderProgram.hpp"
#include "../include/Render.hpp"
#include <iostream>

int main() {
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion = 3;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, contextSettings);
    window.setActive(true);

    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    ShaderProgram shaderProgram(
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 1.0);\n"
        "}\0",
        "#version 330 core\n"
        "out vec4 fragmentColor;\n"
        "void main() {\n"
        "   fragmentColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
        "}\0"
    );

    Render render;

    bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                running = false;
        }

        render.render(shaderProgram);
        window.display();
    }

    return 0;
}
