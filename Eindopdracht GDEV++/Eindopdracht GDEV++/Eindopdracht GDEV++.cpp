// Eindopdracht GDEV++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include "Actor.h" 
#include "Vector2.h" 
#include "Eindopdracht GDEV++.h"

//The window size
Vector2 windowWH(720, 720);

//State related stuff, such as the current state of the game, 
//the last state when coming from pausing and the possible states.
//The enum makes the code more readable
enum Gamestate {
	Start,
	Playing,
	GameOver,
	Pause
};
int state = Start;
int lastState = -1;

//caused some issues when it wasn't a pointer, learned it the hard way haha this is an actorList
std::list<Actor*> actorList;

//Textures for sprites, 2 is for the collectibles
sf::Texture texture, texture2;

//Acceleration for the player
Vector2 acceleration = Vector2(750, 0);

//Player maximum speed
float maxSpeed = 450;

//The amount we're allowed to miss ingame
int allowedToMiss = 5;

//Program
int main() {

	//Should remove the full location name after, but for debugging it doesn't load the sprites otherwise
	texture.loadFromFile("nico.png");
	texture2.loadFromFile("mochi.png");

	//Randomize the seed of the game every time
	srand(time(NULL));

	//A clock to keep track of the time for spawning entities,
	//And a deltatime clock to keep all movements clean and consistant on all devices
	sf::Clock clock;
	sf::Clock dtClock;
	float dt;
	
	//Welcome text
	std::cout << "Hello World! Starting program.." << std::endl;

	//Sound related stuff
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);

	//Text related stuff
	sf::Font font = sf::Font();
	font.loadFromFile("arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(10, 10);
	text.setStyle(sf::Text::Bold);
	text.setString("Score: ");
	
	//For the out of focus pause
	bool playing = true;

	//Player stats
	int points = 0, missed = 0;

	//PlayerCharacter data
	Actor player;
	player.body.setTexture(texture);
	Vector2 tmp = player.GetWidthHeight();
	player.SetWidthHeight(Vector2(tmp.x * .65f, tmp.y * 1));
	player.fixBounds();
	player.gravity = Vector2(0, 0);
	player.friction = Vector2(400, 0);

	//Open a window
	sf::RenderWindow window(sf::VideoMode(
		(unsigned int)windowWH.x, (unsigned int)windowWH.y),
		"Tale of Nico and the Mochis", sf::Style::Close | sf::Style::Titlebar
	);

	std::string menuText = "Welcome to the game!\n\n"
		"Use the left [<] and right [>] arrow keys to move.\n\n"
		"Collect the mochi to gain points!\n"
		"The more points you have, the more mochi will appear!\n\n"
		"You're only allowed to miss "
		+ std::to_string(allowedToMiss) + " mochi!\n\n"
		"Press enter to start";

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
			//Move the text to the center of the screen
			text.setPosition(
				windowWH.x / 2 - text.getLocalBounds().width / 2,
				windowWH.y / 2 - text.getLocalBounds().height / 2
			);
			
			//Update the text
			text.setString(menuText);
			
			//Check input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				SetUpGame(text, points, missed, sound, player, actorList, state);
			}
			break;
		case Playing:
			//run game function with all the data
			RunGame(window, player, actorList, text, sound, points, missed, dt, clock, texture2);
			break;
		case GameOver:
			//Update the text
			menuText = "Game Over!\n\n"
				"You collected " + std::to_string(points) + " mochi!\n\n"
				"Press enter to restart";
			
			//Move to start
			state = Start;
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
				text.setString(
					"Score: " + std::to_string(points) +
					"\nMissed: " + std::to_string(missed)
				);
				break;
			}
		}
		checkKeys(player, dt, window);

		window.draw(text);

		//Display game!!!
		window.display();
	}
	std::cout << "Goodbye World! Leaving program..\n    Nico says bye! -w-" << std::endl;
}

//This handles the input and applies a force to the player as well as update the physics
void checkKeys(Actor& player, float deltaTime, sf::RenderWindow& window) {
	//We multiply it by time so that the acceleration value is around the same as gravity and friction
	Vector2 accelerationPerFrame = acceleration * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		player.ApplyForce(-accelerationPerFrame);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		player.ApplyForce(accelerationPerFrame);
	}
	
	//Update said forces
	player.UpdatePhysics(deltaTime, window);
}

