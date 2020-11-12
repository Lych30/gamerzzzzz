#pragma once

struct Player {

	float speed;
	float turnPerSecond =0.0f;
	sf::CircleShape ShipShape;
};

struct Fire {
	sf::RectangleShape FireShape;
	float timeToLive;
	float speedFire;
};
