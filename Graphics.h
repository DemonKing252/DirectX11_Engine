#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "D3DCommon.h"
#include "Vertex.h"
#include "includes/WICTextureLoader.h"
#include <memory>
#include "Window.h"
#include "D3DUtil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
// ImGui for DirectX11
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui.h"

// DirectX Libraries 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

class D3DUploadBuffer;
class PSConstBuffer;
class VSConstBuffer;

typedef class Graphics
{
public:

	// Delete the copy constructor. Only one instance of graphics should be created!
	Graphics(Graphics* copy) = delete;
	Graphics();
	~Graphics();

	void InitPipeline(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitGraphics(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitImGui(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::shared_ptr<Window> window);
	void Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Draw(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void UpdateConstants(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Clean();

	bool VsyncEnabled()const;

private:
	float mCntr = 0.0f;
	bool m_bVsyncEnabled = true;

	// shader info
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_d3dVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_d3dPixelShaderNoIllumination;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_d3dPixelShaderDefault;

	// buffers
	std::shared_ptr<VertexBuffer<Vertex>> m_vertexBuffer;
	std::shared_ptr<IndexBuffer<UINT>> m_indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dVSConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dPSConstantBuffer;

	// input layout and constant buffer instance
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_d3dInputLayout;

	VSConstBuffer* m_VSConstBuffer;
	PSConstBuffer* m_PSConstBuffer;
	
	// texturing
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> fence_shaderResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> redstoneLamp_shaderResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stoneBrick_shaderResource;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_d3dSamplerState;

	// blending
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_d3dBlendState;
	
	DirectX::XMMATRIX m_4x4Model;
	DirectX::XMMATRIX m_4x4View;
	DirectX::XMMATRIX m_4x4Projection;

} Graphics, GFX;