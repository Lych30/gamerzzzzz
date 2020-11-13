#pragma once

struct Player {

	float speed;
	float turnPerSecond =0.0f;
	sf::CircleShape ShipShape;
	float reload = 0.25f;
};
class Bullet {
public:
	sf::CircleShape bullet;
	sf::Vector2f currentVelocity;
	float maxSpeed;
	
		Bullet(float radius = 5.f) : currentVelocity(0.f,0.f),maxSpeed(0.75f)
	{
			this->bullet.setRadius(radius);
			this->bullet.setFillColor(sf::Color::Cyan);
			this->bullet.setOutlineColor(sf::Color::Blue);
			this->bullet.setOutlineThickness(2.f);

	}
};
