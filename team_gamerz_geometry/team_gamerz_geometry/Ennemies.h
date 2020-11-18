#pragma once
#include <SFML/Graphics.hpp>
class Player;
sf::Vector2f SetPos(int screenWidth, int screenHeight, int offsetScreen);

class Ennemy {
public:
	sf::CircleShape shape;
	float speed = 0;
	int type = 0;
	int hp = 0;
	sf::Vector2f playerPos;
	Ennemy(Player* player, sf::RenderWindow* window, int offset);
};

