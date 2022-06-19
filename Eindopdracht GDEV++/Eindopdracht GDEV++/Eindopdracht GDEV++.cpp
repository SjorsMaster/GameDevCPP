// Eindopdracht GDEV++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include "Vector2.h" 
#include "AngyMan.h" 
#include "Eindopdracht GDEV++.h"


Vector2 windowWH(720, 720);

enum Gamestate {
	Start,
	Playing,
	GameOver,
	Pause
};
int state = Start;

//caused some issues when it wasn't a pointer, learned it the hard way haha
std::list<AngyMan*> angymanList;

sf::Texture texture;
sf::Texture texture2;

Vector2 acceleration = Vector2(750, 0);
float maxSpeed = 450;

int main() {
	//Should remove the full location name after, but for debugging it doesn't load the sprites otherwise
	texture.loadFromFile("nico.png");
	texture2.loadFromFile("mochi.png");

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
	Actor p;
	p.body.setTexture(texture);
	Vector2 tmp = p.GetWidthHeight();
	p.SetWidthHeight(Vector2(tmp.x * .65f, tmp.y * 1));
	p.fixBounds();
	p.gravity = Vector2(0, 0);
	p.friction = Vector2(400, 0);

	int lastState = -1;

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
				lastState = state;
				state = Pause;
				break;
			case sf::Event::GainedFocus:
				state = lastState;
				text.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
				break;
			}
		}
		checkKeys(p, dt, window);

		window.draw(text);

		//Display game!!!
		window.display();
	}
	std::cout << "Goodbye World! Leaving program.." << std::endl;
}

/// [!] They want the friction in a rigibody class oop! Do it with weight too!!
void checkKeys(Actor& p, float deltaTime, sf::RenderWindow& window) {
	//We multiply it by time so that the acceleration value is around the same as gravity and friction
	Vector2 accelerationPerFrame = acceleration * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		p.ApplyForce(-accelerationPerFrame);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		p.ApplyForce(accelerationPerFrame);
	}

	p.UpdatePhysics(deltaTime, window);
}

void RunGame(sf::RenderWindow& window, Actor& player, std::list <AngyMan*>& angymanList, sf::Text& text, sf::Sound& sound, int& points, int& missed, float& deltaTime, sf::Clock& clock, sf::Texture& enemySprite) {
	//Update positions and sprite pos
	player.SetPos(Vector2(player.GetPos().x, player.GetPos().y));

	//for each enemy in angymanlist window update
	for (auto iter = angymanList.begin(); iter != angymanList.end();) {
		// get an angyman from the index of iter out of the list
		AngyMan* badguy = *iter;

		badguy->UpdatePhysics(deltaTime, window);
		badguy->body.setPosition(badguy->GetPos().x, badguy->GetPos().y);

		//std::cout << "poepen " << badguy->GetPos().x << " plassen " << badguy->GetPos().y << std::endl;

		window.draw(badguy->body);

		//check collision between player and angyman
		if (player.CheckCollision(badguy)) {
			points++;
			text.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));
			std::cout << *iter << ": Captured!" << std::endl;
			iter = angymanList.erase(iter);
			sound.setPitch((float)(rand() % 100 + 10) / 100);
			sound.play();
			badguy->~AngyMan();
			delete badguy;
		}
		//Out of view
		else if (badguy->GetPos().y > windowWH.y) {
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

	float& speed = player.moveSpeed.x;
	if (abs(speed) > maxSpeed) {
		speed = speed > 0 ? maxSpeed : -maxSpeed;
	}

	//spawn enemy based on score
	float pps = (points + 6) * 0.03f;
	if (clock.getElapsedTime().asSeconds() > 1 / pps) {
		spawnEnemy(angymanList, enemySprite, windowWH);
		clock.restart();
	}

	if (missed >= 3) {
		missed = 3;
		std::cout << "-= GAME OVER =-" << std::endl;
		for (AngyMan* a : angymanList) {
			std::cout << a << ": Cleared from memory!" << std::endl;
			a->~AngyMan();
			delete a;
		}
		angymanList.clear();

		state = GameOver;
	}

	//Draw stuff
	window.draw(player.body);
}


//Using an enum for gamestate in function seemed to cause some issues, so I'm using an int instead as recommended by others online
void SetUpGame(sf::Text& score, int& points, int& missed, sf::Sound& sound, Actor& player, std::list <AngyMan*>& angymanList, int& state) {
	//Player stuff setup
	std::cout << "-= PREPARING GAME =-" << std::endl;

	player.moveSpeed = Vector2(0, 0);
	player.SetPos(
		Vector2(
			windowWH.x / 2 - player.GetWidthHeight().x / 2,
			(windowWH.y * .9f) - player.GetWidthHeight().y / 2
		)
	);

	//Score values setup
	points = 0;
	missed = 0;

	//Text setup
	score.setPosition(10, 10);
	score.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));

	//Sound setup
	sound.play();

	//Set the gamestate
	state = Playing;
}