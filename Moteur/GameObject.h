#pragma once
#include "../Common/d3dUtil.h"
#include <iostream>
#include "Component.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    void addComponent(Component* component);
    Component* getComponent(ComponentType type);
    void update(float deltaTime);

private:
    std::vector<Component*> m_components;
};

