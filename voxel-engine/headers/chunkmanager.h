#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#define SCX 16
#define SCY 16
#define SCZ 16

#include "chunk.h"
#include "main.h"

class ChunkManager {
private:
	Chunk* chunks[SCX][SCY][SCZ];
public:
	ChunkManager();
	~ChunkManager();
	uint8_t Get(int x, int y, int z);
	void Set(int x, int y, int z, uint8_t type);
	void Render(Shader shader);
};
#endif