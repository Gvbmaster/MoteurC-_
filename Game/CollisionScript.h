#pragma once
#include "../Moteur/ComponentScript.h"

class CollisionScript : public ComponentScript {
public:
    CollisionScript();
    virtual ~CollisionScript();

    void init(GameObject* gameObject) override;
    virtual void handleCollision(GameObject* otherObject) = 0;

    void update(float deltaTime) override;
};
