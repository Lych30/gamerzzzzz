#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>;
#include "SetEnnemiesPosition.h"
sf::Vector2f SetPos(int screenWidth, int screenHeight, int offsetScreen) {
	float randomPosX, randomPosY;
	int randomScreenSide;
	randomScreenSide = rand() % 3;
	switch (randomScreenSide) {
	case 0:
		randomPosX = -offsetScreen;
		randomPosY = rand() % (screenHeight + offsetScreen) - offsetScreen;
		break;
	case 1:
		randomPosX = rand() % (screenWidth + offsetScreen) - offsetScreen;
		randomPosY = -offsetScreen;
		break;
	case 2:
		randomPosX = offsetScreen;
		randomPosY = rand() % (screenHeight + offsetScreen) - offsetScreen;
		break;
	default:
		randomPosX = rand() % (screenWidth + offsetScreen) - offsetScreen;
		randomPosY = offsetScreen;
		break;
	}
	
	return sf::Vector2f(randomPosX, randomPosY);
}