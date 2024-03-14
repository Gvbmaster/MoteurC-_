#include "pch.h"
#include "Component.h"
#include "GameObject.h"


Component::Component() {
}

Component::~Component() {
}

void Component::init(GameObject* gameObject) {
	m_gameObject = gameObject;
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
