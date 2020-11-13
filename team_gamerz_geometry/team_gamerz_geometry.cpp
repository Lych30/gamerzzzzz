
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Player.h"
#include "MathUtils.h"
#include "Ennemi_1.h"
#include <vector>
Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);

int main()
{
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

	EnnemyTypeOne nmy;
	std::vector<EnnemyTypeOne> ennemies;
	// Initialise everything below
	// Game loop
	while (window.isOpen()) {

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
		//DIRECTION
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
		//SHOOT BULLETS
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


		//MA MERDE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && nmy.spawnReload <= 0) {
			std::cout << "A key pressed" << std::endl;
			nmy.ennemyShape.setPosition(200, 200);
			ennemies.push_back(EnnemyTypeOne(nmy));
			nmy.spawnReload = 0.5f;
		}
		else {
			nmy.spawnReload -= deltaTime;
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
		window.draw(player->ShipShape);

		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].bullet);
		}

		for (size_t i = 0; i < ennemies.size(); i++) {
			window.draw(ennemies[i].ennemyShape);
		}

		window.display();
	}

   
}
