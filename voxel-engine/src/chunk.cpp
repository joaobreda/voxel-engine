#include "../headers/chunk.h"

Chunk::Chunk(int i, int j, int k) {
    memset(blk, 0, sizeof(blk));
    elements = 0;
    changed = true;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Setting chunk position relative to map
    posX = i * CX;
    posY = j * CY;
    posZ = k * CZ;
    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                blk[x][y][z] = j == CX-1 && x == 5 && y == CY-1 ? 0 : 1;
            }
        }
    }
}

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

uint8_t Chunk::GetNeighbourBlock(Chunk* neighbour, int x, int y, int z) {
    if (neighbour)
        return neighbour->blk[x][y][z];
    return 0;
}

// Where this chunk's mesh is created
void Chunk::Update() {
    changed = false;

    byte4 vertex[CX * CY * CZ * 6 * 6];
    // byte4* vertex = new byte4[CX * CY * CZ * 6 * 6];
    int i = 0;

    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                uint8_t type = blk[x][y][z];

                // Empty block?
                if (!type)
                    continue;

                // Culling unvisible faces below

                // View from negative x
                if (x == 0 && !GetNeighbourBlock(neighXN, CX - 1, y, z)) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                }
                else if (x > 0 && !blk[x - 1][y][z]) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                }

                // View from positive x
                if (x == CX - 1 && !GetNeighbourBlock(neighXP, 0, y, z)) {
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                }
                else if (x < CX - 1 && !blk[x + 1][y][z]) {
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                }

                // View from negative y
                if (y == 0 && !GetNeighbourBlock(neighYN, x, CY - 1, z)) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                }
                else if (y > 0 && !blk[x][y - 1][z]) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x, y, z + 1, type);
                }

                // View from positive y
                if (y == CY - 1 && !GetNeighbourBlock(neighYP, x, 0, z)) {
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                }
                else if (y < CY - 1 && !blk[x][y + 1][z]) {
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                }

                // View from negative z
                if (z == 0 && !GetNeighbourBlock(neighZN, x, y, CZ - 1)) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                }
                else if (z > 0 && !blk[x][y][z - 1]) {
                    vertex[i++] = byte4(x, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                    vertex[i++] = byte4(x, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y + 1, z, type);
                    vertex[i++] = byte4(x + 1, y, z, type);
                }

                // View from positive z
                if (z == CZ - 1 && !GetNeighbourBlock(neighZP, x, y, 0)) {
                    vertex[i++] = byte4(x, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x, y + 1, z + 1, type);
                    vertex[i++] = byte4(x + 1, y, z + 1, type);
                    vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                }
                else if (z < CZ - 1 && !blk[x][y][z + 1]) {
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

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, elements);
}

void Chunk::setNeighbours(Chunk* cXN, Chunk* cXP, Chunk* cYN, Chunk* cYP, Chunk* cZN, Chunk* cZP) {
    // If neighbours changed, we need to rebuild our mesh
    if (neighXN != cXN) {
        neighXN = cXN;
        changed = true;
    }
    if (neighXP != cXP) {
        neighXP = cXP;
        changed = true;
    }
    if (neighYN != cYN) {
        neighYN = cYN;
        changed = true;
    }
    if (neighYP != cYP) {
        neighYP = cYP;
        changed = true;
    }
    if (neighZN != cZN) {
        neighZN = cZN;
        changed = true;
    }
    if (neighZP != cZP) {
        neighZP = cZP;
        changed = true;
    } 
}