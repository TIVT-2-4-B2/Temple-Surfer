#pragma once

#include "DrawComponent.h"
#include "TextureComponent.h"

#include <List>
#include <Memory>
#include <Vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <queue>
#include "tigl.h"
#include <mutex>
#include <map>


class OBJComponent : public DrawComponent {
private:
	// Animation data
	float animationDelay = 0;
	float animationTime = 0;
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

public:
	// Holds a object file
	class ObjectFile {
	public:
		// Holds the object
		std::vector<std::shared_ptr<ObjectGroup>> groups;
		std::vector<std::shared_ptr<MaterialInfo>> materials;
		int animationIndex;
	};

private: 
	// Object that is created when building to communicate with gl thread.
	class ObjectBuilder {
	public:
		tigl::VBO* asyncObjectVBOCall(std::vector<tigl::Vertex> vertices);
		std::shared_ptr<TextureComponent> asyncObjectTextureCall(std::string path);
		void awaitObjectGLCall();
	private:
		// Used for operation safety on objects.
		std::mutex buildLock;
		bool inputGiven = false;
		bool outputGiven = false;
		int operation = -1;

		// Used for VBO create calls.
		std::vector<tigl::Vertex> verticesRequest;
		tigl::VBO* vboResponse;

		// Used for texure create calls.
		std::string pathRequest;
		std::shared_ptr<TextureComponent> textureResponse;
	};

	// Holds the amount of working threads.
	int amountWorkers = 0;
	
	// Holds the information about the object
	std::mutex objectDataLock;
	std::shared_ptr<std::vector<std::shared_ptr<ObjectFile>>> objectData = std::make_shared<std::vector<std::shared_ptr<ObjectFile>>>();

	// Loads in the texture data.
	void loadObjectFile(const std::string fileName, std::shared_ptr<ObjectBuilder> context, int listIndex);
	void loadMaterialFile(const std::string& fileName, const std::string& dirName, std::shared_ptr<ObjectFile>& file, std::shared_ptr<ObjectBuilder> context);
	void objectDrawer(std::shared_ptr<ObjectFile> file);
public:
	OBJComponent(const std::string& fileName);
	OBJComponent(const std::string& folderName, float animationDelay);
	~OBJComponent();

	virtual void draw() override;
	virtual void update(float elapsedTime) override;
};

// Global private caching map.
static std::map<std::string, std::shared_ptr<std::vector<std::shared_ptr<OBJComponent::ObjectFile>>>> cachedObjects;