//Gameplay and everything handled in it
void RunGame(sf::RenderWindow& window, Actor& player, std::list <Actor*>& actorList, sf::Text& text, sf::Sound& sound, int& points, int& missed, float& deltaTime, sf::Clock& clock, sf::Texture& enemySprite) {
	//Update positions and sprite pos
	player.SetPos(Vector2(player.GetPos().x, player.GetPos().y));
	window.draw(player.body);

	//for each enemy in angymanlist window update
	for (auto iter = actorList.begin(); iter != actorList.end();) {
		// get an angyman from the index of iter out of the list
		Actor* entity = *iter;

		//update character physics and position, then draw it
		entity->UpdatePhysics(deltaTime, window);
		entity->body.setPosition(entity->GetPos().x, entity->GetPos().y);
		window.draw(entity->body);

		//check collision between player and entity, add points if needed, play a sound and clean up the entity
		if (player.CheckCollision(entity)) {
			points++;
			text.setString(
				"Score: " + std::to_string(points) +
				"\nMissed: " + std::to_string(missed)
			);
			std::cout << *iter << ": Captured!" << std::endl;
			iter = actorList.erase(iter);
			sound.setPitch((float)(rand() % 100 + 10) / 100);
			sound.play();
			entity->~Actor();
			delete entity;
		}
		//Out of view, add missed and clean up the mochi
		else if (entity->GetPos().y > windowWH.y) {
			std::cout << *iter << ": Missed!" << std::endl;
			missed++;
			text.setString(
				"Score: " + std::to_string(points) +
				"\nMissed: " + std::to_string(missed)
			);
			iter = actorList.erase(iter);
			entity->~Actor();
			delete entity;
		}
		else {
			iter++;
		}
	}

	//Limit the playerspeed, put it in speed otherwise I had to type out movespeed.x constantly
	float& speed = player.moveSpeed.x;
	if (abs(speed) > maxSpeed) {
		speed = speed > 0 ? maxSpeed : -maxSpeed;
	}

	//spawn enemy based on score, funky formula, Byte helped me figure this out, awesome math teacher go brr
	float pps = (points + 6) * 0.03f;
	if (clock.getElapsedTime().asSeconds() > 1 / pps) {
		spawnEntity(actorList, enemySprite, windowWH);
		clock.restart();
	}

	//Keep track of the missed points and perform actions if needed
	if (missed >= allowedToMiss) {
		missed = allowedToMiss;
		std::cout << "-= GAME OVER =-" << std::endl;
		
		//Clean up the mess I made :)
		for (Actor* a : actorList) {
			std::cout << a << ": Cleared from memory!" << std::endl;
			a->~Actor();
			delete a;
		}
		actorList.clear();
		
		//Exit to gameover
		state = GameOver;
	}
}

//Using an enum for gamestate in function seemed to cause some issues, so I'm using an int instead as recommended by others online
//This prepares the game for play, it sets some values as it might still be stored when the player has died!
void SetUpGame(sf::Text& score, int& points, int& missed, sf::Sound& sound, Actor& player, std::list <Actor*>& angymanList, int& state) {
	//Player stuff setup
	std::cout << "-= PREPARING GAME =-" << std::endl;

	//Reset player speed
	player.moveSpeed = Vector2(0, 0);
	//Reset player position - width height
	player.SetPos(
		Vector2(
			windowWH.x / 2 - player.GetWidthHeight().x / 2,
			(windowWH.y * .9f) - player.GetWidthHeight().y / 2
		)
	);

	//Score values setup
	points = 0;
	missed = 0;

	//Text setup for score
	score.setPosition(10, 10);
	score.setString("Score: " + std::to_string(points) + "\nMissed: " + std::to_string(missed));

	//Play sound at the start of the game
	sound.play();

	//Set the gamestate
	state = Playing;
}

//Spawns an entity at a random position
void spawnEntity(std::list <Actor*>& actorList, sf::Texture& texture, Vector2 windowWH) {
	//Create a cool new entity
	Actor* entity = new Actor();
	entity->body.setTexture(texture);

	//Fix the sprite
	entity->SetWidthHeight(Vector2(
		entity->GetWidthHeight().x * 0.25f,
		entity->GetWidthHeight().y * 0.2f)
	);

	//Set some stats
	entity->gravity = Vector2(0, 100);
	entity->friction = Vector2(0, 0);
	entity->moveSpeed = Vector2(0, 0);
	
	//Fix the colliders cause the width does normally not equal the width of the sprite
	entity->fixBounds();
	
	//Set a position
	int boundsCorrector = std::abs((unsigned int)windowWH.x - entity->GetWidthHeight().x);
	entity->SetPos(Vector2(rand() % boundsCorrector, -entity->GetWidthHeight().y));
	
	//Give a color
	entity->body.setColor(sf::Color(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));

	//Put it into the list! BOOM, we have an entity!
	actorList.push_back(entity);
}