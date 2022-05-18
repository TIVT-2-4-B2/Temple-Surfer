#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

#define FLOORWIDTH 10.0f

class FloorComponent : public DrawComponent
{
	std::vector<Vertex> verts;

public:
	FloorComponent();
	~FloorComponent();

	virtual void draw() override;
};