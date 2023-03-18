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
	//rawPlayer.printCommandInOneString();

	if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 4) {
		_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset); }
	else if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 2) {
		std::cout << "player id is 2 byte size";
		_id = net::read_int16(rawPlayer, _dataLayout.findFragment(0)._offset); }
	_positionX = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset);
	_positionY = net::read_float32(rawPlayer, _dataLayout.findFragment(13)._offset + 4);
	//std::cout << _id << " " << _positionX << " " << _positionY << "\n";
}

Player Player::PlayerMove(NetworkCommand rawPlayer)
{
	uint32_t _id = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawPlayer);

	_id = net::read_int32(rawPlayer, _dataLayout.findFragment(0)._offset + 4);
	_positionX = net::read_float32big(rawPlayer, _dataLayout.findFragment(1)._offset + 9);
	_positionY = net::read_float32big(rawPlayer, _dataLayout.findFragment(1)._offset + 13);
	//std::cout << _id << "\n";
	//std::cout << _positionX << " " << _positionY << "\n";//12 16, 21 25, 16 12
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
}

void PlayerList::update(Player playerMove)
{
	for (size_t i = 0; i < _playerList.size(); i++) {
		if (_playerList[i]._id == playerMove._id) {
			//std::cout << sqrt(pow(_playerList[i]._positionX - playerMove._positionX, 2) + 
			//				  pow(_playerList[i]._positionY - playerMove._positionY, 2)) << "\n";
			_playerList[i]._positionX = playerMove._positionX;
			_playerList[i]._positionY = playerMove._positionY;
			//std::cout << std::setw(9) << playerMove._positionX << " " << std::setw(9) << playerMove._positionY << "\n";
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

size_t PlayerList::size()
{
	return _playerList.size();
}
Player& PlayerList::operator[](size_t elementIndex) 
{
	return _playerList[elementIndex];
}
