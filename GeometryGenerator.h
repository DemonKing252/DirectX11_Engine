#pragma once
#include <Windows.h>
#include <d3d11.h>
class SubMeshGeometry;
class GeometryGenerator
{
public:
	SubMeshGeometry CreateBox(ID3D11Device * device, float width, float height, float depth);
	SubMeshGeometry CreateGrid(ID3D11Device * device, float width, float depth);

};

