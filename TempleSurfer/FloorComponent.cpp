#include "FloorComponent.h"



FloorComponent::FloorComponent()
{
	glm::vec3 s(FLOOR_WIDTH);
	std::vector<glm::vec4> colorVecList; //Create a vector for every colored lane
	colorVecList.push_back(glm::vec4(255, 0, 0, 1)); //Red
	colorVecList.push_back(glm::vec4(0, 255, 0, 1)); //Green
	colorVecList.push_back(glm::vec4(0, 0, 255, 1)); //Blue
	const float offset = (FLOOR_WIDTH * (2.0f / 3.0f));
	for (size_t i = 0; i < 3; i++) //For loop to make every seperate lane.
	{
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(-s.x / 3, 0, FLOOR_LENGTH), colorVecList[i], glm::vec2(0, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(s.x / 3, 0, FLOOR_LENGTH), colorVecList[i], glm::vec2(1, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(s.x / 3, 0, 0), colorVecList[i], glm::vec2(1, 1), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(-s.x / 3, 0, 0), colorVecList[i], glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	}
	//Adding sides
	verts.push_back(Vertex::PCTN(glm::vec3(FLOOR_WIDTH, 0, FLOOR_LENGTH), colorVecList[1], glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(FLOOR_WIDTH + 8, -4, FLOOR_LENGTH), colorVecList[1], glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(FLOOR_WIDTH + 8, -4, 0), colorVecList[1], glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(FLOOR_WIDTH, 0, 0), colorVecList[1], glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	verts.push_back(Vertex::PCTN(glm::vec3(-FLOOR_WIDTH, 0, FLOOR_LENGTH), colorVecList[1], glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(-FLOOR_WIDTH - 8, -4, FLOOR_LENGTH), colorVecList[1], glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(-FLOOR_WIDTH - 8, -4, 0), colorVecList[1], glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(-FLOOR_WIDTH, 0, 0), colorVecList[1], glm::vec2(0, 1), glm::vec3(0, -1, 0)));


}

FloorComponent::~FloorComponent()
{
}

void FloorComponent::draw()
{
	tigl::drawVertices(GL_QUADS, verts);
}