#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
	int windowWidth = 512, windowHeight = 512;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "First SFML Project", sf::Style::Default);

	while (window.isOpen())
	{
		sf::Event someEvent;

		while (window.pollEvent(someEvent))
		{
			switch (someEvent.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
	}

	return 0;
}
