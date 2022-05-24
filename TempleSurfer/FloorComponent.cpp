#include "FloorComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"

FloorComponent::FloorComponent()
{
	std::vector<glm::vec4> colorVecList; //Create a vector for every colored lane
	colorVecList.push_back(glm::vec4(255, 0, 0, 1)); //Red
	colorVecList.push_back(glm::vec4(0, 255, 0, 1)); //Green
	colorVecList.push_back(glm::vec4(0, 0, 255, 1)); //Blue
	const float offset = (FLOOR_WIDTH * (2.0f / 3.0f));
	for (size_t i = 0; i < 3; i++) //For loop to make every seperate lane.
	{
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(-FLOOR_WIDTH / 3, 0, FLOOR_LENGTH + FLOOR_OVERLAP), colorVecList[i], glm::vec2(0, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(FLOOR_WIDTH / 3, 0, FLOOR_LENGTH + FLOOR_OVERLAP), colorVecList[i], glm::vec2(1, 0), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(FLOOR_WIDTH / 3, 0, 0), colorVecList[i], glm::vec2(1, 1), glm::vec3(0, -1, 0)));
		verts.push_back(Vertex::PCTN(glm::vec3(-offset + i * (FLOOR_WIDTH / 1.5), 0, 0) + glm::vec3(-FLOOR_WIDTH / 3, 0, 0), colorVecList[i], glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	}
}

FloorComponent::~FloorComponent()
{
}

void FloorComponent::draw()
{
	std::shared_ptr<TextureComponent> tex = gameObject->getComponent<TextureComponent>();
	if (tex != NULL)
	{
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(true);
		tex->bind();
	}
	tigl::drawVertices(GL_QUADS, verts);
	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);
}