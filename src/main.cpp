#include "../include/Camera.hpp"
#include "../include/Chunk.hpp"
#include "../include/Cube.hpp"
#include "../include/CubePalette.hpp"
#include "../include/PerlinNoise.hpp"
#include "../include/Ray.hpp"
#include "../include/ShaderProgram.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {
  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.stencilBits = 8;
  contextSettings.majorVersion = 3;
  contextSettings.minorVersion = 3;

  sf::Window window(sf::VideoMode(800, 600), "Majnkraft", sf::Style::Default,
                    contextSettings);
  window.setActive(true);
  window.setMouseCursorGrabbed(true);
  window.setMouseCursorVisible(false);

  if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
    std::cerr << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  glViewport(0, 0, static_cast<GLsizei>(window.getSize().x),
             static_cast<GLsizei>(window.getSize().y));
  glEnable(GL_DEPTH_TEST);

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                -90.0f, 0.0f);

  ShaderProgram shaders;
  GLuint programId = shaders.getProgramId();
  if (programId == 0) {
    std::cerr << "Failed to create shader program" << std::endl;
    return -1;
  }

  shaders.use();

  // Cube cube("../img/grass.jpg");
  CubePalette palette;
  PerlinNoise perlin;

  const size_t chunkSize = 16;
  Chunk<chunkSize, chunkSize, chunkSize> chunk(glm::vec2(0, 0), palette);
  chunk.Generate(perlin);

  // shaders.setUniform("projection", camera.Projection());
  Ray::HitType hitType;
  Chunk<chunkSize, chunkSize, chunkSize>::HitRecord hitRecord;

  // Cube cube("../img/grass.jpg");
  CubePalette palette;

  // shaders.setUniform("projection", camera.Projection());
  PerlinNoise perlin;

  const size_t chunkSize = 16;
  Chunk<chunkSize, chunkSize, chunkSize> chunk(glm::vec2(0, 0), palette);
  chunk.Generate(perlin);

  sf::Clock clock;
  sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
  sf::Vector2i mousePosition = sf::Mouse::getPosition();

  glEnable(GL_DEPTH_TEST);

  while (window.isOpen()) {
    const float dt = clock.restart().asSeconds();

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::Resized) {
        glViewport(0, 0, event.size.width, event.size.height);
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        Ray ray(camera.m_position, camera.m_front);
        Ray::HitType hitType = chunk.Hit(ray, 0.0f, 3.0f, hitRecord);

        if (hitType == Ray::HitType::Hit) {
          std::cout << "Hit block at: (" << hitRecord.m_cubeIndex.x << ", "
                    << hitRecord.m_cubeIndex.y << ", "
                    << hitRecord.m_cubeIndex.z << ")" << std::endl;

          if (event.mouseButton.button == sf::Mouse::Left) {
            if (chunk.RemoveBlock(hitRecord.m_cubeIndex.x,
                                  hitRecord.m_cubeIndex.y,
                                  hitRecord.m_cubeIndex.z)) {
              std::cout << "Block removed at: (" << hitRecord.m_cubeIndex.x
                        << ", " << hitRecord.m_cubeIndex.y << ", "
                        << hitRecord.m_cubeIndex.z << ")" << std::endl;
            } else
              std::cout << "Failed to remove block" << std::endl;
          } else if (event.mouseButton.button == sf::Mouse::Right) {
            glm::ivec3 neighbor = hitRecord.m_neighbourIndex;
            if (chunk.PlaceBlock(neighbor.x, neighbor.y, neighbor.z,
                                 Cube::Type::Grass)) {
              std::cout << "Block placed at: (" << neighbor.x << ", "
                        << neighbor.y << ", " << neighbor.z << ")" << std::endl;
            } else
              std::cout << "Failed to place block" << std::endl;
          }
        } else
          std::cout << "No block hit" << std::endl;
      }
    }

    float movementSpeed = 0.0275f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      camera.MoveForward(dt + movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      camera.MoveBackward(dt + movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      camera.MoveLeft(dt + movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      camera.MoveRight(dt + movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      camera.MoveUp(dt + movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
      camera.MoveDown(dt + movementSpeed);
    }

    const sf::Vector2i newMousePosition = sf::Mouse::getPosition();
    camera.Rotate(newMousePosition - mousePosition);
    mousePosition = newMousePosition;
    // std::cout << "Yaw: " << camera.GetYaw() << ", Pitch: " <<
    // camera.GetPitch() << std::endl; std::cout << "Mouse Position: " <<
    // newMousePosition.x << ", " << newMousePosition.y << std::endl;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaders.setUniform("view", camera.View());
    shaders.setUniform("projection", camera.Projection());

    glDrawArrays(GL_LINES, 0, 2);

    // cube.Draw();
    chunk.Draw(shaders);

    window.display();
  }

  return 0;
}
