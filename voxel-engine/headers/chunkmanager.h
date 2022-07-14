#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

// Number of chunks rendered around the player
#define SCX 16
#define SCY 16
#define SCZ 16

#include "chunk.h"
#include "main.h"
#include <math.h>
#include <memory>
#include <unordered_map>
#include "glm/gtx/hash.hpp"

using ChunkMap = std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>>;

class ChunkManager {
private:
	glm::vec3 lastPlayerPos;
public:
	// Hashmap where we store visible chunks
	ChunkMap chunks;
	ChunkManager();
	~ChunkManager();
	void Update();
	void Render(Shader shader);
};
#endif