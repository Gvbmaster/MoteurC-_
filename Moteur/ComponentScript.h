#pragma once

#include "Component.h"

class ComponentScript : public Component {
public:
    ComponentScript();
    virtual ~ComponentScript();

    virtual void init(GameObject* gameObject);
    virtual void update(float deltaTime);
};