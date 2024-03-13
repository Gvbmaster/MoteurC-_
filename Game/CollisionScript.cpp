//// CollisionScript.cpp
//#include "CollisionScript.h"
//#include "../Moteur/Collider.h"
//
//
//void CollisionScript::handleCollision(GameObject* otherObject) {
//    // Code pour gérer la collision avec l'autre objet
//}
//
//void CollisionScript::update(float deltaTime) {
//    // Obtenez le collider attaché à cet objet
//    Collider* myCollider = getGameObject()->getComponent<Collider>(ComponentType::Collider);
//
//    // Vérifiez s'il y a collision avec d'autres objets
//    for (GameObject* otherObject : getGameObject()->getNearbyObjects()) {
//        Collider* otherCollider = otherObject->getComponent<Collider>(ComponentType::Collider);
//        if (otherCollider && myCollider->collides(otherCollider)) {
//            // Collision détectée, effectuez les actions appropriées
//            handleCollision(otherObject);
//        }
//    }
//}