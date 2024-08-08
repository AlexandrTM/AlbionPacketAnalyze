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

PlayerSelf::PlayerSelf(NetworkCommand& rawPlayer)
{
	_positionX = 0;
	_positionY = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawPlayer);

	_positionX = net::read_float32(rawPlayer, dataLayout.findFragment(1)._offset);
	_positionY = net::read_float32(rawPlayer, dataLayout.findFragment(1)._offset + 4);
}

void PlayerSelf::update(NetworkCommand& rawPlayer)
{
	PlayerSelf player = PlayerSelf(rawPlayer);
	if (_positionX == 0 and _positionY == 0) {
		*this = player;
	}
	else if (player._positionX != 0 and player._positionY != 0) {
		_positionX = player._positionX;
		_positionY = player._positionY;
	}
}

Player::Player()
{
	_id		   = 0;
	_health    = 0;
	_positionX = 0;
	_positionY = 0;
	_isVisible = false;
}
Player::Player(uint32_t id, uint32_t health, float_t positionX, float_t positionY, bool isVisible)
{
	_id        = id;
	_health    = health;
	_positionX = positionX;
	_positionY = positionY;
	_isVisible = isVisible;
}
Player::Player(NetworkCommand& rawPlayer) // find health
{
	_id        = 0;
	_health    = 0;
	_positionX = 0;
	_positionY = 0;
	_isVisible = true;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawPlayer);
	//_dataLayout.printInfo(rawPlayer);

	DataFragment idFragment = dataLayout.findFragment(0);
	uint8_t idSize = idFragment._dataType._size;
	if 
		(idSize == 1) { _id = net::read_uint8 (rawPlayer, idFragment._offset); }
	else if 
		(idSize == 2) { _id = net::read_uint16(rawPlayer, idFragment._offset); }
	else if 
		(idSize == 4) { _id = net::read_uint32(rawPlayer, idFragment._offset); }
	else if 
		(idSize == 8) { _id = net::read_uint32(rawPlayer, idFragment._offset + 4); }

	_positionX = net::read_float32(rawPlayer, dataLayout.findFragment(14)._offset);
	_positionY = net::read_float32(rawPlayer, dataLayout.findFragment(14)._offset + 4);
}

PlayerList::PlayerList()
{
	_playerList = {};
}

void PlayerList::update(EntityMove playerMove)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == playerMove._id) {
			_playerList[i]._positionX = playerMove._positionX;
			_playerList[i]._positionY = playerMove._positionY;
			return;
		}
	}
}

void PlayerList::update(HealthUpdate healthUpdate)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == healthUpdate._id) {
			_playerList[i]._health = healthUpdate._health;
			return;
		}
	}
}

void PlayerList::newPlayer(Player player)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == player._id) {
			_playerList[i]._health    = player._health;
			_playerList[i]._positionX = player._positionX;
			_playerList[i]._positionY = player._positionY;
			_playerList[i]._isVisible = true;
			return;
		}
	}
	_playerList.push_back(player);
}
void PlayerList::playerLeave(Player playerLeave)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == playerLeave._id) {
			_playerList[i]._isVisible = false;
			return;
		}
	}
}
void PlayerList::removePlayer(Player player)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == player._id) {
			_playerList.erase(_playerList.begin() + i);
			return;
		}
	}
}
void PlayerList::removePlayer(size_t elementIndex)
{
	_playerList.erase(std::begin(_playerList) + elementIndex);
}

size_t PlayerList::size()
{
	return _playerList.size();
}
Player& PlayerList::operator[](size_t elementIndex) 
{
	return _playerList[elementIndex];
}
