#pragma once
#include "../Moteur/ComponentScript.h"

class PlayerScript : public ComponentScript {
public:
    PlayerScript(GameObject* gameObject);
    virtual ~PlayerScript();

    void init() override;
    void update(float deltaTime) override;
    // Autres méthodes spécifiques au joueur
};
