
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Player.h"
#include "MathUtils.h"
Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);
Fire* CreateFire(float FireLength, float FireWidth);

int main()
{
	
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Geometry Wars");
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);
	Player *player = CreatePlayer(40,40);
	Fire* fire = CreateFire(4,4);
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
		sf::Vector2f dir;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			dir.x -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			dir.x += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			dir.y -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			dir.y += 1.0f;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			CreateFire(4, 4);
			fire->FireShape.setPosition(player->ShipShape.getPosition());		
		}
		player->ShipShape.move(dir * 100.0f * deltaTime);
		// Same for mouse with : sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		// HOW TO HANDLE MOUSE POSITION
		sf::Vector2i mousePositionInt = sf::Mouse::getPosition(window);
		//std::cout << mousePositionInt.x << ", " << mousePositionInt.y << std::endl;
		sf::Vector2f mousePosition(mousePositionInt);

		// HOW TO ORIENT IN A SPECIFIC DIRECTION
		sf::Vector2f shipToAim = mousePosition - player->ShipShape.getPosition();
		float aimingAngle = atan2f(shipToAim.y, shipToAim.x);
		player->ShipShape.setRotation(ConvertRadToDeg(aimingAngle + IIM_PI / 2.0f));

		window.clear();
		//window.draw(player->sprite);
		window.draw(player->ShipShape);
		window.draw(fire->FireShape);
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
