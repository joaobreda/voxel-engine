#include "../headers/chunkmanager.h"

extern Camera camera;

ChunkManager::ChunkManager() {
	memset(chunks, 0, sizeof chunks);
    for (int x = 0; x < SCX; x++)
        for (int y = 0; y < SCY; y++)
            for (int z = 0; z < SCZ; z++)
                chunks[x][y][z] = new Chunk(x, y, z, 
                    x > 0 ? &chunks[x-1][y][z] : nullptr, 
                    x < SCX - 1 ? &chunks[x+1][y][z] : nullptr,
                    y > 0 ? &chunks[x][y-1][z] : nullptr, 
                    y < SCY - 1 ? &chunks[x][y+1][z] : nullptr,
                    z > 0 ? &chunks[x][y][z-1] : nullptr,
                    z < SCZ - 1 ? &chunks[x][y][z+1] : nullptr);
}

ChunkManager::~ChunkManager() {
    for (int x = 0; x < SCX; x++)
        for (int y = 0; y < SCX; y++)
            for (int z = 0; z < SCX; z++)
                delete chunks[x][y][z];
}

uint8_t ChunkManager::Get(int x, int y, int z) {
    int cx = x / CX;
    int cy = y / CY;
    int cz = z / CZ; 

    x %= CX;
    y %= CY;
    z %= CZ;

    if (!chunks[cx][cy][cz])
        return 0;
    else
        return chunks[cx][cy][cz]->Get(x, y, z);
}

void ChunkManager::Set(int x, int y, int z, uint8_t type) {
    int cx = x / CX;
    int cy = y / CY;
    int cz = z / CZ;

    x %= CX;
    y %= CY;
    z %= CZ;

    if (!chunks[cx][cy][cz])
        chunks[cx][cy][cz] = new Chunk();

    chunks[cx][cy][cz]->Set(x, y, z, type);
}

void ChunkManager::Render(Shader shader) {
    for (int x = 0; x < SCX; x++)
        for (int y = 0; y < SCY; y++)
            for (int z = 0; z < SCZ; z++)
                if (chunks[x][y][z]) {
                    // changing each chunk to their position so they aren't stacked on top of eachother
                    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunks[x][y][z]->posX, chunks[x][y][z]->posY, chunks[x][y][z]->posZ));
                    shader.setMat4("model", model);

                    chunks[x][y][z]->Render();
                }
}