#pragma once

#include <vector>
#include <iostream>
#include "Transform.h"
#include "Component.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    int GetX();
    int GetY();
    int GetZ();

    void addComponent(Component* component);
    Component* getComponent(ComponentType type);
    void update(float deltaTime);
    // void draw(Renderer& renderer);
    Transform& getTransform() { return m_transform; }

private:
    std::vector<Component*> m_components;
    Transform m_transform;

    float m_x;
    float m_y;
    float m_z;
};