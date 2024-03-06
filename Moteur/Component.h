#pragma once
//#include "../Common/d3dUtil.h"
//#include "GameObject.h"
//#include "Renderer.h"
#include "GameObject.h"

enum class ComponentType {
    Transform,
    Renderer,
};

class Component {
public:
    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void init();
    virtual void update(float deltaTime);
    // virtual void draw(Renderer& renderer);
    virtual ComponentType getType() const = 0;
    GameObject* getGameObject() { return m_gameObject; }
    Transform& getTransform() { return m_gameObject->getTransform(); }

protected:
    GameObject* m_gameObject;
};