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
	// Animation data
	float animationDelay;
	float animationTime;
	int animationIndex = 0;

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
		glm::vec4 specular;
		glm::vec4 ambient;
		glm::vec4 diffuse;
	};

	// Holds a object file
	class ObjectFile {
	public:
		// Holds the object
		std::vector<std::shared_ptr<ObjectGroup>> groups;
		std::vector<std::shared_ptr<MaterialInfo>> materials;
	};
	
	// Holds the information about the object
	std::vector<std::shared_ptr<ObjectFile>> objectData;

	// Loads in the texture data.
	void loadObjectFile(const std::string& fileName);
	void loadMaterialFile(const std::string& fileName, const std::string& dirName, std::shared_ptr<ObjectFile>& file);
	void objectDrawer(std::shared_ptr<ObjectFile> file);
public:
	OBJComponent(const std::string& fileName);
	OBJComponent(const std::string& folderName, float animationDelay);
	~OBJComponent();

	virtual void draw() override;
	virtual void update(float elapsedTime) override;
};