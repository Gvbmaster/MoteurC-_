#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::addComponent(Component* component)
{
	m_Components.push_back(component);
}

void GameObject::getComponent(Component* component)
{
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i] == component)
		{
			//return m_Components[i];
		}
	}
}