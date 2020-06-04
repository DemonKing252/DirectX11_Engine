#pragma once
#include "EntityComponentSystem.h"
#include <Windows.h>
class SubMeshGeometry : public Entity
{
public:
	SubMeshGeometry();
	~SubMeshGeometry();

	UINT IndexCount;
	UINT VertexCount;
};

