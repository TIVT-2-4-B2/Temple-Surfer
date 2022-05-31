#pragma once

#include "DrawComponent.h"
#include "TextureComponent.h"

#include <List>
#include <Memory>
#include <Vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"

class OBJComponent : public DrawComponent {
private:
	// This holds the vertices.
	class ObjectGroup {
	public:
		std::string name;					// Object group name
		tigl::VBO* bufferedObjectVertices;	// List of vertices
		int materialIndex;					// Index for the texture file
	};

	// Holds the texture data.
	class MaterialInfo {
	public:
		MaterialInfo();
		std::string name;
		std::shared_ptr<TextureComponent> texture;
	};

	// Holds the object
	std::vector<std::shared_ptr<ObjectGroup>> groups;
	std::vector<std::shared_ptr<MaterialInfo>> materials;

	// Loads in the texture data.
	void loadMaterialFile(const std::string& fileName, const std::string& dirName);

public:

public:
	OBJComponent(const std::string& fileName);
	~OBJComponent();

	virtual void draw() override;
};