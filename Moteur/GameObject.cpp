#include "pch.h"
#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

float GameObject::GetX()
{
    return m_transform.vPos.x;
}

float GameObject::GetY()
{
    return m_transform.vPos.y;
}

float GameObject::GetZ()
{
    return m_transform.vPos.z;
}

void GameObject::addComponent(Component* component) 
{
    m_components.push_back(component);
}

Component* GameObject::getComponent(ComponentType type) {
    for (Component* component : m_components) {
        if (component->getType() == type) {
            return component;
        }
    }
    return nullptr;
}

Transform& GameObject::getTransform()
{
	return m_transform;
}

void GameObject::update(float deltaTime) {
	for (auto& component : m_components) {
		component->update(deltaTime);
	}
}