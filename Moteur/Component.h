#pragma once
//#include "../Common/d3dUtil.h"
//#include "GameObject.h"
//#include "Renderer.h"
#include "Transform.h"

enum class ComponentType {
    Transform,
    Renderer,
};

class Component {
public:
    Component();
    virtual ~Component();

    virtual void init();
    virtual void update(float deltaTime);
    // virtual void draw(Renderer& renderer);
    // virtual ComponentType getType() const = 0;

    Transform& getTransform() { return m_transform; }

protected:
    Transform m_transform;
};
