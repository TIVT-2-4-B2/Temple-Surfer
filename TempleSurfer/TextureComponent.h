#pragma once

#include "Component.h"
#include <gl/glew.h>
#include <string>

class TextureComponent : public Component
{
	GLuint id;
public:
	TextureComponent(const std::string& fileName);
	void bind();
};
