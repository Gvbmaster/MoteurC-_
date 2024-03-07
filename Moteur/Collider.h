// Collider.h
#pragma once
#include "Component.h"

class Collider : public Component {
public:
    Collider(GameObject* gameObject) : Component(gameObject) {}
    ~Collider();
    virtual bool Collides(Collider* other) = 0;
private:

};