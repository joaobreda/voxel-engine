#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#define SCX 16
#define SCY 16
#define SCZ 16

#include "chunk.h"
#include "main.h"
#include <math.h>

using ChunkMap = std::unordered_map<glm::vec3, std::shared_ptr<Chunk>,
	std::hash<glm::vec3>, std::equal_to<glm::vec3>>;
using ChunkIterator = std::unordered_map<glm::vec3, std::shared_ptr<Chunk>,
	std::hash<glm::vec3>, std::equal_to<glm::vec3>>::iterator;

class ChunkManager {
public:
	ChunkMap chunks;
	ChunkManager();
	~ChunkManager();
	inline ChunkIterator begin() { return chunks.begin(); }
	inline ChunkIterator end() { return chunks.end(); }
	inline std::shared_ptr<Chunk> GetChunk(const glm::vec3& pos) {
		if (chunks[pos] != nullptr)
			return chunks[pos];
		return nullptr;
	}
	void Render(Shader shader);
};
#endif