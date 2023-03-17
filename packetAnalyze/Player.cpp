#include "pch.h"

PlayerSelf::PlayerSelf()
{
	_positionX = 0;
	_positionY = 0;
}
PlayerSelf::PlayerSelf(float_t positionX, float_t positionY)
{
	_positionX = positionX;
	_positionY = positionY;
}

PlayerSelf::PlayerSelf(NetworkCommand rawPlayer)
{
	_positionX = 0;
	_positionY = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawPlayer);

	_positionX = net::read_float32(rawPlayer, _dataLayout.findFragment(1)._offset);
	_positionY = net::read_float32(rawPlayer, _dataLayout.findFragment(1)._offset + 4);
}

void PlayerSelf::update(NetworkCommand rawPlayer)
{
	PlayerSelf _player = PlayerSelf(rawPlayer);
	if (_positionX == 0 and _positionY == 0) {
		*this = _player;
	}
	else if (_player._positionX != 0 and _player._positionY != 0) {
		_positionX = _player._positionX;
		_positionY = _player._positionY;
	}
}

Player::Player()
{
	_id = 0;
	_positionX = 0;
	_positionY = 0;
}
Player::Player(uint32_t id, float_t positionX, float_t positionY)
{
	_id = id;
	_positionX = positionX;
	_positionY = positionY;
}
Player::Player(NetworkCommand rawPlayer)
{
	_id = 0;
	_positionX = 0;
	_positionY = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawPlayer);

	if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 4) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset); }
	else if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 2) {
		_id = net::read_int16(rawPlayer, _dataLayout.findFragment(0)._offset); }
	_positionX = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset);
	_positionY = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset + 4);
	//std::cout << _id << " " << _positionX << " " << _positionY << "\n";
}
PlayerList::PlayerList()
{
	_playerList = {};
}

void PlayerList::update(Player player)
{
	if (_playerList.size() == 0) {
		_playerList.push_back(player);
	}
	else {
		for (size_t i = 0; i < _playerList.size(); i++) {
			if (_playerList[i]._id == player._id) {
				_playerList[i]._positionX = player._positionX;
				_playerList[i]._positionY = player._positionX;
				break;
			}
			else if (i == (_playerList.size() - 1)) {
				_playerList.push_back(player);
			}
		}
	}
}

size_t PlayerList::size()
{
	return _playerList.size();
}
Player& PlayerList::operator[](size_t elementIndex) 
{
	return _playerList[elementIndex];
}
