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

	DataFragment& positionFragment = dataLayout.findFragment(1);

	// important
	if (positionFragment._offset != std::numeric_limits<ptrdiff_t>::min()) {
		_positionX = net::read_float32(rawPlayer, dataLayout.findFragment(1)._offset);
		_positionY = net::read_float32(rawPlayer, dataLayout.findFragment(1)._offset + 4);
	}

	//std::cout << "_positionX: " << _positionX << " _positionY: " << _positionY << "\n";
}

void PlayerSelf::update(NetworkCommand& rawPlayer)
{
	PlayerSelf player = PlayerSelf(rawPlayer);
	_positionX = player._positionX;
	_positionY = player._positionY;
	//std::cout << "_positionX: " << _positionX << " _positionY: " << _positionY << "\n";
}
void PlayerSelf::printInfo() const
{
	std::cout << "name: " << _name << " guild: " << _guild << " alliance: " << _alliance << "\n";
}

Player::Player()
{
	_id		   = 0;
	_health    = 0;
	_positionX = 0;
	_positionY = 0;
	_isVisible = false;
}
Player::Player(uint64_t id, uint32_t health, float_t positionX, float_t positionY, bool isVisible)
{
	_id        = id;
	_health    = health;
	_positionX = positionX;
	_positionY = positionY;
	_isVisible = isVisible;
}
void Player::printInfo() const
{
	std::cout << "name: " << _name << " guild: " << _guild << " alliance: " << _alliance << 
		" X: " << _positionX << " Y:" << _positionY << "\n";
}
Player::Player(NetworkCommand& rawPlayer) // find health
{
	_id        = 0;
	_health    = 0;
	_positionX = -100;
	_positionY = 500;
	_isVisible = true;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawPlayer);

	_id = net::read_integer(rawPlayer, dataLayout.findFragment(0));

	if (dataLayout.size() > 2) {
		DataFragment& playerNameFragment = dataLayout.findFragment(1);
		DataFragment& playerGuildFragment = dataLayout.findFragment(8);
		DataFragment& playerAllianceFragment = dataLayout.findFragment(51);

		_name = net::readString(rawPlayer, playerNameFragment);
		_guild = net::readString(rawPlayer, playerGuildFragment);
		_alliance = net::readString(rawPlayer, playerAllianceFragment);
		//dataLayout.printInfo(rawPlayer);
	}

	//printInfo();
	
	// проверил прямые и обратные пары, проверил первое и второе число отрицательное
	// 20 возможно какой-то коэффициент, у числа короткая дробная часть
	// 30 35 очень маленькие числа возможно скорость ?
	// 19 25 скорее всего координаты но преобразованные

	/*_positionX = -net::read_float32(rawPlayer, dataLayout.findFragment(19)._offset);
	_positionY = net::read_float32(rawPlayer, dataLayout.findFragment(25)._offset);*/
}

PlayerList::PlayerList()
{
	_playerList = {};
}

void PlayerList::update(HealthUpdateHandler healthUpdate)
{
	for (auto& player : _playerList) {
		if (player._id == healthUpdate._id) {
			player._health = healthUpdate._health;
			return;
		}
	}
}

void PlayerList::newPlayer(Player newPlayer)
{
	for (auto& player: _playerList) {
		if (player._id == newPlayer._id) {
			player._health = newPlayer._health;
			player._isVisible = true;
			return;
		}
	}
	_playerList.push_back(newPlayer);
}
void PlayerList::playerLeave(Player playerLeave)
{
	for (auto& player : _playerList) {
		if (player._id == playerLeave._id) {
			player._isVisible = false;
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

size_t PlayerList::size() const
{
	return _playerList.size();
}
Player& PlayerList::operator[](size_t elementIndex) 
{
	return _playerList[elementIndex];
}
