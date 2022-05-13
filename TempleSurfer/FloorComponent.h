#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

class FloorComponent : public DrawComponent
{
	std::vector<Vertex> verts;

public:
	FloorComponent(float size);
	~FloorComponent();

	virtual void draw() override;
};