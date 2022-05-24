#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

class PaneComponent : public DrawComponent
{
	std::vector<Vertex> verts;

public:
	PaneComponent(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec4 color);
	~PaneComponent();

	virtual void draw() override;
};
