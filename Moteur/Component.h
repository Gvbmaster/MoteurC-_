#pragma once

class GameObject;
struct Transform;

enum class ComponentType {
    Camera,
};

class Component {
public:
    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void init();
    virtual void update(float deltaTime);
    // virtual void draw(Renderer& renderer);
    virtual ComponentType getType() const = 0;
    virtual Transform* getTransform() = 0;

protected:
    GameObject* m_gameObject;
};
