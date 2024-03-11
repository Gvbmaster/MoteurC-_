#include "pch.h"
#include "Collider.h"
#include <cmath> // Pour la fonction sqrt

Collider::Collider() {
}

Collider::~Collider() {
}

double Collider::dist2OBJ(GameObject OBJ1, GameObject OBJ2) {
    int OBJ1_x = OBJ1.GetX();
    int OBJ1_y = OBJ1.GetY();
    int OBJ1_z = OBJ1.GetZ();

    int OBJ2_x = OBJ2.GetX();
    int OBJ2_y = OBJ2.GetY();
    int OBJ2_z = OBJ2.GetZ();

    // Calcul de la distance euclidienne entre les deux objets
    return sqrt(pow(OBJ2_x - OBJ1_x, 2) + pow(OBJ2_y - OBJ1_y, 2) + pow(OBJ2_z - OBJ1_z, 2));
}