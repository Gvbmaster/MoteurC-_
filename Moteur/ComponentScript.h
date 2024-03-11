#pragma once

#include "Component.h"

class ComponentScript : public Component {
public:
    ComponentScript(GameObject* gameObject) : Component(gameObject) {}
    virtual ~ComponentScript() {}

    virtual void init() {}
    virtual void update(float deltaTime) {}
};