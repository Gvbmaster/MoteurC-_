#include "Component.h"

Component::Component() {
}

Component::~Component() {
}

void Component::init() {
    m_transform.Identity();
}

void Component::update(float deltaTime) {
    m_transform.UpdateMatrix();
}
