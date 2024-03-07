#pragma once
#include "../Moteur/ComponentScript.h"

class ProjectileScript : public ComponentScript {
public:
    ProjectileScript(GameObject* gameObject);
    virtual ~ProjectileScript();

    void init() override;
    void update(float deltaTime) override;
    // Autres méthodes spécifiques aux projectiles
};

