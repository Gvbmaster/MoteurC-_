#pragma once
#include "../Common/d3dUtil.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void addComponent(Component* component);
	void getComponent(Component* component);

protected :
	std::vector<Component*> m_Components;
};