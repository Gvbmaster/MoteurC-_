#include "pch.h"
#include "ComponentScript.h"


ComponentScript::ComponentScript() {
}
ComponentScript::~ComponentScript() {
}


void ComponentScript::init(GameObject* gameObject) {
	Component::init(gameObject);
}

void ComponentScript::update(float deltaTime) {
	Component::update(deltaTime);
}
