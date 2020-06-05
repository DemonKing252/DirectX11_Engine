#pragma once
#include "EntityComponentSystem.h"
// Why does this look so empty? 
// Wait and find out.
enum PixelShader { Default, NoIllumination, Undefined};
class RenderItem : public Entity
{
public:
	RenderItem();
	~RenderItem();

	PixelShader pixelShader;

	bool m_bDoesRotate;
};

