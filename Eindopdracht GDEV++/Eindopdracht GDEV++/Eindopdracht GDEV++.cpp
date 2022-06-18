// Eindopdracht GDEV++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include "Maths.h" 
#include "Player.h" 
#include "AngyMan.h" 
#include "Eindopdracht GDEV++.h"


Vector2 windowWH(720, 720);

enum Gamestate {
	Start,
	Playing,
	GameOver,
	Win,
	Pause
};
int state = Start;

//caused some issues when it wasn't a pointer, learned it the hard way haha
std::list<AngyMan*> angymanList;

sf::Texture texture;
sf::Texture texture2;

int main() {
	//Should remove the full location name after, but for debugging it doesn't load the sprites otherwise
	texture.loadFromFile("sjors is a bottom2.png");
	texture2.loadFromFile("ooppppp.png");

	srand(time(NULL));

	sf::Clock clock;
	std::cout << "Hello World! Starting program.." << std::endl;

	sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);


	sf::Font font = sf::Font();

	//get font from location
	font.loadFromFile("arial.ttf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(10, 10);
	text.setStyle(sf::Text::Bold);
	text.setString("Score: ");

	bool playing = true;

	int points = 0, missed = 0;

	//Player data
	Player p;
	p.body.setTexture(texture);
	p.widthHeight.y *= 1;
	p.widthHeight.x *= .65f;
	p.fixBounds();
	
	int lastState;

	//Open a window
	sf::RenderWindow window(sf::VideoMode((unsigned int)windowWH.x, (unsigned int)windowWH.y), "CatchGame", sf::Style::Close | sf::Style::Titlebar /* | sf::Style::Resize*/);

	sf::Clock dtClock;
	float dt;

	//While the game is running
	while (window.isOpen()) {
		//Clear the last frame
		window.clear();

		//update the deltatime based on clock
		dt = dtClock.getElapsedTime().asSeconds();
		dtClock.restart();

		//switch statement for all the gamestates
		switch (state) {
		case Start:
			//start screen
			text.setPosition(windowWH.x / 2 - text.getLocalBounds().width / 2, windowWH.y / 2);
			text.setString("Press enter to start!");
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				SetUpGame(text, points, missed, sound, p, angymanList, state);
			}
			break;
		case Playing:
			//run game function with all the data
			RunGame(window, p, angymanList, text, sound, points, missed, dt, clock, texture2);
			break;
		case GameOver:
			//game over screen
			text.setPosition(windowWH.x / 2 - text.getLocalBounds().width / 2, windowWH.y / 2);
			text.setString("Game over!\nYou scored: " + std::to_string(points));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				SetUpGame(text, points, missed, sound, p, angymanList, state);
			}
			break;
		case Win:
			//win screen
			break;
		case Pause:
			text.setString("Paused");
			break;
		}

		sf::Event evnt;
		//Interactions with the window
		while (window.pollEvent(evnt)) {
			switch (evnt.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::LostFocus:
				//std::cout << "\a"; //Out of focus beepboopboop :)
				lastState = state;
				state = Pause;
				break;
				/*
				case sf::Event::Resized:
					windowWH.x = evnt.size.width;
					windowWH.y = evnt.size.height;
					break;
				*/
			case sf::Event::GainedFocus:
				state = lastState;
				text.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
				break;
			}
		}
		checkKeys(p, dt);

		window.draw(text);

		//Display game!!!
		window.display();
	}
	std::cout << "Goodbye World! Leaving program.." << std::endl;
}

/// [!] They want the friction in a rigibody class oop! Do it with weight too!!
void checkKeys(Player& p, float deltaTime) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//Move left
		p.moveSpeed -= p.acceleration * deltaTime;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//Move right
		p.moveSpeed += p.acceleration * deltaTime;
	}
	else if (abs(p.moveSpeed) < 0.01) {
		//Stop
		p.moveSpeed = 0;
	}
	else if (p.moveSpeed > 0) {
		p.moveSpeed -= p.acceleration * deltaTime;
	}
	else if (p.moveSpeed < 0) {
		p.moveSpeed += p.acceleration * deltaTime;
	}
	p.position.x += p.moveSpeed * deltaTime * p.speed;
}

