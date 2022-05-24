#pragma once

#include "DrawComponent.h"
#include "TextureComponent.h"

#include <List>
#include <Memory>
#include <Vector>
#include <glm/glm.hpp>

class OBJComponent : DrawComponent {
private:
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

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<FaceCollection> faces;
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
	virtual void draw();
};