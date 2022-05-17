#include "ChunkGenerator.h"
#include <Time.h>

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
}

GameChunk ChunkGenerator::generateChunk()
{
	// Choosing one of the presets
	ChunkPreset preset = presets.at(rand() % presets.size());

	// Building chunk with preset.


	// Returning the generated chunk.
	
}
