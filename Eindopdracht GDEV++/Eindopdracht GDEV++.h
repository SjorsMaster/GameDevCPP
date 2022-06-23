#pragma once
/// <summary>
/// For details on these functions, see the comments in the CPP file
/// </summary>

void checkKeys(Actor& p, float deltaTime, sf::RenderWindow& window);

void RunGame(sf::RenderWindow& window, Actor& player, std::list<Actor*>& actorList, sf::Text& text, sf::Sound& sound, int& points, int& missed, float& deltaTime, sf::Clock& clock, sf::Texture& enemySprite);

void SetUpGame(sf::Text& score, int& points, int& missed, sf::Sound& sound, Actor& player, std::list<Actor*>& actorList, int& state);

void spawnEntity(std::list<Actor*>& actorList, sf::Texture& texture, Vector2 windowWH);








