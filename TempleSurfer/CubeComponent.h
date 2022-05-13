#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

class CubeComponent : public DrawComponent
{
	std::vector<Vertex> verts;
public:
	CubeComponent(glm::vec3 dimensions = glm::vec3(1,1,1), glm::vec4 color = glm::vec4(1,0,0,1));
	~CubeComponent();


	virtual void draw() override;
};

