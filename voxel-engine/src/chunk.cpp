#include "../headers/chunk.h"

Chunk::Chunk(int i, int j, int k, const ChunkMap &chunks_m) {
    memset(blk, 0, sizeof(blk));
    elements = 0;
    changed = true;
    glGenBuffers(1, &VBO);
    // setting chunk position relative to map
    posX = i * CX;
    posY = j * CY;
    posZ = k * CZ;
    // reference to all chunks
    chunks = chunks_m;
    // Putting blocks in all of the positions avaible for this chunk   
    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                Set(x, y, z, posY <= 0 ? 1 : 0);
            }
        }
    }
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &VBO);
}

uint8_t Chunk::Get(int x, int y, int z) {
    return blk[x][y][z];
}

void Chunk::Set(int x, int y, int z, uint8_t type) {
    blk[x][y][z] = type;
    changed = true;
}

// Where the mesh is created
void Chunk::Update() {
    changed = false;

    byte4 vertex[CX * CY * CZ * 6 * 6];
    int i = 0;

    // NEIGHBORS
    std::shared_ptr<Chunk> cXN = chunks[glm::vec3(posX / CX - 1, posY / CY, posZ / CZ)];
    std::shared_ptr<Chunk> cXP = chunks[glm::vec3(posX / CX + 1, posY / CY, posZ / CZ)];
    std::shared_ptr<Chunk> cYN = chunks[glm::vec3(posX / CX, posY / CY - 1, posZ / CZ)];
    std::shared_ptr<Chunk> cYP = chunks[glm::vec3(posX / CX, posY / CY + 1, posZ / CZ)];
    std::shared_ptr<Chunk> cZN = chunks[glm::vec3(posX / CX, posY / CY, posZ / CZ - 1)];
    std::shared_ptr<Chunk> cZP = chunks[glm::vec3(posX / CX, posY / CY, posZ / CZ + 1)];

    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                uint8_t type = blk[x][y][z];

                // Empty block?
                if (!type)
                    continue;

                // View from negative x
                if (cXN) {
                    if (!cXN->blk[CX - 1][y][z]) {
                        vertex[i++] = byte4(x, y, z, type);
                        vertex[i++] = byte4(x, y, z + 1, type);
                        vertex[i++] = byte4(x, y + 1, z, type);
                        vertex[i++] = byte4(x, y + 1, z, type);
                        vertex[i++] = byte4(x, y, z + 1, type);
                        vertex[i++] = byte4(x, y + 1, z + 1, type);
                    }
                }
                else if (x == 0 || (x > 0 && !blk[x - 1][y][z])) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                }

                // View from positive x
                if (cXP) {
                    if (x == CX - 1 && !cXP->blk[0][y][z]) {
                        vertex[i++] = byte4(x + 1, y, z, type);
                        vertex[i++] = byte4(x + 1, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y, z + 1, type);
                        vertex[i++] = byte4(x + 1, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                        vertex[i++] = byte4(x + 1, y, z + 1, type);
                    }
                }
                else if (x == CX - 1 || (x < CX - 1 && !blk[x + 1][y][z])) {
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                }

                // View from negative y
                if (cYN) {
                    if (y == 0 && !cYN->blk[x][CY - 1][z]) {
                        vertex[i++] = byte4(x, y, z, type);
                        vertex[i++] = byte4(x + 1, y, z, type);
                        vertex[i++] = byte4(x, y, z + 1, type);
                        vertex[i++] = byte4(x + 1, y, z, type);
                        vertex[i++] = byte4(x + 1, y, z + 1, type);
                        vertex[i++] = byte4(x, y, z + 1, type);
                    }
                }
                else if (y == 0 || (y > 0 && !blk[x][y - 1][z])) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                }

                // View from positive y
                if (cYP) {
                    if (y == CY - 1 && !cYP->blk[x][0][z]) {
                        vertex[i++] = byte4(x, y + 1, z, type);
                        vertex[i++] = byte4(x, y + 1, z + 1, type);
                        vertex[i++] = byte4(x + 1, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y + 1, z, type);
                        vertex[i++] = byte4(x, y + 1, z + 1, type);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                    }
                }
                else if (y == CY - 1 || (y < CY - 1 && !blk[x][y + 1][z])) {
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                }

                // View from negative z
                if (cZN) {
                    if (z == 0 && !cZN->blk[x][y][CZ - 1]) {
                        vertex[i++] = byte4(x, y, z, type);
                        vertex[i++] = byte4(x, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y, z, type);
                        vertex[i++] = byte4(x, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y + 1, z, type);
                        vertex[i++] = byte4(x + 1, y, z, type);
                    }
                }
                else if (z == 0 || (z > 0 && !blk[x][y][z - 1])) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                }

                // View from positive z
                if (cZP) {
                    if (z == CZ - 1 && !cZP->blk[x][y][0]) {
                        vertex[i++] = byte4(x, y, z + 1, type);
                        vertex[i++] = byte4(x + 1, y, z + 1, type);
                        vertex[i++] = byte4(x, y + 1, z + 1, type);
                        vertex[i++] = byte4(x, y + 1, z + 1, type);
                        vertex[i++] = byte4(x + 1, y, z + 1, type);
                        vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                    }
                }
                else if (z == CZ - 1 || (z < CZ - 1 && !blk[x][y][z + 1])) {
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                }
            }
        }
    }
    elements = i;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, elements * sizeof * vertex, vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, sizeof(byte4), 0);
    glEnableVertexAttribArray(0);
}

void Chunk::Render() {
    if (changed)
        Update();

    // If this chunk is empty, we don't need to draw anything.
    if (!elements)
        return;

    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, elements);
}