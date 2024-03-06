#include "pch.h"
#include "Component.h"

Component::Component(GameObject* gameObject) : m_gameObject(gameObject) {
}

Component::~Component() {
}

ComponentType Component::getType() const {
	return ComponentType::Transform;
}

void Component::init() {
}

void Component::update(float detalTime) {
}

// void Component::draw(Renderer& renderer) {
// 	renderer.draw(*this);
// }