#include "../include/World.hpp"

World::World(const glm::vec2& origin, CubePalette& palette, const PerlinNoise& rng)
    : m_origin(origin), m_palette(palette), m_rng(rng) {
    // Utwórz 9 chunków (3x3)
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            glm::vec2 chunkOrigin = origin + glm::vec2(x * 16, z * 16); // Każdy chunk ma rozmiar 16x16
            m_chunks.emplace_back(chunkOrigin, palette);
            m_chunks.back().Generate(rng);
        }
    }
}

void World::Draw(ShaderProgram& shader) const {
    for (const auto& chunk : m_chunks) {
        chunk.Draw(shader);
    }
}

bool World::RemoveBlock(const glm::ivec3& worldPos) {
    Chunk<16, 16, 16>& chunk = GetChunkAt(worldPos);
    glm::ivec3 localPos = WorldToLocalPos(worldPos);
    return chunk.RemoveBlock(localPos.x, localPos.y, localPos.z);
}

bool World::PlaceBlock(const glm::ivec3& worldPos, Cube::Type type) {
    Chunk<16, 16, 16>& chunk = GetChunkAt(worldPos);
    glm::ivec3 localPos = WorldToLocalPos(worldPos);
    return chunk.PlaceBlock(localPos.x, localPos.y, localPos.z, type);
}

Chunk<16, 16, 16>& World::GetChunkAt(const glm::ivec3& worldPos) {
    int chunkX = static_cast<int>(worldPos.x) / 16;
    int chunkZ = static_cast<int>(worldPos.z) / 16;
    size_t index = (chunkX + 1) * 3 + (chunkZ + 1);
    return m_chunks[index];
}

glm::ivec3 World::WorldToLocalPos(const glm::ivec3& worldPos) const {
    return glm::ivec3(
        worldPos.x % 16,
        worldPos.y,
        worldPos.z % 16
    );
}