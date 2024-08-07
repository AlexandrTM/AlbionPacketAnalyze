#include "pch.h"

Mob::Mob()
{
	_id			 = 0;
	_positionX	 = 0;
	_positionY	 = 0;
	_health		 = 0;
	_tier		 = 0;
	_enchantment = 0;
}

Mob::Mob(NetworkCommand& rawMob)
{
	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawMob);

	DataFragment idFragment     = dataLayout.findFragment(0);
	DataFragment healthFragment = dataLayout.findFragment(14);
	DataFragment tierFragment   = dataLayout.findFragment(21);

	uint8_t idSize = idFragment._dataType._size;
	if 
		(idSize == 1) { _id = net::read_uint8(rawMob, idFragment._offset); }
	else if 
		(idSize == 2) { _id = net::read_uint16(rawMob, idFragment._offset); }
	else if 
		(idSize == 4) { _id = net::read_uint32(rawMob, idFragment._offset); }
	else if 
		(idSize == 8) { _id = net::read_uint32(rawMob, idFragment._offset + 4); }

	_health    = net::read_float32(rawMob, healthFragment._offset);
	_positionX = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
	_positionY = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);

	/*std::cout << 
		"x:  "			<< std::setw(6) << _positionX			  << " " <<
		"y:  "			<< std::setw(6) << _positionY			  << " " << "\n" <<
		"x1: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset) << " " <<
		"x2: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset + 4) << "\n";*/

	std::cout << 
		"some1:  " << net::read_uint16 (rawMob, dataLayout.findFragment(1 )._offset) << " " << "\n" <<
		"some10: " << net::read_float32(rawMob, dataLayout.findFragment(10)._offset) << " " << "\n" <<
		"some11: " << net::read_float32(rawMob, dataLayout.findFragment(11)._offset) << " " << "\n" <<
		"some12: " << net::read_float32(rawMob, dataLayout.findFragment(12)._offset) << " " << "\n" <<
		"some13: " << net::read_float32(rawMob, dataLayout.findFragment(13)._offset) << " " << "\n" <<
		"some14: " << net::read_float32(rawMob, dataLayout.findFragment(14)._offset) << " " << "\n" <<
		"some15: " << net::read_float32(rawMob, dataLayout.findFragment(15)._offset) << " " << "\n" <<
		"some16: " << net::read_float32(rawMob, dataLayout.findFragment(16)._offset) << " " << "\n" <<
		"some17: " << net::read_uint32 (rawMob, dataLayout.findFragment(17)._offset) << " " << "\n" <<
		"some18: " << net::read_float32(rawMob, dataLayout.findFragment(18)._offset) << " " << "\n" <<
		"some19: " << net::read_float32(rawMob, dataLayout.findFragment(19)._offset) << " " << "\n";

	std::cout << 
		"id: "			<< std::setw(7) << (unsigned)_id		  << " " << "\n" <<
		"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " << "\n" <<
		"health: "      << std::setw(2) << (unsigned)_health	  << " " << "\n" <<
		"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " << "\n" <<
		"x: "			<< std::setw(7) << _positionX			  << " " << "\n" <<
		"y: "			<< std::setw(7) << _positionY			  << " " << "\n" <<
		"\n";

	dataLayout.printInfo(rawMob);
}

void Mob::printInfo()
{
	std::cout << 
		"id: "			<< std::setw(7) << (unsigned)_id		  << " " <<
		"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " <<
		"health: "      << std::setw(2) << (unsigned)_health	  << " " <<
		"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " <<
		"x: "			<< std::setw(6) << _positionX			  << " " <<
		"y: "			<< std::setw(6) << _positionY			  << "\n";
}

MobList::MobList()
{
	_mobList = {};
}

void MobList::newMob(Mob mob)
{
	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == mob._id) {
			_mobList[i]._positionX   = mob._positionX;
			_mobList[i]._positionY   = mob._positionY;
			_mobList[i]._health		 = mob._health;
			_mobList[i]._tier		 = mob._tier;
			_mobList[i]._enchantment = mob._enchantment;
			return;
		}
	}
	_mobList.push_back(mob);
}

void MobList::update(EntityMove mobMove)
{
	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == mobMove._id) {
			_mobList[i]._positionX = mobMove._positionX;
			_mobList[i]._positionY = mobMove._positionY;
			return;
		}
	}
}

size_t MobList::size()
{
	return _mobList.size();
}
Mob& MobList::operator[](size_t elementIndex)
{
	return _mobList[elementIndex];
}
