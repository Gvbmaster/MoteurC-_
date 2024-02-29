#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::addComponent(Component* component) 
{
    m_components.push_back(component);
}

Component* GameObject::getComponent(ComponentType type) {
    for (auto& component : m_components) {
        // Vous devrez ajouter une méthode getType() à la classe Component pour obtenir le type de chaque composant
        // Par exemple :
        // if (component->getType() == type) {
        //     std::cout << "Le composant a été trouvé!" << std::endl;
        //     return component;
        // }
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