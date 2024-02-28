#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::addComponent(Component* component)
{
	m_Components.push_back(component);
}

void GameObject::getComponent(Component* component)
{
    for (int i = 0; i < m_Components.size(); i++)
    {
        if (m_Components[i] == component)
        {
            std::cout << "Le composant a été trouvé!" << std::endl;
            return;
        }
    }
    std::cout << "Le composant n'a pas été trouvé!" << std::endl;
}


void GameObject::update(float deltaTime) {
	for (auto& component : m_Components) {
		component->update(deltaTime);
	}
}

void GameObject::draw(Renderer& renderer) {
	for (auto& component : m_Components) {
		component->draw(renderer);
	}
}