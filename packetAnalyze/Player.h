#ifndef  PLAYER_H
#define PLAYER_H

struct PlayerSelf
{
	float_t _positionX;
	float_t _positionY;

	std::string _name = {}; // 2 data fragment id
	std::string _guild = {}; // 57 data fragment id
	std::string _alliance = {}; // 77 data fragment id

	PlayerSelf();
	PlayerSelf(float_t positionX, float_t positionY);
	PlayerSelf(NetworkCommand& rawPlayer);

	void update(NetworkCommand& rawPlayer);
	void printInfo() const;
};

struct Player
{
	uint64_t _id;
	uint32_t _health;
	float_t _positionX;
	float_t _positionY;
	bool _isVisible;

	std::string _name = {};
	std::string _guild = {};
	std::string _alliance = {};

	Player();
	Player(NetworkCommand& rawPlayer);
	Player(uint64_t id, uint32_t health, float_t positionX, float_t positionY, bool isVisible);

	void printInfo() const;
};

struct PlayerList
{
	std::vector<Player> _playerList;

	PlayerList();
	
	void newPlayer(Player player);
	void update(HealthUpdateHandler healthUpdate);
	void playerLeave(Player playerLeave);

	void removePlayer(Player player);
	void removePlayer(size_t elementIndex);

	size_t size() const;
	Player& operator[](size_t elementIndex);
};

#endif // ! PLAYER_H