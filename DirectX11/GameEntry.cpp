//START HERE
#include "TestGame.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	//Get the instance of class Game
	Game* testGame = Game::GetInstance(hInstance);

	//Init and run game
	if (testGame->Init())
	{
		testGame->Run();		
	}	
	
	//Release all resources
	testGame->DestroyInstance();
}
 