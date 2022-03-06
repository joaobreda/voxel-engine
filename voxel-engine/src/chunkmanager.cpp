#include "../headers/chunkmanager.h"

extern Camera camera;

ChunkManager::ChunkManager() {
    glm::vec3 key;
    int playerPosX = floor(camera.Position[0] / 16);
    int playerPosY = floor(camera.Position[1] / 16);
    int playerPosZ = floor(camera.Position[2] / 16);
    // Create chunks around the player
    for (int x = playerPosX+(-SCX/2); x <= playerPosX+SCX/2; x++)
        for (int y = playerPosY+(-SCY/2); y <= playerPosY+SCY/2; y++)
            for (int z = playerPosZ+(-SCZ/2); z <= playerPosZ+SCZ/2; z++) {
                key.x = x; key.y = y; key.z = z;
                chunks[key] = std::shared_ptr<Chunk>(new Chunk(x, y, z, chunks));
            }
}

ChunkManager::~ChunkManager() {
    for (ChunkIterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
        (iter)->second.reset();
    }
}

void ChunkManager::Render(Shader shader) {
    glm::vec3 pos;
    if (!chunks.empty()) {
        for (ChunkIterator iter = chunks.begin(); iter != chunks.end(); ++iter) {
            // changing each chunk to their position so they aren't stacked on top of eachother
            pos.x = (*iter).second->posX; pos.y = (*iter).second->posY; pos.z = (*iter).second->posZ;
            glm::mat4 model = glm::translate(glm::mat4(1), pos);
            shader.setMat4("model", model);

            (*iter).second->Render();
        }
    }
}