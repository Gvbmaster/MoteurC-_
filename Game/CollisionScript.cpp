//// CollisionScript.cpp
//#include "CollisionScript.h"
//#include "../Moteur/Collider.h"
//
//
//void CollisionScript::handleCollision(GameObject* otherObject) {
//    // Code pour g�rer la collision avec l'autre objet
//}
//
//void CollisionScript::update(float deltaTime) {
//    // Obtenez le collider attach� � cet objet
//    Collider* myCollider = getGameObject()->getComponent<Collider>(ComponentType::Collider);
//
//    // V�rifiez s'il y a collision avec d'autres objets
//    for (GameObject* otherObject : getGameObject()->getNearbyObjects()) {
//        Collider* otherCollider = otherObject->getComponent<Collider>(ComponentType::Collider);
//        if (otherCollider && myCollider->collides(otherCollider)) {
//            // Collision d�tect�e, effectuez les actions appropri�es
//            handleCollision(otherObject);
//        }
//    }
//}