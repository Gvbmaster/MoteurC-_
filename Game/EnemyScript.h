#pragma once
#include "../Moteur/ComponentScript.h"

class EnemyScript : public ComponentScript {
public:
    EnemyScript(GameObject* gameObject);
    virtual ~EnemyScript();

    void init() override;
    void update(float deltaTime) override;
    // Autres méthodes spécifiques aux ennemis
};



