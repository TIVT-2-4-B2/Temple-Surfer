#include "ChunkGenerator.h"
#include <Time.h>

using enum ChunkObstacle;

void ChunkGenerator::generatorInit()
{
	// Seeds the random function
	srand(time(NULL));

	// Loading the all possible presets
	// TODO make with file IO. Temp solution right here..
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
				{{BLOCK},{BLOCK},{NONE}},
				{{NONE},{NONE},{DUCK}}}});
	presets.push_back({ {{{NONE},{NONE},{BLOCK}},
			{{BLOCK},{NONE},{NONE}},
			{{NONE},{NONE},{BLOCK}}} });
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
			{{NONE},{BLOCK},{BLOCK}},
			{{NONE},{NONE},{BLOCK}}} });
	presets.push_back({ {{{NONE},{JUMP},{BLOCK}},
			{{BLOCK},{BLOCK},{NONE}},
			{{JUMP},{NONE},{JUMP}}} });

	//Building chunks with all presets.
	for (auto& preset : presets)
	{
		chunkPointers.push_back(buildChunk(preset));
	}
}

std::shared_ptr<GameChunk> ChunkGenerator::getChunk()
{
	// Returning a random chunk that is generated.
	return chunkPointers.at(rand() % chunkPointers.size());
}

std::shared_ptr<GameChunk> ChunkGenerator::buildChunk(ChunkPreset preset)
{
	return nullptr;
}
