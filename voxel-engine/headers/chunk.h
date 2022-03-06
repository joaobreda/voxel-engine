#ifndef CHUNK_H
#define CHUNK_H

#define CX 16
#define CY 16
#define CZ 16

#include <glad/glad.h> 
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <unordered_map>
#include "glm/gtx/hash.hpp"

typedef glm::tvec4<GLbyte> byte4;

class Chunk {
private:
    // uint8_t = 1 byte (type of unsigned integer of length 8 bits)
    uint8_t blk[CX][CY][CZ];
    unsigned int VBO, VAO;
    int elements;
    bool changed;
public:
    // Global map position of this chunk 
    int posX, posY, posZ;
    // Reference chunkmap
    using ChunkMap = std::unordered_map<glm::vec3, std::shared_ptr<Chunk>,
        std::hash<glm::vec3>, std::equal_to<glm::vec3>>;
    ChunkMap chunks;
    Chunk(int i, int j, int k, const ChunkMap &chunks_m);
    ~Chunk();
    uint8_t Get(int x, int y, int z);
    void Set(int x, int y, int z, uint8_t type);
    void Update();
    void Render();
};
#endif