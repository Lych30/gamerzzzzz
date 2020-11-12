#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>;
#include "Player.h"



Player* CreatePlayer(float ShipLength,float shipWidth) {
	Player* player = new Player;
	player->ShipShape.setPointCount(3);
	player->ShipShape.setRadius(ShipLength);
	player->ShipShape.setOrigin(ShipLength, shipWidth);
	player->ShipShape.setFillColor(sf::Color(255, 0, 0));
	player->ShipShape.setPosition(400, 200);
	player->ShipShape.setScale(0.5f,1);
	
	return player;
}

void PlayerMove(Player* player, sf::Event event,float deltatime) {
	
	float angleRADS = (3.1415926536f / 180)* (player->ShipShape.getRotation());
	float X = player->speed * sin(angleRADS);
	float Y = player->speed * -cos(angleRADS);
	/*if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up) {
			//player->sprite.move(X, Y);
			player->ShipShape.move(X, Y);
		}
		if (event.key.code == sf::Keyboard::Left) {
			player->turnPerSecond = -16.0f;
		}
		if (event.key.code == sf::Keyboard::Right) {
			player->turnPerSecond = 16.0f;
		}
	}
	if (event.type == sf::Event::KeyReleased) {
		if (sf::Keyboard::Left) {
			player->turnPerSecond = 0.0f;
		}
		if (sf::Keyboard::Right) {
			player->turnPerSecond = 0.0f;
		}
	}*/
	
}

Fire* CreateFire(float FireLength, float FireWidth) {
	Fire* fire = new Fire;
	fire->FireShape.setSize(sf::Vector2f(4, 4));
	fire->FireShape.setFillColor(sf::Color(234, 145, 18));
	fire->FireShape.setOutlineColor(sf::Color(245,67,5));
	fire->FireShape.setOutlineThickness(1.0f);
	fire->FireShape.setOrigin(FireLength, FireWidth);
	fire->speedFire = 10.0f;
	fire->timeToLive = 3.0f;

	return fire;
}
