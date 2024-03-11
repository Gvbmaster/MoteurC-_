#pragma once
#include "../Moteur/ComponentScript.h"

class CollisionScript : public ComponentScript {
public:
    CollisionScript(GameObject* gameObject) : ComponentScript(gameObject) {}
	virtual ~CollisionScript() {}

    virtual void handleCollision(GameObject* otherObject) = 0;

    void update(float deltaTime) override;
};
