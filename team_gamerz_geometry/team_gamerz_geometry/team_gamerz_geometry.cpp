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


class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	ParticleSystem(unsigned int count) :
		m_particles(count),
		m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(0.1f)),
		m_emitter(0.f, 0.f)
	{
	}

	void setEmitter(sf::Vector2f position)
	{
		m_emitter = position;
	}

	void update(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if (p.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			m_vertices[i].position += p.velocity * elapsed.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// our particles don't use a texture
		states.texture = NULL;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

public:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
	};
	void resetParticle(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (std::rand() % 360) * 3.14f / 180.f;
		float speed = (std::rand() % 50) + 50.f;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		m_vertices[index].position = m_emitter;
	}

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
};









std::string getAppPath();
std::string getAssetsPath();
std::string getSoundsPath();

Player* CreatePlayer(float ShipLength, float shipWidth);
void PlayerMove(Player* player, sf::Event event, float deltatime);

void test(int &i) {
	i++; //test reference, ne pas toucher
}

int main()
{
	//test(5); //test reference, ne pas toucher
	int a = 8;
	test(a);


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

	int comptScore = 0;

	//Setup de la Font
	arcade.loadFromFile(getAssetsPath() + "\\arcade1.ttf");

	//écriture de "score:"
	textScore.setFont(arcade);
	textScore.setString("score:");
	textScore.setOutlineColor(sf::Color(213, 19, 19));
	textScore.setOutlineThickness(1.0f);
	textScore.setFillColor(sf::Color(255, 143, 0));
	textScore.setPosition(5, 5);

	//écriture de la variable score en string
	score.setFont(arcade);
	score.setFillColor(sf::Color(239, 159, 58));
	score.setString(std::to_string(comptScore));
	score.setPosition(175, 5);

	bool IsLoaded = false;
	bool isPlaying = false;
	static int screenWidth, screenHeight;
	screenHeight = 720;
	screenWidth = 1280;
	sf::Clock clock;
	sf::RenderWindow *window =  new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "GeometryWar");
	ParticleSystem particles(250);
	sf::Clock ParticuleClock;
	ParticleSystem shot(50);
	sf::Clock shotClock;
	//window->setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);
	sf::Texture curseur;
	curseur.loadFromFile(getAssetsPath() + "\\Cursor.png");
	sf::Sprite sprite(curseur);
	sprite.setOrigin(37, 37);
	Player *player = CreatePlayer(40,40);
	player->speed = 100.0f;

	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	Bullet b1;
	std::vector<Bullet> bullets;

	int ennemyHitDelay = 0.2f;

	std::vector<Ennemy> ennemy; //liste des ennemies
	sf::Vector2f motherShipPosition;
	sf::Vector2f motherShipPositionOffset;
	float spawnDelay = 0.5f;// delais de spawn par défaut.
	float actualSpawnDelay = 0.5f; // var qui se décrémente de dT et revient à "spawnDelay" pour reset
	float chanceToSpawnMotherShip = 20.0f; //pourcentage de chance qu'un vaisseau mere apparaisse (var a augmenter au fur et à mesur que le temps avance...)
	float randomPercent = 0; // valeur random entre 0-100. var comparée à "randomPercent"
	int nbLittleShips = 0; //nombre de vaissaux enfants apres le random.
	int nbLittleShipsRandMin = 2; //random Min
	int nbLittleShipsRandMax = 4; //random Max
	int randomLittleShipOffsetMax = 80;
	int randomLittleShipOffsetMin = randomLittleShipOffsetMax / 2;


	std::vector<Button>buttons;
	// Initialise everything below
	// Game loop

	stars = CreateStar(screenWidth, screenHeight,window,stars);
	buttons = InitialiseButton(window, 0, 0, buttons, "PLAY");
	buttons = InitialiseButton(window, 0, 300, buttons, "QUITTER");
	while (window->isOpen()) {

		// HOW TO HANDLE MOUSE POSITION
		sf::Vector2i mousePositionInt = sf::Mouse::getPosition(*window);
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
			tir1.play();
		}
		else
		{
			player->reload -= deltaTime;
		}
		//2
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && player->reload <= 0) {

			for (int i = 0; i < 5; i++) {
				aimDir = (mousePosition - player->ShipShape.getPosition());
				switch (i) {
				case 0:
					aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) + sf::Vector2f(0.2f, 0.2f);
					break;
				case 1:
					aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) - sf::Vector2f(0.2f, 0.2f);
					break; 
				case 2:
					aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) + sf::Vector2f(0.1f, 0.1f);
					break;
				case 3:
					aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2))) - sf::Vector2f(0.1f, 0.1f);
					break;
				default:
					aimDirNorm = (aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));
					break;
				}
					

				
				b1.bullet.setPosition(player->ShipShape.getPosition());
				b1.currentVelocity = aimDirNorm * (b1.maxSpeed + .5f);
				bullets.push_back(Bullet(b1));
		}
			tir2.play();

			player->reload = 2.0f;
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
				player->ShipShape.move(aimDirNorm*5.f);
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
			sf::Vector2i fire = sf::Vector2i(bullets[i].bullet.getPosition().x, bullets[i].bullet.getPosition().y);
			shot.setEmitter(window->mapPixelToCoords(fire));
			sf::Time Shotelapsed = shotClock.restart();
			shot.update(Shotelapsed);
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

		sf::Vector2i mouse = sf::Vector2i(player->ShipShape.getPosition().x, player->ShipShape.getPosition().y);
		particles.setEmitter(window->mapPixelToCoords(mouse));

		sf::Time elapsed = ParticuleClock.restart();
		particles.update(elapsed);

		//SPAWN ENNEMIES
		//BUG A REGLER: les child ne se positionnent pas vers le mother. il faut affecter le vaisseau mere a une valeur
		if (actualSpawnDelay <= 0 && gameHasStarted) {
			int offset = 30;
			ennemy.push_back({ player, window ,offset, comptScore}); // summon random ennemy
			int n = ennemy.size(); // n-1 = last element of the vector (enfin avec les bugs c pa vré... starf')
			motherShipPosition = ennemy[n - 1].shape.getPosition();
			randomPercent = rand() % 100;
			if (ennemy[n - 1].type) { //if the ennemy is a mother ship
				ennemy[n - 1].shape.setRadius(20.0f);
				ennemy[n - 1].shape.setFillColor(sf::Color(255, 0, 0, 255));
				nbLittleShips = rand() % (nbLittleShipsRandMax - nbLittleShipsRandMin) + nbLittleShipsRandMin; // nb child ship count
				for (int i = 0; i < nbLittleShips; i++) {
					ennemy.push_back({ player, window,offset,comptScore }); //create ennemy at random Window position (pos will change next line)
					/*create random Offset*/
					
					motherShipPositionOffset.x = (rand() % randomLittleShipOffsetMax) - randomLittleShipOffsetMin;
					motherShipPositionOffset.y = (rand() % randomLittleShipOffsetMax) - randomLittleShipOffsetMin;
					while (((motherShipPositionOffset.x < -2 && motherShipPositionOffset.x > -1) && (motherShipPositionOffset.x < 1 && motherShipPositionOffset.x > 2))
						 && ((motherShipPositionOffset.y < -2 && motherShipPositionOffset.y > -1) && (motherShipPositionOffset.y < 1 && motherShipPositionOffset.y > 2))
						) { //detect if the child ship is too near the mother ship... it redo the position calcul
						motherShipPositionOffset.x = (rand() % randomLittleShipOffsetMax) - randomLittleShipOffsetMin;
						motherShipPositionOffset.y = (rand() % randomLittleShipOffsetMax) - randomLittleShipOffsetMin;
					}
					n = ennemy.size();
					ennemy[n - 1].shape.setPosition(motherShipPosition + motherShipPositionOffset);
					ennemy[n - 1].shape.setFillColor(sf::Color(0, 0, 255, 255));
					
					ennemy[n - 1].EnnemyDir = player->ShipShape.getPosition() - ennemy[n - 1].shape.getPosition();
					ennemy[n - 1].EnnemyDirNorm = ennemy[n - 1].EnnemyDir / sqrt(pow(ennemy[n - 1].EnnemyDir.x,  2.0f) + pow(ennemy[n - 1].EnnemyDir.y, 2.0f));
					ennemy[n - 1].shape.move(ennemy[i].EnnemyDirNorm* ennemy[n-1].speed*deltaTime);
				}
			}
			actualSpawnDelay = spawnDelay;
		}
		else
		{
			actualSpawnDelay -= deltaTime;
		}
		

		
		window->clear();


		for (size_t i = 0; i < ennemy.size() && ennemy.size() > 0; i++)
		{
			ennemy[i].shape.move(ennemy[i].EnnemyDirNorm* ennemy[i].speed*deltaTime);
			window->draw(ennemy[i].shape);
			for (size_t j = 0; j < bullets.size(); j++)
			{
				//ENNEMY DESTROY
				if (ennemy[i].shape.getGlobalBounds().contains(bullets[j].bullet.getPosition()))
				{
					ennemy[i].hp --;
					bullets.erase(bullets.begin() + j);
					if (ennemy[i].hp <= 0) {
						ennemy.erase(ennemy.begin() + i);
						comptScore++;
						score.setString(std::to_string(comptScore));
						break;
					}
				}

				//PLAYER LOOSE
				if (ennemy[i].shape.getGlobalBounds().contains(player->ShipShape.getPosition())) {
					std::cout<< "player touché" << std::endl;
					player->hp--;
					if (player->hp <= 0) {
						gameHasStarted = false;
						player->ShipShape.setFillColor(sf::Color(0, 0, 0, 0));
					}
				}
			}
		}



		for (size_t i = 0; i < bullets.size(); i++)
		{
			window->draw(bullets[i].bullet);
			window->draw(shot);
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

		//DRAWS
		sprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));
		window->draw(sprite);
		sprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));
		window->draw(sprite);
		if (dir.x > 0 || dir.y > 0 || dir.x < 0 || dir.y < 0) {
			particles.m_lifetime = sf::milliseconds((std::rand() % 2000) + 1000);
		}
		window->draw(particles);
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
