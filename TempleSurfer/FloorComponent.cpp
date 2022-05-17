#include "FloorComponent.h"



FloorComponent::FloorComponent(float size)
{
	glm::vec3 s(size);
	std::vector<glm::vec4> colorVecList; //Create a vector for every colored lane
	colorVecList.push_back(glm::vec4(255, 0, 0, 1)); //Red
	colorVecList.push_back(glm::vec4(0, 255, 0, 1)); //Green
	colorVecList.push_back(glm::vec4(0, 0, 255, 1)); //Blue
	const float offset = (10.0f * (2.0f / 3.0f));
	for (size_t i = 0; i < 3; i++) //For loop to make every seperate lane.
	{
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (size / 1.5), 0, 0) + glm::vec3(-s.x / 3, 0, -s.z * 1), colorVecList[i], glm::vec2(0, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (size / 1.5), 0, 0) + glm::vec3(s.x / 3, 0, -s.z * 1), colorVecList[i], glm::vec2(1, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (size / 1.5), 0, 0) + glm::vec3(s.x / 3, 0, s.z * 1), colorVecList[i], glm::vec2(1, 1), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (size / 1.5), 0, 0) + glm::vec3(-s.x / 3, 0, s.z * 1), colorVecList[i], glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	}

}

FloorComponent::~FloorComponent()
{
}

void FloorComponent::draw()
{
	tigl::drawVertices(GL_QUADS, verts);
}