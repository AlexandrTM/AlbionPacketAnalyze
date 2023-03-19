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
	//_dataLayout.printInfo();

	uint8_t idSize = _dataLayout.findFragment(0)._dataType._dataTypeSize;
	if (idSize == 2) {
		_id = net::read_int16(rawPlayer, _dataLayout.findFragment(0)._offset);
	}
	if (idSize == 4) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset);
	}
	if (idSize == 8) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset + 4);
	}
	_positionX = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset);
	_positionY = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset + 4);
}

Player Player::PlayerMove(NetworkCommand& rawPlayer)
{
	uint32_t _id = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawPlayer);

	uint8_t idSize = _dataLayout.findFragment(0)._dataType._dataTypeSize;
	if (idSize == 2) {
		_id = net::read_int16(rawPlayer, _dataLayout.findFragment(0)._offset);
	}
	if (idSize == 4) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset);
	}
	if (idSize == 8) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset + 4);
	}
	_positionX = net::read_float32big(rawPlayer, _dataLayout.findFragment(1)._offset + 9);
	_positionY = net::read_float32big(rawPlayer, _dataLayout.findFragment(1)._offset + 13);

	return Player(_id, _positionX, _positionY);
}

PlayerList::PlayerList()
{
	_playerList = {};
}
void PlayerList::newPlayer(Player player)
{
	if (_playerList.size() == 0) {
		_playerList.push_back(player);
	}
	else {
		for (size_t i = 0; i < _playerList.size(); i++) {
			if (_playerList[i]._id == player._id) {
				_playerList[i]._positionX = player._positionX;
				_playerList[i]._positionY = player._positionY;
				break;
			}
			else if (i == _playerList.size() - 1) {
				_playerList.push_back(player);
			}
		}
	}
	//_playerList.push_back(player);
}

void PlayerList::update(Player playerMove)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == playerMove._id) {
			_playerList[i]._positionX = playerMove._positionX;
			_playerList[i]._positionY = playerMove._positionY;
			break;
		}
	}
}
void PlayerList::removePlayer(Player player)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == player._id) {
			_playerList.erase(std::begin(_playerList) + i);
			break;
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
