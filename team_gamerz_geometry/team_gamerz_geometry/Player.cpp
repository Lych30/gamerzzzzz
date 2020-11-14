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
	
}

