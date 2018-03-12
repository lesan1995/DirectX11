#pragma once

#include "DxApp.h"
//#include "SpriteBatch.h"
//#include "DDSTextureLoader.h"
//#include "SimpleMath.h"

class Game: public DxApp
{

public:
	// SINGLETON PATTERN
	static Game* GetInstance(HINSTANCE hInstance);
	void DestroyInstance();

	bool Init() override;
	//void Run();
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;

private:
	Game(HINSTANCE hInstance);
	static Game* instance;
	virtual ~Game();
	//std::shared_ptr<DirectX::SpriteBatch>	m_pSpriteBatch;
	ID3D11ShaderResourceView*				m_pTexture;
};