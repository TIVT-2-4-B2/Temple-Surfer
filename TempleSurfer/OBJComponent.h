#pragma once

#include "DrawComponent.h"
#include "TextureComponent.h"

#include <List>
#include <Memory>
#include <Vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OBJComponent : public DrawComponent {
private:
	// Used to create the vertex list for VBO
	class VertexIndex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class FaceCollection
	{
	public:
		std::list<VertexIndex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		std::shared_ptr<TextureComponent> texture;
	};

	// Used for rendering the VBO.
	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<FaceCollection> faces;
	/*	VBO* vbo;*/
	};


	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<std::shared_ptr<ObjGroup>> groups;
	std::vector<std::shared_ptr<MaterialInfo>> materials;

	void loadMaterialFile(const std::string& fileName, const std::string& dirName);

public:
	OBJComponent(const std::string& fileName);
	~OBJComponent();

	virtual void draw() override;
};