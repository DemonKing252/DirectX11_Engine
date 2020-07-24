#pragma once
#include "EntityComponentSystem.h"
#include <Windows.h>
// Why does this look so empty? 
// Wait and find out.
class RenderItem : public Entity
{
public:
	RenderItem();
	~RenderItem();

	// If applicable!
	bool m_bDoesRotate;

	// If applicable!
	UINT m_iLightIndex = 0;
};

