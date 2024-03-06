#pragma once

#include <vector>
#include <iostream>
#include "Transform.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    void addComponent(Component* component);
    Component* getComponent(ComponentType type);
    void update(float deltaTime);
    // void draw(Renderer& renderer);
    Transform& getTransform() { return m_transform; }

private:
    std::vector<Component*> m_components;
    Transform m_transform;
};