#include "GeometryGenerator.h"
#include "SubMeshGeometry.h"

SubMeshGeometry GeometryGenerator::CreateBox(ID3D11Device * device, float width, float height, float depth)
{
	SubMeshGeometry subMesh;
	

	Vertex vertices[] =
	{
		//								[  vertex position  ]					[texture coord]				   [	  normal	  ]
		/*0*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),

		/*4*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*5*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*6*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*7*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),

		/*8*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*9*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*10*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*11*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),

		/*12*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*13*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*14*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*15*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),

		/*16*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*17*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*18*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*19*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),

		/*20*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*21*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, -height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*22*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*23*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, +height * 0.5f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
	};
	UINT indices[36]
	{
		// bottom
		0, 1, 2,
		2, 3, 0,

		// top
		6, 5, 4,
		4, 7, 6,

		// left
		8, 9, 10,
		10, 11, 8,

		// right
		14, 13, 12,
		12, 15, 14,

		// back 
		16, 17, 18,
		18, 19, 16,

		// front
		22, 21, 20,
		20, 23, 22

	};

	subMesh.VertexCount = ARRAYSIZE(vertices);
	subMesh.IndexCount = ARRAYSIZE(indices);
	subMesh.vertexBuffer.Initialize(device, vertices, ARRAYSIZE(vertices));
	subMesh.indexBuffer.Initialize(device, indices, ARRAYSIZE(indices));

	return subMesh;
}

SubMeshGeometry GeometryGenerator::CreateGrid(ID3D11Device * device, float width, float depth)
{
	SubMeshGeometry subMesh;


	Vertex vertices[] =
	{
		/*0*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, 0.0f, +depth * 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, 0.0f, +depth * 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+width * 0.5f, 0.0f, -depth * 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-width * 0.5f, 0.0f, -depth * 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f)),
		
	};
	UINT indices[36]
	{
		// bottom
		0, 1, 2,
		2, 3, 0

	};

	subMesh.VertexCount = ARRAYSIZE(vertices);
	subMesh.IndexCount = ARRAYSIZE(indices);
	subMesh.vertexBuffer.Initialize(device, vertices, ARRAYSIZE(vertices));
	subMesh.indexBuffer.Initialize(device, indices, ARRAYSIZE(indices));

	return subMesh;
}
