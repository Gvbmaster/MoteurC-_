#include "pch.h"
#include "Component.h"
#include "GameObject.h"


Component::Component(GameObject* gameObject) : m_gameObject(gameObject) {
}

Component::~Component() {
}

ComponentType Component::getType() const {
	return m_type;
}
void Component::init() {
}

void Component::update(float detalTime) {
}

// void Component::draw(Renderer& renderer) {
// 	renderer.draw(*this);
// }

Transform* Component::getTransform()
{
	return &m_gameObject->getTransform();
}
