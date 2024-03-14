#pragma once
#include "../Moteur/ComponentScript.h"

class PlayerScript : public ComponentScript {
public:
    PlayerScript();
    virtual ~PlayerScript();

    void init(GameObject* gameObject) override;
    void update(float deltaTime) override;
    // Autres m�thodes sp�cifiques au joueur
};
