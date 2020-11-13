#pragma once
class EnnemyTypeOne {
public:
	float speed;
	sf::CircleShape ennemyShape;
	float spawnReload = 0.5f;

	EnnemyTypeOne(float radius = 20.f)
	{
		this->ennemyShape.setPointCount(4);
		this->ennemyShape.setFillColor(sf::Color(255, 0, 0));
		this->ennemyShape.setRadius(radius);
	}
};
