#pragma once

//#include "GameObject.h"

class GameObject;
struct Transform;

enum class ComponentType {
	Camera,
    Renderer,
    Light,
    Script,
};

class Component {
public:
    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void init();
    virtual void update(float deltaTime);
    virtual ComponentType getType() const = 0;
    ComponentType m_type;
protected:
    GameObject* m_gameObject;
    Transform* getTransform();
};
