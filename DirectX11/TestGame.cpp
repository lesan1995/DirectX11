#include "TestGame.h"

Game* Game::instance = 0;

Game::Game(HINSTANCE hInstance) : DxApp(hInstance)
{
}


Game::~Game()
{
	//delete instance;
	//DestroyInstance();
}

void Game::DestroyInstance()
{
	delete instance;
}

Game* Game::GetInstance(HINSTANCE hInstance)
{
	if (instance)
	{
		return instance;
	}
	else
	{
		instance = new Game(hInstance);
		return instance;
	}
}

bool Game::Init()
{
	bool isSuccess = DxApp::Init();

	//CREATE SPRITEBATCH OBJECT
	//m_pSpriteBatch.reset(new DirectX::SpriteBatch(m_pImmediateContext));

	//CREATE TEXTURE FOR RENDERING
	//HRESULT LoadTextureResult = Dir(m_pDevice, L"bamboo3.png", nullptr, &m_pTexture);

	return isSuccess;
}

void Game::Update(float deltaTime)
{

}

void Game::Render(float deltaTime)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);

	m_pSwapChain->Present(0, 0);
}
