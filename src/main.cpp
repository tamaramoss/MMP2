#include "stdafx.h"

#include "Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // checking for leaks

	Game game;
	game.run();
	return 0;
}
