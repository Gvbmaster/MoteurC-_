#pragma once

#include <vector>
#include <iostream>
#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    void addComponent(Component* component);
    Component* getComponent(ComponentType type);
    void update(float deltaTime);

private:
    std::vector<Component*> m_components;
    Transform m_transform;
};
