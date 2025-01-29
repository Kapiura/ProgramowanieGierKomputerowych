#include "../include/CubePalette.hpp"

CubePalette::CubePalette()
{
    Cube grass("../img/grass.jpg");
    Cube stone("../img/stone.jpg");
    Cube coord("../img/grass_debug.jpg");

    m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::Stone, std::move(stone)));
    m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::Grass, std::move(grass)));
    m_palette.insert(std::pair<Cube::Type, Cube>(Cube::Type::Grass_debug, std::move(coord)));
}

const Cube &CubePalette::LookUp(Cube::Type type) const
{
    return m_palette.at(type);
}