void RunGame(sf::RenderWindow& window, Actor& player, std::list <AngyMan*>& angymanList, sf::Text& text, sf::Sound& sound, int& points, int& missed, float& deltaTime, sf::Clock& clock, sf::Texture& enemySprite) {
	//Update positions
	player.SetPos(Vector2(player.position.x, player.position.y));

	FixBounds(player);

	//for each enemy in angymanlist window update
	for (auto iter = angymanList.begin(); iter != angymanList.end();) {
		// get an angyman from the index of iter out of the list
		AngyMan* badguy = *iter;

		badguy->Update(deltaTime);

		window.draw(badguy->body);

		//badguy. //fetch distance
		//check collision between player and angyman
		if (CheckCollision(&player, badguy)) {
			points++;
			text.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
			
			//badguy->moveSpeed *= -1;
			
			std::cout << *iter << ": Captured!" << std::endl;
			iter = angymanList.erase(iter);
			sound.play();
			badguy->~AngyMan();
			delete badguy;
		}
		//Out of view
		else if (badguy->position.y > windowWH.y) {
			std::cout << *iter << ": Missed!" << std::endl;
			missed++;
			text.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
			iter = angymanList.erase(iter);
			badguy->~AngyMan();
			delete badguy;
		}
		else {
			iter++;
		}
	}

	//spawn enemy every 5 seconds
	if (clock.getElapsedTime().asSeconds() > (10 / (points + 1))) {
		spawnEnemy(angymanList, enemySprite);
		clock.restart();
	}

	if (missed >= 3) {
		missed = 3;
		state = GameOver;
	}

	//Draw stuff
	window.draw(player.body);
}

void spawnEnemy(std::list <AngyMan*>& angymanList, sf::Texture& texture) {
	//create a new angyman
	AngyMan* enemy = new AngyMan();
	enemy->body.setTexture(texture);

	enemy->widthHeight.y *= .85 / 2;
	enemy->widthHeight.x *= 0.5 / 2;

	enemy->fixBounds();
	int boundsCorrector = std::abs((unsigned int)windowWH.x - enemy->widthHeight.x);
	enemy->SetPos(Vector2(rand() % boundsCorrector, -enemy->widthHeight.y));
	enemy->body.setColor(sf::Color(rand() % 250, rand() % 250, rand() % 250));

	enemy->moveSpeed = 1;
	angymanList.push_back(enemy);
}

//Using an enum for gamestate in function seemed to cause some issues, so I'm using an int instead as recommended by others online
void SetUpGame(sf::Text& score, int& points, int& missed, sf::Sound& sound, Actor& player, std::list <AngyMan*>& angymanList, int& state) {
	//Player stuff setup
	player.moveSpeed = 0;
	player.SetPos(Vector2(windowWH.x / 2 - player.widthHeight.x / 2, (windowWH.y * .9f) - player.widthHeight.y / 2));

	//Score values setup
	points = 0;
	missed = 0;
	
	//Text setup
	score.setPosition(10, 10);
	score.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
	
	//Sound setup
	sound.play();
	

	for (AngyMan* a : angymanList) {
		a->~AngyMan();
		delete a;
	}
	angymanList.clear();

	//Set the gamestate
	state = Playing;
}

void FixBounds(Actor& player) {
	if (player.position.x < 0) {
		player.position.x = 0;
		//p.moveSpeed = 0;
		player.moveSpeed = -player.moveSpeed;
	}
	else if (player.position.x + player.widthHeight.x > windowWH.x) {
		player.position.x = windowWH.x - player.widthHeight.x;
		//p.moveSpeed = 0;
		player.moveSpeed = -player.moveSpeed;
	}
}