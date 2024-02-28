#pragma once
#include "../Common/d3dUtil.h"
#include "GameObject.h"
#include "Renderer.h"

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
    // virtual void draw(Renderer& renderer); // D�commentez si n�cessaire
    // virtual ComponentType getType() const = 0; // D�commentez si n�cessaire
};
