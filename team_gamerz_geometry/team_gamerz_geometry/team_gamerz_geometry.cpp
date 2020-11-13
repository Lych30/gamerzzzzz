
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Player.h"
#include "MathUtils.h"
#include <vector>
Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);

int main()
{
	bool IsLoaded = false;
	int screenWidth, screenHeight;
	screenHeight = 720;
	screenWidth = 1280;
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "GeometryWar");
	//window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	Player *player = CreatePlayer(40,40);
	player->speed = 100.0f;

	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	Bullet b1;
	std::vector<Bullet> bullets;

	Star star;
	std::vector<Star> stars;
	// Initialise everything below
	// Game loop
	while (window.isOpen()) {
	
		for (int i = 0; i < 100 && IsLoaded == false; i++)
		{
			if (star.randomPlacement == 1)
			{
				star.star.setPosition(rand() % (screenWidth + 1), rand() % (screenHeight + 1));
				star.star.setRadius(rand() % 5);
				stars.push_back(Star(star));
			}
			if (i == 99)
			{
				IsLoaded = true;
			}
			
		}

		// HOW TO HANDLE MOUSE POSITION
		sf::Vector2i mousePositionInt = sf::Mouse::getPosition(window);
		//std::cout << mousePositionInt.x << ", " << mousePositionInt.y << std::endl;
		sf::Vector2f mousePosition(mousePositionInt);



		// HOW TO ORIENT IN A SPECIFIC DIRECTION
		sf::Vector2f shipToAim = mousePosition - player->ShipShape.getPosition();
		float aimingAngle = atan2f(shipToAim.y, shipToAim.x);
		player->ShipShape.setRotation(ConvertRadToDeg(aimingAngle + IIM_PI / 2.0f));



		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		float deltaAngle = deltaTime * 3.14159265358979323846f * 2.0f * player->turnPerSecond;
		//player->sprite.rotate(deltaAngle);
		player->ShipShape.rotate(deltaAngle);
		sf::Event event;
		while (window.pollEvent(event)) {

			//PLAYER
			PlayerMove(player,event,deltaTime);


			aimDir = mousePosition - player->ShipShape.getPosition();
			aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
			// Process any input event here
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::Vector2f dir;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			dir.x -= 5.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			dir.x += 5.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			dir.y -= 5.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			dir.y += 5.0f;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && player->reload <=0) {
			b1.bullet.setPosition(player->ShipShape.getPosition());
			b1.currentVelocity = aimDirNorm * b1.maxSpeed;
			bullets.push_back(Bullet(b1));
			player->reload = 0.5f;
		}
		else
		{
			player->reload -= deltaTime;
		}
		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].bullet.move(bullets[i].currentVelocity);
			if (bullets[i].bullet.getPosition().x<0 || bullets[i].bullet.getPosition().x > window.getSize().x
				|| bullets[i].bullet.getPosition().y<0 || bullets[i].bullet.getPosition().y > window.getSize().y)
			{
				bullets.erase(bullets.begin() + i);
			}
		}





		player->ShipShape.move(dir * 100.0f * deltaTime);


		//TP
		if (player->ShipShape.getPosition().x < -30) {
			player->ShipShape.setPosition(screenWidth + 30, player->ShipShape.getPosition().y);
		}
		if (player->ShipShape.getPosition().x > screenWidth + 30) {
			player->ShipShape.setPosition(-29, player->ShipShape.getPosition().y);
		}
		if (player->ShipShape.getPosition().y < -30) {
			player->ShipShape.setPosition(player->ShipShape.getPosition().x, screenHeight + 30);
		}
		if (player->ShipShape.getPosition().y > screenHeight + 30) {
			player->ShipShape.setPosition(player->ShipShape.getPosition().x, -29);
		}
		
		window.clear();
		

		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].bullet);
		}
		for (size_t i = 0; i < stars.size(); i++)
		{
			window.draw(stars[i].star);
		}
		window.draw(player->ShipShape);
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
