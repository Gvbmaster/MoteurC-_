#include "pch.h"
#include "Component.h"

Component::Component(const Component& other) {
	m_transform = other.m_transform;
}

Component::~Component() {
}

ComponentType Component::getType() const {
	return ComponentType::Transform;
}

void Component::init() {
    m_transform.Identity();
}

void Component::update(float deltaTime) {
    m_transform.UpdateMatrix();
}

// void Component::draw(Renderer& renderer) {
// }