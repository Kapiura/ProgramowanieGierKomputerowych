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
#include <thread>
#include <random>


int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(10000, 99999);
  int random_number = dis(gen);

  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;
  contextSettings.stencilBits = 8;
  contextSettings.majorVersion = 3;
  contextSettings.minorVersion = 3;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Majnkraft",
                          sf::Style::Default, contextSettings);
  window.setActive(true);
  window.setMouseCursorGrabbed(true);
  window.setMouseCursorVisible(false);

  if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
    std::cerr << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  glViewport(0, 0, static_cast<GLsizei>(window.getSize().x),
             static_cast<GLsizei>(window.getSize().y));

  Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                -90.0f, 0.0f);

  ShaderProgram shaders;
  GLuint programId = shaders.getProgramId();
  if (programId == 0) {
    std::cerr << "Failed to create shader program" << std::endl;
    return -1;
  }

  shaders.use();
  shaders.setUniform("projection", camera.Projection());

  CubePalette palette;
  PerlinNoise perlin(random_number);
  // World world(glm::vec2(0, 0), palette, perlin);

  const size_t chunkSize = 16;
  std::vector<Chunk<chunkSize, chunkSize, chunkSize>> chunks;
  const int gridWidth = 6;
  const int gridHeight = 6;
  // Chunk<chunkSize, chunkSize, chunkSize> chunk(glm::vec2(0, 0), palette);
  // chunk.Generate(perlin);
    for (int x = 0; x < gridWidth; ++x) {
    for (int y = 0; y < gridHeight; ++y) {
      glm::vec2 position(x * chunkSize, y * chunkSize);
      chunks.emplace_back(position, palette);
      chunks.back().Generate(perlin);
    }
  }

  sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
  sf::Vector2i mousePosition = sf::Mouse::getPosition();

  Ray::HitType hitType;
  Chunk<chunkSize, chunkSize, chunkSize>::HitRecord hitRecord;

  sf::Clock clock;
  glEnable(GL_DEPTH_TEST);

  sf::Vector2u windowSize = window.getSize();
  while (window.isOpen()) {
    const float dt = clock.restart().asSeconds();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized)
        glViewport(0, 0, event.size.width, event.size.height);

      else if (event.type == sf::Event::MouseButtonPressed) {
        for (auto &chunk : chunks) {

          glm::vec3 direction = glm::normalize(camera.m_front);
          hitType = chunk.Hit(Ray(camera.m_position, direction), -6.0f, 6.0f,
                              hitRecord);

          if (hitType == Ray::HitType::Hit) {
            if (event.mouseButton.button == sf::Mouse::Left) {
              chunk.RemoveBlock(hitRecord.m_cubeIndex.x,
                                hitRecord.m_cubeIndex.y,
                                hitRecord.m_cubeIndex.z);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
              glm::vec3 hitCubeCenter =
                  glm::vec3(hitRecord.m_cubeIndex) + glm::vec3(0.5f);

              glm::vec3 direction =
                  glm::normalize(hitCubeCenter - camera.m_position);

              glm::ivec3 neighborOffset(
                  (direction.x > 0.5f) ? 1 : ((direction.x < -0.5f) ? -1 : 0),
                  (direction.y > 0.5f) ? 1 : ((direction.y < -0.5f) ? -1 : 0),
                  (direction.z > 0.5f) ? 1 : ((direction.z < -0.5f) ? -1 : 0));

              hitRecord.m_neighbourIndex =
                  hitRecord.m_cubeIndex + neighborOffset;

              chunk.PlaceBlock(hitRecord.m_neighbourIndex.x,
                               hitRecord.m_neighbourIndex.y,
                               hitRecord.m_neighbourIndex.z, Cube::Type::Stone);
            }
          }
        }
      }
    }

    float movementSpeed = 0.2f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      camera.MoveForward(dt + movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      camera.MoveBackward(dt + movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      camera.MoveLeft(dt + movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      camera.MoveRight(dt + movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
      camera.MoveUp(dt + movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
      camera.MoveDown(dt + movementSpeed);

    const sf::Vector2i newMousePosition = sf::Mouse::getPosition();
    camera.Rotate(newMousePosition - mousePosition);
    mousePosition = newMousePosition;

    shaders.setUniform("view", camera.View());
    shaders.setUniform("projection", camera.Projection());

   for (auto &chunk : chunks)
      chunk.Draw(shaders);

    window.display();
  }

  return 0;
}
