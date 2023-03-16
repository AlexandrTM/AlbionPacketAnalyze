#include "pch.h"

Player::Player()
{
	_positionX = 0;
	_positionY = 0;
}

Player::Player(float_t positionX, float_t positionY)
{
	_positionX = positionX;
	_positionY = positionY;
}

Player::Player(NetworkCommand rawPlayer)
{
	_positionX = 0;
	_positionY = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawPlayer);

	_positionX = net::read_float32(rawPlayer, _dataLayout.findFragment(1)._offset);
	_positionY = net::read_float32(rawPlayer, _dataLayout.findFragment(1)._offset + 4);
}

void Player::update(NetworkCommand rawPlayer)
{
	Player _player = Player(rawPlayer);
	if (_positionX == 0 and _positionY == 0) {
		*this = _player;
	}
	else if (_player._positionX != 0 and _player._positionY != 0) {
		_positionX = _player._positionX;
		_positionY = _player._positionY;
	}
}
