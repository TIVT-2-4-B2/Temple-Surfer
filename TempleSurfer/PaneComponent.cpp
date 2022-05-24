#include "PaneComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"

PaneComponent::PaneComponent(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec4 color)
{
	verts.push_back(Vertex::PCTN(v1, color, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(v2, color, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(v3, color, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(v4, color, glm::vec2(0, 1), glm::vec3(0, 1, 0)));
}

PaneComponent::~PaneComponent()
{
}

void PaneComponent::draw()
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