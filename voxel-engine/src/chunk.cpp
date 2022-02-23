#include "../headers/chunk.h"

Chunk::Chunk() {
    memset(blk, 0, sizeof(blk));
    elements = 0;
    changed = true;
    glGenBuffers(1, &VBO);
    for (int x = 0; x < CX; x++)
    {
        for (int y = 0; y < CY; y++)
        {
            for (int z = 0; z < CZ; z++)
            {
                Set(x, y, z, 1);
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

void Chunk::Update() {
    changed = false;

    byte4 vertex[CX * CY * CZ * 6 * 6];
    int i = 0;

    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                uint8_t type = blk[x][y][z];

                // Empty block?
                if (!type)
                    continue;

                // View from negative x
                vertex[i++] = byte4(x, y, z, type);
                vertex[i++] = byte4(x, y, z + 1, type);
                vertex[i++] = byte4(x, y + 1, z, type);
                vertex[i++] = byte4(x, y + 1, z, type);
                vertex[i++] = byte4(x, y, z + 1, type);
                vertex[i++] = byte4(x, y + 1, z + 1, type);

                // View from positive x
                vertex[i++] = byte4(x + 1, y, z, type);
                vertex[i++] = byte4(x + 1, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y, z + 1, type);
                vertex[i++] = byte4(x + 1, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
                vertex[i++] = byte4(x + 1, y, z + 1, type);

                // View from negative y
                vertex[i++] = byte4(x, y, z, type);
                vertex[i++] = byte4(x + 1, y, z, type);
                vertex[i++] = byte4(x, y, z + 1, type);
                vertex[i++] = byte4(x + 1, y, z, type);
                vertex[i++] = byte4(x + 1, y, z + 1, type);
                vertex[i++] = byte4(x, y, z + 1, type);

                // View from positive y
                vertex[i++] = byte4(x, y + 1, z, type);
                vertex[i++] = byte4(x, y + 1, z + 1, type);
                vertex[i++] = byte4(x + 1, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y + 1, z, type);
                vertex[i++] = byte4(x, y + 1, z + 1, type);
                vertex[i++] = byte4(x + 1, y + 1, z + 1, type);

                // View from negative z
                vertex[i++] = byte4(x, y, z, type);
                vertex[i++] = byte4(x, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y, z, type);
                vertex[i++] = byte4(x, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y + 1, z, type);
                vertex[i++] = byte4(x + 1, y, z, type);

                // View from positive z
                vertex[i++] = byte4(x, y, z + 1, type);
                vertex[i++] = byte4(x + 1, y, z + 1, type);
                vertex[i++] = byte4(x, y + 1, z + 1, type);
                vertex[i++] = byte4(x, y + 1, z + 1, type);
                vertex[i++] = byte4(x + 1, y, z + 1, type);
                vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
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

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, elements);
}