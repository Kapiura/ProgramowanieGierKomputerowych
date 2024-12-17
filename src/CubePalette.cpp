#include "../include/CubePalette.hpp"
CubePalette::CubePalette()
{
  Cube grass("../img/grass.jpg");
  Cube stone("../img/stone.jpg");
  Cube grass_debug("../img/grass_debug.jpg");
  m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::Stone, std::move(stone)));
  m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::Grass, std::move(grass)));
  m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::GrassDebug, std::move(grass_debug)));
}
const Cube &CubePalette::LookUp(Cube::Type type) const {
  return m_palette.at(type);
}