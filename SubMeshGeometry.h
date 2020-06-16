#pragma once
#include "EntityComponentSystem.h"
#include <Windows.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
class SubMeshGeometry : public Component
{
public:
	SubMeshGeometry();
	~SubMeshGeometry();

	UINT IndexStart;
	UINT IndexCount;
	UINT VertexCount;

	VertexBufferComponent<Vertex> vertexBuffer;
	IndexBufferComponent<UINT> indexBuffer;

	// This cannot be changed to a different list like a triangle fan or a quad for now.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
};

