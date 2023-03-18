#ifndef  PLAYER_H
#define PLAYER_H

struct PlayerSelf
{
	float_t _positionX;
	float_t _positionY;

	PlayerSelf();
	PlayerSelf(float_t positionX, float_t positionY);
	PlayerSelf(NetworkCommand rawPlayer);

	void update(NetworkCommand rawPlayer);
};

struct Player
{
	uint32_t _id;
	float_t _positionX;
	float_t _positionY;

	Player();
	Player(uint32_t id, float_t positionX, float_t positionY);
	Player(NetworkCommand rawPlayer);
	static Player PlayerMove(NetworkCommand rawPlayer);
};

class PlayerList
{
private:
	std::vector<Player> _playerList;

public:
	PlayerList();
	
	void newPlayer(Player player);
	void update(Player playerMove);
	void removePlayer(Player player);

	size_t size();
	Player& operator[](size_t elementIndex);
};

#endif // ! PLAYER_H