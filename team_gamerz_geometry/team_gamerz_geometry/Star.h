#pragma once
#include <SFML/Graphics.hpp>
std::vector<Star>CreateStar(int Width,int Height,sf::RenderWindow *window,std::vector<Star>stars)
{
	Star star;
	for (int i = 0; i < 100; i++)
	{
		if (star.randomPlacement == 1)
		{
			star.star.setPosition(rand() % (Width + 1), rand() % (Height + 1));
			star.star.setRadius(rand() % 5);
			stars.push_back(Star(star));
		}
	}
	return stars;
}

