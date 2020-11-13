#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>;
#include "Ennemi_1.h"
#include <vector>
//fonction random spawnpoint


int SpawnPoint(int screenWidth, int screenHeight) {
	int randomPosX = 0;
	int randomPosY = 0;
	int randomSideSpawn = 0;
	randomSideSpawn = rand() % 3; //nombre de cotés moins 1
	

	switch (randomSideSpawn)
	{
	case 0: // spawn coté gauche
		randomPosX = -30;
		randomPosY = rand() %  (screenHeight+30) - 30 ;
		break;
	case 1: // spawn coté haut
		randomPosX = rand() % (screenWidth+30) - 30;
		randomPosY = -30;
		break;
	case 2: // spawn coté droit
		randomPosX = screenWidth + 30;
		randomPosY = rand() % (screenHeight + 30) - 30;
		break;
	default: // spawn coté bas
		randomPosX = rand() % (screenWidth + 30) - 30;
		randomPosY = (screenHeight)+30;
		break;
	}


	return randomPosX, randomPosY;
 }

//fonction summon ennemy1
EnnemyTypeOne* SummonE1() {

	EnnemyTypeOne*  ennemy = new EnnemyTypeOne;
	ennemy->ennemyShape.setPosition(1,1);
	ennemy->ennemyShape.setScale(20,20);
	ennemy->ennemyShape.setFillColor(sf::Color(255,0,0));
	return ennemy;
}

//fonction summon ennemi2 qui summon des ennemy1