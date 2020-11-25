#pragma once
#include <SFML/Graphics.hpp>
class Player;

sf::Vector2f SetPos(int &screenWidth, int &screenHeight, int &offsetScreen);

class Ennemy {
public:
	
	sf::CircleShape shape;
	float speed = 80.0f;
	bool type = false; //0 -> child ship ---- 1 -> mother ship
	int hp = 0;
	sf::Vector2f EnnemyDir;
	sf::Vector2f EnnemyDirNorm;
	sf::Vector2f playerPos;
	Ennemy(Player* player, sf::RenderWindow* window, int offset, int actualScore);

};

