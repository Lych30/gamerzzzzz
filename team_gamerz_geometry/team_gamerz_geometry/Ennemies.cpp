#include "Ennemies.h"
#include "Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>;

sf::Vector2f SetPos(int screenWidth, int screenHeight, int offsetScreen) {
	float PosX, PosY;
	int randomScreenSide;
	randomScreenSide = rand() % 4;
	switch (randomScreenSide) {
	case 0: //gauche
		PosX = -offsetScreen;
		PosY = rand() % (screenHeight);
		break;
	case 1://haut
		PosX = rand() % (screenWidth);
		PosY = -offsetScreen;
		break;
	case 2://droit
		PosX = screenWidth + offsetScreen;
		PosY = rand() % (screenHeight);
		break;
	case 3://bas
		PosX = rand() % (screenWidth);
		PosY = screenHeight + offsetScreen;
		break;
	}
	std::cout << "side " << randomScreenSide << " posX: " << PosX << " posY: " << PosY << std::endl << std::endl;
	return sf::Vector2f(PosX, PosY);
}


Ennemy::Ennemy(Player* player, sf::RenderWindow* window, int offset, int actualScore) {
	
	int randomNumber;
	randomNumber = rand() % 100;
	
	if (randomNumber < 20) {
		this->type = true;
		this->hp = 2;
	}
	else {
		this->type = false;
		this->hp = 1;
	}
	
	/*switch (actualScore) {
	case 10:
		this->speed = 2.4f;
		if (this->type) {
			this->hp = 4;
		}
		else
		{
			this->hp = 2;
		}
		
		break;
	case 20:
		this->speed = 2.2f;
		if (this->type) {
			this->hp = 4;
		}
		else
		{
			this->hp = 2;
		}
		break;
	case 30:
		this->speed = 2.0f;
		if (this->type) {
			this->hp = 5;
		}
		else
		{
			this->hp = 3;
		}
		break;
	case 40:
		this->speed = 1.8f;
		if (this->type) {
			this->hp = 6;
		}
		else
		{
			this->hp = 4;
		}
		break;
	case 50:
		this->speed = 1.6f;
		if (this->type) {
			this->hp = 7;
		}
		else
		{
			this->hp = 4;
		}
		break;
	case 60:
		this->speed = 1.5f;
		if (this->type) {
			this->hp = 8;
		}
		else
		{
			this->hp = 5;
		}
		break;
	case 70:
		this->speed = 1.4f;
		if (this->type) {
			this->hp = 9;
		}
		else
		{
			this->hp = 5;
		}
		break;

	}*/

	/*if (actualScore % 10 == 0) {
		
		this->speed -= 0.1f;
		if (this->type) {
			this->hp += 2;
		}
		else {
			this->hp += 1;
		}
		std::cout << this->speed << std::endl;
	}*/
	
	if (actualScore >= 0 && actualScore <= 9) {
		this->speed = 60.0f;
		if (this->type) {
			this->hp = 4;
		}
		else
		{
			this->hp = 2;
		}
	}
	if (actualScore >= 10 && actualScore <= 19) {
		this->speed = 100.0f;
		if (this->type) {
			this->hp = 5;
		}
		else
		{
			this->hp = 3;
		}
	}
	if (actualScore >= 20 && actualScore <= 29) {
		this->speed =140.0f;
		if (this->type) {
			this->hp = 6;
		}
		else
		{
			this->hp = 4;
		}
	}
	//std::cout << "speed: " << this->speed << std::endl;
	//std::cout << "hp: " << this->hp << std::endl;
	
	this->shape.setPosition(SetPos(window->getSize().x, window->getSize().y, offset));
	std::cout << window->getSize().x << window->getSize().y << std::endl;
	//this->shape.setPosition(player->ShipShape.getPosition());
	playerPos = player->ShipShape.getPosition();
	EnnemyDir = playerPos - shape.getPosition();
	EnnemyDirNorm = EnnemyDir / sqrt(pow(EnnemyDir.x, 2) + pow(EnnemyDir.y, 2));
	this->shape.setRadius(10.0f);
	this->shape.setOutlineColor(sf::Color(255, 255, 255));
	this->shape.setOutlineThickness(1.0f);

}