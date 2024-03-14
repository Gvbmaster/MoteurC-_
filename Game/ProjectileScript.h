#pragma once
#include "../Moteur/ComponentScript.h"

class ProjectileScript : public ComponentScript {
public:
    ProjectileScript();
    virtual ~ProjectileScript();

    void init(GameObject* gameObject) override;
    void update(float deltaTime) override;
    // Autres méthodes spécifiques aux projectiles
};

