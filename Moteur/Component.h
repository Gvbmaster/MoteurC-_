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
    // virtual void draw(Renderer& renderer); // Décommentez si nécessaire
    // virtual ComponentType getType() const = 0; // Décommentez si nécessaire
};
