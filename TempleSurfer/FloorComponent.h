#pragma once

#include "DrawComponent.h"
#include "tigl.h"
using tigl::Vertex;

#include <vector>

#define FLOOR_WIDTH 10.0f
#define FLOOR_LENGTH 30.0f

class FloorComponent : public DrawComponent
{
	std::vector<Vertex> verts;

public:
	FloorComponent();
	~FloorComponent();

	virtual void draw() override;
};