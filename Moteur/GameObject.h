#pragma once

#include <vector>
#include <iostream>
#include "Transform.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    float GetX();
    float GetY();
    float GetZ();

    void addComponent(Component* component);
    void update(float deltaTime);
    Component* getComponent(ComponentType type);
    Transform& getTransform();


private:
    std::vector<Component*> m_components;
    Transform m_transform;
};
