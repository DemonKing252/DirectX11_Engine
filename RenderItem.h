#pragma once
#include "EntityComponentSystem.h"
// Why does this look so empty? 
// Wait and find out.
enum ShaderResource { Fence, RedstoneLamp, StoneBrick, None };
enum PixelShader { Default, NoIllumination, Undefined};
class RenderItem : public Entity
{
public:
	RenderItem();
	~RenderItem();

	ShaderResource shaderResource;
	PixelShader pixelShader;

	bool m_bDoesRotate;
	bool m_bIsLight;
};

