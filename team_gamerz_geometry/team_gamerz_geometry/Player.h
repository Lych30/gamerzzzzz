#pragma once
#include <SFML/Graphics.hpp>
struct Player {

	float speed;
	float turnPerSecond =0.0f;
	sf::CircleShape ShipShape;
	float reload = 0.25f;
	bool IsDashing = false;
	float DashTime = 0.05f;
	float DashReloadTime = 2.f;
	bool DashReady = false;
	int hp = 3;

};
class Bullet {
public:
	sf::CircleShape bullet;
	sf::Vector2f currentVelocity;
	float maxSpeed;
	
		Bullet(float radius = 5.f) : currentVelocity(0.f,0.f),maxSpeed(1.25f)
	{
			this->bullet.setRadius(radius);
			this->bullet.setFillColor(sf::Color::Cyan);
			this->bullet.setOutlineColor(sf::Color::Blue);
			this->bullet.setOutlineThickness(2.f);

	}
};
class Star {
public :
	sf::CircleShape star;
	int randomPlacement = rand() % 10;
	Star() {
		this->star.setPointCount(4);
	}
};
