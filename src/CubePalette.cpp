#include "../include/CubePalette.hpp"
CubePalette::CubePalette() {
  Cube grass("../img/grass.jpg");
  Cube stone("../img/stone.jpg");
  m_palette.insert(
      std::pair<Cube::Type, Cube>(Cube::Type::Stone, std::move(stone)));
  m_palette.insert(
      std::pair<Cube::Type, Cube>(Cube::Type::Grass, std::move(grass)));
}
const Cube &CubePalette::LookUp(Cube::Type type) const {
  return m_palette.at(type);
}