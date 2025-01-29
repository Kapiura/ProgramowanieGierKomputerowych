#pragma once
#include "../include/Chunk.hpp"
#include "../include/CubePalette.hpp"
#include "../include/PerlinNoise.hpp"
#include <vector>
#include <glm/glm.hpp>

class World {
public:
    World(const glm::vec2& origin, CubePalette& palette, const PerlinNoise& rng);

    void Draw(ShaderProgram& shader) const;
    bool RemoveBlock(const glm::ivec3& worldPos);
    bool PlaceBlock(const glm::ivec3& worldPos, Cube::Type type);

private:
    std::vector<Chunk<16, 16, 16>> m_chunks;
    CubePalette& m_palette;
    PerlinNoise m_rng;
    glm::vec2 m_origin;

    Chunk<16, 16, 16>& GetChunkAt(const glm::ivec3& worldPos);
    glm::ivec3 WorldToLocalPos(const glm::ivec3& worldPos) const;
};