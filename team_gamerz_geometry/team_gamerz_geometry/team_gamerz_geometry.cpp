
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Player.h"
Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);

int main()
{
	
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "ChronoSpacer");
	window.setKeyRepeatEnabled(false);
	Player *player = CreatePlayer(40,40);
	player->speed = 5.0f;
	// Initialise everything below
	// Game loop
	while (window.isOpen()) {

		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		float deltaAngle = deltaTime * 3.14159265358979323846f * 2.0f * player->turnPerSecond;
		//player->sprite.rotate(deltaAngle);
		player->ShipShape.rotate(deltaAngle);
		sf::Event event;
		while (window.pollEvent(event)) {
			PlayerMove(player,event,deltaTime);
		
			// Process any input event here
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();
		//window.draw(player->sprite);
		window.draw(player->ShipShape);
		// Whatever I want to draw goes here
		window.display();
	}

   
}
// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
