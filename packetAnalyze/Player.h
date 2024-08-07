#ifndef  PLAYER_H
#define PLAYER_H

struct PlayerSelf
{
	float_t _positionX;
	float_t _positionY;

	PlayerSelf();
	PlayerSelf(float_t positionX, float_t positionY);
	PlayerSelf(NetworkCommand& rawPlayer);

	void update(NetworkCommand& rawPlayer);
};

struct Player
{
	uint32_t _id;
	float_t _positionX;
	float_t _positionY;
	bool _isVisible;

	Player();
	Player(NetworkCommand& rawPlayer);
	Player(uint32_t id, float_t positionX, float_t positionY);
};

class PlayerList
{
private:
	std::vector<Player> _playerList;

public:
	PlayerList();
	
	void newPlayer(Player player);
	void update(EntityMove playerMove);
	void playerLeave(Player playerLeave);

	void removePlayer(Player player);
	void removePlayer(size_t elementIndex);

	size_t size();
	Player& operator[](size_t elementIndex);
};

#endif // ! PLAYER_H