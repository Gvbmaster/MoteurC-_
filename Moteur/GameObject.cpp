#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

int GameObject::GetX()
{
    return m_x;
}

int GameObject::GetY()
{
    return m_y;
}

int GameObject::GetZ()
{
    return m_z;
}

void GameObject::addComponent(Component* component) 
{
    m_components.push_back(component);
}

Component* GameObject::getComponent(ComponentType type) {
    for (auto& component : m_components) {
        if (component->getType() == type) {
			return component;
		}
    }
    std::cout << "Le composant n'a pas été trouvé!" << std::endl;
    return nullptr;
}



void GameObject::update(float deltaTime) {
	for (auto& component : m_components) {
		component->update(deltaTime);
	}
}

//void GameObject::draw(Renderer& renderer) {
//	for (auto& component : m_Components) {
//		component->draw(renderer);
//	}
//}