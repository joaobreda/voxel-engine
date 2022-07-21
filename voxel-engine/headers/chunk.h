#ifndef CHUNK_H
#define CHUNK_H

// Number of blocks inside a chunk
#define CX 16
#define CY 16
#define CZ 16

#include <glad/glad.h> 
#include <glm/gtc/type_ptr.hpp>
#include "noiseutils.h"
#include <iostream>
#include <sstream>
#include <string>

typedef glm::tvec4<GLbyte> byte4;

class Chunk {
private:
    // uint8_t = 1 byte | type of unsigned integer of length 8 bits
    uint8_t blk[CX][CY][CZ];
    unsigned int VBO, VAO;
    int elements;
    bool changed;
public:
    Chunk* neighXN, * neighXP, * neighYN, * neighYP, * neighZN, * neighZP;
    // Global map position of this chunk 
    int posX, posY, posZ;
    Chunk(int i, int j, int k);
    ~Chunk();
    uint8_t GetNeighbourBlock(Chunk* neighbour, int x, int y, int z);
    void Update();
    void Render();
    void setNeighbours(Chunk* neighXN, Chunk* neighXP, Chunk* neighYN, Chunk* neighYP, Chunk* neighZN, Chunk* neighZP);
};
#endif