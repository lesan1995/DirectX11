#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11_1.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <string>
#include "DXUtils.h"
#include <DirectXColors.h>
//#include <vld.h>
//#include "DXERR\dxerr.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class DxApp
{
public:	
	
	int Run();
	virtual bool Init();
	//virtual void Run();
	virtual void Update(float deltaTime) = 0;
	virtual void Render(float deltaTime) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// SINGLETON PATTERN
	DxApp(HINSTANCE hInstance);
	virtual ~DxApp();
	//static DxApp*	instance;
	// WIN32 ATTRIBUTES
	HWND			m_hAppWnd;
	HINSTANCE		m_hInstance;
	UINT			m_clientWidth;
	UINT			m_clientHeight;
	std::wstring	m_appTitle;
	DWORD			m_wndStyle;

	// DIRECTX ATTRIBUTES
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pImmediateContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	D3D_DRIVER_TYPE				m_DriveType;
	D3D_FEATURE_LEVEL			m_FeatureLevel;
	D3D11_VIEWPORT				m_ViewPort;
	ID3D11Texture2D*			m_pBackBufferTexture;


	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;

	ID3D11Buffer*				m_pBuffer;
	D3D11_BUFFER_DESC			m_BufferDesc;
	D3D11_MAPPED_SUBRESOURCE	m_SubResource;

	//FOR DRAWING A TRIANGLE
	struct Vertex {
		FLOAT				m_x, m_y, m_z;
		D3DXCOLOR			m_color;
	};

protected:
	// INIT WIN32
	bool		InitWindows();

	//INIT DIRECT3D
	bool		InitDirect3D();
	bool		InitPipeline();
	bool		InitGraphics();

	//CLEANUP 
	void		CleanUp();

};

