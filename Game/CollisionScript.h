#pragma once
#include "../Moteur/ComponentScript.h"

class CollisionScript : public ComponentScript {
public:
    CollisionScript(GameObject* gameObject) : ComponentScript(gameObject) {}
	virtual ~CollisionScript() {}

    void update(float deltaTime) override;
};
