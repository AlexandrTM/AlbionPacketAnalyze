#include "pch.h"

Mob::Mob()
{
	_id			 = 0;
	_category    = 0;
	_positionX	 = 0;
	_positionY	 = 0;
	_health		 = 0;
	_tier		 = 0;
	_enchantment = 0;
}

Mob::Mob(NetworkCommand& rawMob)
{
	_id			 = 0;
	_category    = 0;
	_positionX	 = 0;
	_positionY	 = 0;
	_health		 = 0;
	_tier		 = 0;
	_enchantment = 0;

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

	_category    = net::read_uint8  (rawMob, dataLayout.findFragment(2)._offset);
	_health      = net::read_float32(rawMob, healthFragment._offset);
	_positionX   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
	_positionY   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);
	_tier        = net::read_uint8  (rawMob, dataLayout.findFragment(30)._offset);

	/*std::cout << 
		// current position
		"x1: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset) << " " <<
		"x2: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset + 4) << "\n";
		// target position
		_positionX   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
		_positionY   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);*/

	//std::cout << 
	//	"mob type:    " << net::read_uint16 (rawMob, dataLayout.findFragment(1 )._offset) << " " << "\n" <<
	//	//"rotation angle ? :      " << net::read_float32(rawMob, dataLayout.findFragment(10)._offset) << " " << "\n" <<
	//	"some11:      " << net::read_float32(rawMob, dataLayout.findFragment(11)._offset) << " " << "\n" <<
	//	"some12:      " << net::read_float32(rawMob, dataLayout.findFragment(12)._offset) << " " << "\n" <<
	//	//"health ? :      " << net::read_float32(rawMob, dataLayout.findFragment(13)._offset) << " " << "\n" <<
	//	//"health ? :      " << net::read_float32(rawMob, dataLayout.findFragment(14)._offset) << " " << "\n" <<
	//	"some15:      " << net::read_float32(rawMob, dataLayout.findFragment(15)._offset) << " " << "\n" <<
	//	"some16:      " << net::read_float32(rawMob, dataLayout.findFragment(16)._offset) << " " << "\n" <<
	//	"some17:      " << net::read_uint32 (rawMob, dataLayout.findFragment(17)._offset) << " " << "\n" <<
	//	"some18:      " << net::read_float32(rawMob, dataLayout.findFragment(18)._offset) << " " << "\n" <<
	//	"some19:      " << net::read_float32(rawMob, dataLayout.findFragment(19)._offset) << " " << "\n";

	std::cout <<
		"mob type:    " << net::read_uint16(rawMob, dataLayout.findFragment(1)._offset) << " " 
		"mob type0:   " << net::read_uint8(rawMob, dataLayout.findFragment(1)._offset) << " "
		"mob type1:   " << net::read_uint8(rawMob, dataLayout.findFragment(1)._offset + 1) << " "
		<< "\n";


	//std::cout << 
	//	"id:          "	<< (unsigned)_id		  << " " << "\n" <<
	//	"tier:        "	<< (unsigned)_tier		  << " " << "\n" <<
	//	"health:      " << (unsigned)_health	  << " " << "\n" <<
	//	"enchantment: " << (unsigned)_enchantment << " " << "\n" <<
	//	"x:           "	<< _positionX			  << " " << "\n" <<
	//	"y:           "	<< _positionY			  << " " << "\n" <<
	//	"\n";

	//dataLayout.printInfo(rawMob);
}

Mob::Mob(uint32_t id, uint8_t category, 
	uint32_t health, uint8_t tier, uint8_t enchantment, 
	float_t positionX, float_t positionY)
{
	_id			 = id;
	_category    = category;
	_health		 = health;
	_tier		 = tier;
	_enchantment = enchantment;
	_positionX   = positionX;
	_positionY   = positionX;
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
			_mobList[i]._health		 = mob._health;
			_mobList[i]._tier		 = mob._tier;
			_mobList[i]._enchantment = mob._enchantment;
			_mobList[i]._positionX   = mob._positionX;
			_mobList[i]._positionY   = mob._positionY;
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

void MobList::update(HealthUpdate healthUpdate)
{
	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == healthUpdate._id) {
			if (healthUpdate._health == 0) {
				_mobList.erase(_mobList.begin() + i);
			}
			else {
				_mobList[i]._health = healthUpdate._health;
			}
			return;
		}
	}
}

void MobList::mobChangeState(NetworkCommand& mobChangeState)
{
	uint32_t id = 0;
	uint8_t enchantment = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(mobChangeState);

	DataFragment idFragment     = dataLayout.findFragment(0);
	uint8_t idSize = idFragment._dataType._size;
	if 
		(idSize == 1) { id = net::read_uint8 (mobChangeState, idFragment._offset); }
	else if 
		(idSize == 2) { id = net::read_uint16(mobChangeState, idFragment._offset); }
	else if 
		(idSize == 4) { id = net::read_uint32(mobChangeState, idFragment._offset); }
	else if 
		(idSize == 8) { id = net::read_uint32(mobChangeState, idFragment._offset + 4); }

	enchantment = net::read_uint8(mobChangeState, dataLayout.findFragment(1)._offset);

	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == id) {
			_mobList[i]._enchantment = enchantment;
			return;
		}
	}
	// _mobList.push_back(Mob::Mob(id, 0, 0, enchantment, 0, 0)); if isMobComplete
}

size_t MobList::size()
{
	return _mobList.size();
}
Mob& MobList::operator[](size_t elementIndex)
{
	return _mobList[elementIndex];
}
