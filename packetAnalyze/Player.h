#ifndef  PLAYER_H
#define PLAYER_H

struct Player
{
	float_t _positionX;
	float_t _positionY;

	Player();
	Player(float_t positionX, float_t positionY);
	Player(NetworkCommand rawPlayer);

	void update(NetworkCommand rawPlayer);
};

#endif // ! PLAYER_H