#pragma once
#include "../Moteur/ComponentScript.h"

class EnemyScript : public ComponentScript {
public:
    EnemyScript();
    virtual ~EnemyScript();

    void init(GameObject* gameObject) override;
    void update(float deltaTime) override;
    // Autres m�thodes sp�cifiques aux ennemis
};



