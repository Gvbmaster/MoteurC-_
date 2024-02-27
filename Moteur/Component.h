#pragma once
#include "../Common/d3dUtil.h"
class Component
{
public:
	Component();
	virtual ~Component();
	virtual void init();
	virtual void update();
	virtual void draw();

protected:



};

