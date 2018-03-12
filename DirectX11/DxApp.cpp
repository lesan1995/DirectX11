#include "DxApp.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define APP_TITLE L"DX APPLICATION"
#define APP_LPSZ_CLASS_NAME L"DXAPPWNDCLASS"

namespace
{
	DxApp* g_pApp = nullptr;
}
LRESULT CALLBACK WndEventHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
LRESULT CALLBACK MainWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
	{
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	}
	else
	{
		return WndEventHandler(hwnd, msg, wParam, lParam);
	}
}



DxApp::DxApp(HINSTANCE hInstance)
{
	//WIN32 ATTRIBUTES
	m_hInstance = hInstance;
	m_hAppWnd = NULL;
	m_clientWidth = SCREEN_WIDTH;
	m_clientHeight = SCREEN_HEIGHT;
	m_appTitle = APP_TITLE;
	m_wndStyle = WS_OVERLAPPEDWINDOW;

	//DIRECTX ATTRIBUTES
	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pBackBufferTexture = 0;

	//m_pPixelBlob = nullptr;
	//m_pPixelBlob = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

}



bool DxApp::Init()
{
	if (!InitWindows())
	{
		return false;
	}
	if (!InitDirect3D())
	{
		return false;
	}
	return true;
}


bool DxApp::InitWindows()
{

	//REGISTER WINDOWS CLASS
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbWndExtra = 0;
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_hInstance;
	wcex.lpfnWndProc = MainWinProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = APP_LPSZ_CLASS_NAME;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString(L"FAILED TO CREATE WINDOWS CLASS\n");
		return false;
	}

	//CREATE WINDOWS 
	RECT r = { 0, 0, m_clientWidth, m_clientHeight };
	AdjustWindowRect(&r, m_wndStyle, FALSE);

	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_hAppWnd = CreateWindow(APP_LPSZ_CLASS_NAME, m_appTitle.c_str(), m_wndStyle, x, y, width, height, NULL, NULL, m_hInstance, NULL);

	if (!m_hAppWnd)
	{
#ifdef _DEBUG //This flag will be automatically enabled when running in debug mode
		OutputDebugString(L"FAILED TO CREATE WINDOWS\n");
		__debugbreak();
#endif //_DEBUG	
		return false;
	}
	ShowWindow(m_hAppWnd, SW_SHOW);
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	return true;
}

bool DxApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_DRIVER_TYPE driveType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverType = ARRAYSIZE(driveType);

	D3D_FEATURE_LEVEL featureLevels[]=
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	UINT numFeatures = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc,sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = m_clientWidth;
	swapDesc.BufferDesc.Height = m_clientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hAppWnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen

	HRESULT result;

	for (UINT i = 0; i < numDriverType; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(
			NULL, 
			driveType[i],
			NULL, 
			createDeviceFlags, 
			featureLevels, 
			numFeatures, 
			D3D11_SDK_VERSION, 
			&swapDesc, 
			&m_pSwapChain, 
			&m_pDevice, 
			&m_FeatureLevel, 
			&m_pImmediateContext);
		if (SUCCEEDED(result))
		{
			m_DriveType = driveType[i];
			break;
		}
	}
	if (FAILED(result))
	{
		OutputDebugString(L"FAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}

	//CREATE RENDER TARGET 
	//HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBufferTexture));
	//HR(m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pRenderTargetView));

	m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pBackBufferTexture);
	m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pRenderTargetView);

	Memory::SafeRelease(m_pBackBufferTexture); 

	//BIND RENDER TARGET VIEW
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	//VIEWPORT CREATION
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.Width = static_cast<float>(m_clientWidth);
	m_ViewPort.Height = static_cast<float>(m_clientHeight);
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	//BIND VIEWPORT
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	if (!InitPipeline())
	{
		return false;
	}
	if (!InitGraphics())
	{
		return false;
	}

	return true;
}

bool DxApp::InitPipeline()
{
	ID3D10Blob*					m_pVertexBlob;
	ID3D10Blob*					m_pPixelBlob;
	HRESULT result1 = D3DX11CompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, 0, &m_pVertexBlob, 0, 0);
	HRESULT result2 = D3DX11CompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, 0, &m_pPixelBlob, 0, 0);

	DXASSERT(m_pVertexBlob || m_pPixelBlob);
	if (FAILED(result1) || FAILED(result2))
	{
		MessageBox(NULL, L"Can't find the shader function", L"InitPipeline Error", MB_ICONSTOP);
		_CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, "Can't find the shader function", NULL);
		return false;
	}

	result1 = m_pDevice->CreateVertexShader(m_pVertexBlob->GetBufferPointer(), m_pVertexBlob->GetBufferSize(), NULL, &m_pVertexShader);
	result2 = m_pDevice->CreatePixelShader(m_pPixelBlob->GetBufferPointer(), m_pPixelBlob->GetBufferSize(), NULL, &m_pPixelShader);

	if (FAILED(result1) || FAILED(result2))
	{
		return false;
	}

	m_pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);

	return true;
}

bool DxApp::InitGraphics()
{
	// Create some vertices to define a triangle
	Vertex	OurVertices[] = 
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.4f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};	
	//create the vertex buffer
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.ByteWidth = sizeof(Vertex)* 3;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_pDevice->CreateBuffer(&m_BufferDesc, NULL, &m_pBuffer);

	//copy the vertices into the buffer
	HRESULT result = m_pImmediateContext->Map(m_pBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_SubResource);
	//HREFTYPE result = ERROR;
	if (FAILED(result))
	{		
		MessageBox(NULL, L"Can't map the vertex buffer", L"InitGraphics Error", MB_ICONSTOP);
		abort();
	}
	
	memcpy(m_SubResource.pData, OurVertices, sizeof(OurVertices));
	m_pImmediateContext->Unmap(m_pBuffer, NULL);

	return true;
}

int DxApp::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(0.0f);
			Render(0.0f);
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT DxApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WndEventHandler(hwnd, msg, wParam, lParam);
}

DxApp::~DxApp()
{
	CleanUp();
}

void DxApp::CleanUp()
{
	if (m_pImmediateContext)
	{
		m_pImmediateContext->ClearState();
	}
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
	Memory::SafeRelease(m_pVertexShader);
	Memory::SafeRelease(m_pPixelShader);

	//Memory::SafeDelete(m_pVertexBlob);
	//Memory::SafeDelete(m_pPixelBlob);
	//m_appTitle = nullptr;
	//m_appTitle.clear();
	//Memory::SafeDelete(m_appTitle);
}