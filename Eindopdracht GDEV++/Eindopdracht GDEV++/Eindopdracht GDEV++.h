void checkKeys(Player& p, float deltaTime);
void RunGame(sf::RenderWindow& window, Actor& player, std::list<AngyMan*>& angymanList, sf::Text& text, sf::Sound& sound, int& points, int& missed, float& deltaTime, sf::Clock& clock, sf::Texture& enemySprite);
void spawnEnemy(std::list <AngyMan*>& angymanList, sf::Texture &texture);

bool CheckCollision(Actor* A, Actor* B);

void SetUpGame(sf::Text& score, int& points, int& missed, sf::Sound& sound, Actor& player, std::list<AngyMan*>& angymanList, int& state);

void FixBounds(Actor& player);






