#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include "Player.h"
#include "MathUtils.h"
#include <vector>
#include "Star.h"
#include "Ennemies.h"
#include "Menu.h"


std::string getAppPath();
std::string getAssetsPath();
std::string getSoundsPath();

Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);

int main()
{
	
	std::vector<Star> stars;

	std::cout << getAssetsPath() << std::endl;
	std::cout << getSoundsPath() << std::endl;
	sf::Font arcade;
	sf::Text textScore;
	sf::Text score;

	bool gameHasStarted = false;

	//Set de la musique de fond.
	sf::Music music;
	if (!music.openFromFile(getSoundsPath() + "\\backgroundMusic.ogg")) {
		return -1; // erreur
	}
	music.setVolume(5);
	music.setLoop(true);
	music.play();

	//Set du son du tir1
	sf::Sound tir1;
	sf::SoundBuffer bufferTir1;
	tir1.setBuffer(bufferTir1);
	if (!bufferTir1.loadFromFile(getSoundsPath() + "\\tir1.ogg")) {
		return -1;
	}
	tir1.setVolume(40);

	//Set du son du tir2 (tir spéciale)
	sf::Sound tir2;
	sf::SoundBuffer bufferTir2;
	tir2.setBuffer(bufferTir2);
	if (!bufferTir2.loadFromFile(getSoundsPath() + "\\tir2.ogg")) {
		return -1;
	}
	tir2.setVolume(40);

	int comptScore = 50000;

	//Setup de la Font
	arcade.loadFromFile(getAssetsPath() + "\\arcade1.ttf");

	//écriture de "score:"
	textScore.setFont(arcade);
	textScore.setString("score:");
	textScore.setFillColor(sf::Color(255, 143, 0));
	textScore.setPosition(5, 5);

	//écriture de la variable score en string
	score.setFont(arcade);
	score.setFillColor(sf::Color(239, 159, 58));
	score.setString(std::to_string(comptScore));
	score.setPosition(175, 5);

	bool IsLoaded = false;
	static int screenWidth, screenHeight;
	screenHeight = 720;
	screenWidth = 1280;
	sf::Clock clock;
	sf::RenderWindow *window =  new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "GeometryWar");
	//window.setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);
	Player *player = CreatePlayer(40,40);
	player->speed = 100.0f;

	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	Bullet b1;
	std::vector<Bullet> bullets;

	std::vector<Ennemy> ennemy;
	float spawnDelay = 1;

	std::vector<Button>buttons;
	// Initialise everything below
	// Game loop

	stars = CreateStar(screenWidth, screenHeight,window,stars);
	buttons = InitialiseButton(window, 0, 0, buttons, "PLAY");
	buttons = InitialiseButton(window, 0, 300, buttons, "QUITTER");
	while (window->isOpen()) {

		// HOW TO HANDLE MOUSE POSITION
		sf::Vector2i mousePositionInt = sf::Mouse::getPosition(*window);
		//std::cout << mousePositionInt.x << ", " << mousePositionInt.y << std::endl;
		sf::Vector2f mousePosition(mousePositionInt);



		// HOW TO ORIENT IN A SPECIFIC DIRECTION
		sf::Vector2f shipToAim = mousePosition - player->ShipShape.getPosition();
		float aimingAngle = atan2f(shipToAim.y, shipToAim.x);

		if (player->IsDashing== false)
		{
			player->ShipShape.setRotation(ConvertRadToDeg(aimingAngle + IIM_PI / 2.0f));
		}
		

		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		float deltaAngle = deltaTime * 3.14159265358979323846f * 2.0f * player->turnPerSecond;
		//player->sprite.rotate(deltaAngle);
		if (player->IsDashing == false) {
			player->ShipShape.rotate(deltaAngle);
		}
		
		sf::Event event;
		while (window->pollEvent(event)) {

			//PLAYER
			PlayerMove(player, event, deltaTime);


				aimDir = mousePosition - player->ShipShape.getPosition();
				aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
			


			// Process any input event here
			if (event.type == sf::Event::Closed) {
				window->close();
			}
		}
		// Direction
		sf::Vector2f dir;
		if (player->IsDashing == false)
		{

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
		}

		//TIR
		//1
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && player->reload <=0) {
			b1.bullet.setPosition(player->ShipShape.getPosition());
			b1.currentVelocity = aimDirNorm * b1.maxSpeed;
			bullets.push_back(Bullet(b1));
			player->reload = 0.5f;
			std::cout << aimDirNorm.x << " ; " << aimDirNorm.y << std::endl;
			tir1.play();
		}
		else
		{
			player->reload -= deltaTime;
		}
		//2
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && player->reload <= 0) {

		

			aimDir = (mousePosition - player->ShipShape.getPosition());
			aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) + sf::Vector2f(0.2f,0.2f);
			b1.bullet.setPosition(player->ShipShape.getPosition());
			b1.currentVelocity = aimDirNorm * (b1.maxSpeed +.5f);
			bullets.push_back(Bullet(b1));

			aimDir = (mousePosition - player->ShipShape.getPosition());
			aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) - sf::Vector2f(0.2f, 0.2f);
			b1.bullet.setPosition(player->ShipShape.getPosition());
			b1.currentVelocity = aimDirNorm *(b1.maxSpeed +.5f);
			bullets.push_back(Bullet(b1));

			aimDir = mousePosition - player->ShipShape.getPosition();
			aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
			b1.bullet.setPosition(player->ShipShape.getPosition());
			b1.currentVelocity = aimDirNorm * (b1.maxSpeed +.5f);
			bullets.push_back(Bullet(b1));
			tir2.play();

			player->reload = 0.5f;
		}
		else
		{
			player->reload -= deltaTime;
		}


		//Dash
		if (player->DashReloadTime > 0)
		{
			player->DashReloadTime -= deltaTime;
		}
		else
		{
			player->DashReady = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && player->DashReady == true) {
			player->IsDashing = true;
		}
		 if(player->IsDashing == true)
		{
			if(player->DashTime > 0)
			{
				player->ShipShape.move(aimDirNorm*3.f);
				player->DashTime -= deltaTime;
			}
			else
			{
				player->DashTime = 0.05f;
				player->DashReloadTime = 2.0f;
				player->IsDashing = false;
				player->DashReady = false;

			}
			
		}
		 //Fin du DASH



		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].bullet.move(bullets[i].currentVelocity);
			if (bullets[i].bullet.getPosition().x<0 || bullets[i].bullet.getPosition().x > window->getSize().x
				|| bullets[i].bullet.getPosition().y<0 || bullets[i].bullet.getPosition().y > window->getSize().y)
			{
				bullets.erase(bullets.begin() + i);
			}
		}
		if (player->IsDashing == false)
		{
			player->ShipShape.move(dir * 100.0f * deltaTime);
		}
		


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

		//SPAWN ENNEMIES

		if (spawnDelay <= 0 && gameHasStarted) {
			ennemy.push_back({ player, window ,30});
			spawnDelay = 1.0f;
		}
		else
		{
			spawnDelay -= deltaTime;
		}
		

		
		window->clear();


		for (size_t i = 0; i < ennemy.size(); i++)
		{
			window->draw(ennemy[i].shape);
			for (size_t j = 0; j < bullets.size(); j++)
			{
				
				if (ennemy[i].shape.getGlobalBounds().contains(bullets[j].bullet.getPosition()))
				{
						ennemy.erase(ennemy.begin()+i);
				}
			}
		}

		for (size_t i = 0; i < bullets.size(); i++)
		{
			window->draw(bullets[i].bullet);
		}
		for (size_t i = 0; i < stars.size(); i++)
		{
			window->draw(stars[i].star);
		}

		for (size_t i = 0; i < buttons.size(); i++)
		{
			window->draw(buttons[i].ButtonShape);
			buttons[i].text.setFont(arcade);
			buttons[i].text.setFillColor(sf::Color(255, 143, 0));
			if (buttons[i].ButtonShape.getGlobalBounds().contains(mousePosition)) {
				buttons[i].ButtonShape.setFillColor(sf::Color(0, 255, 0, 0));
				buttons[i].text.setFillColor(sf::Color(255, 255, 255, 255));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && buttons[i].text.getString() == "QUITTER")
				{
					window->close();
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && buttons[i].text.getString() == "PLAY")
				{
					gameHasStarted = true;
					while (buttons.size() > 0)
					{
						buttons.erase(buttons.begin());
					}
				}
			}
			else
			{
				buttons[i].ButtonShape.setFillColor(sf::Color(255, 255, 255, 0));
			}
			if (buttons.size() > 0)
			{
				window->draw(buttons[i].text);
			}

		}

		window->draw(player->ShipShape);
		window->draw(textScore);
		window->draw(score);
		window->display();
	}
}

std::string getAppPath() {
	char cExeFilePath[256];
	GetModuleFileNameA(NULL, cExeFilePath, 256);
	std::string exeFilePath = cExeFilePath;
	int exeNamePos = exeFilePath.find_last_of("\\/");
	std::string appPath = exeFilePath.substr(0, exeNamePos + 1);
	return appPath;
}

std::string getAssetsPath() {
	std::string assetsPath = getAppPath() + "\Assets";
	return assetsPath;
}

std::string getSoundsPath() {
	std::string soundsPath = getAppPath() + "\Sounds";
	return soundsPath;
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
