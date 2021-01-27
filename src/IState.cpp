// FinalFrontier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Game
{
public:

	void Run()
	{
		if (!init())
			return;

		while (m_window.isOpen())
		{
			Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					m_window.close();
			}
		}

		shutdown();
	}

private:
	bool init()
	{
		m_window.create(sf::VideoMode(800, 600), "FinalFrontier");
		return true;
	}

	void shutdown()
	{

	}

	RenderWindow m_window;
};

int main()
{
	Game game;
	game.Run();
    return 0;
}

