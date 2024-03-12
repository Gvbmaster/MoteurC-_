#pragma once

#include "GameObject.h"

enum class ComponentType {
    Transform,
    Renderer,
    Script,
    Collider,
};

class Component {
public:
    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void init();
    virtual void update(float deltaTime);
    virtual ComponentType getType() const = 0;

protected:
    GameObject* m_gameObject;
    Transform* getTransform() { return &m_gameObject->getTransform(); }
};
