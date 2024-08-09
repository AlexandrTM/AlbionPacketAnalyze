#include "pch.h"

Mob::Mob()
{
	_id			 = 0;
	_category    = 0;
	_type	     = 0;
	_positionX	 = 0;
	_positionY	 = 0;
	_health		 = 0;
	_tier		 = 0;
	_enchantment = 0;
	_charges     = 0;
}

Mob::Mob(NetworkCommand& rawMob)
{
	_id			 = 0;
	_category    = 0;
	_type	     = 0;
	_positionX	 = 0;
	_positionY	 = 0;
	_health		 = 0;
	_tier		 = 0;
	_enchantment = 0;
	_charges     = 0;

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

	float_t tierAndCharges = net::read_float32(rawMob, dataLayout.findFragment(11)._offset);
	float_t preTier = 0;

	_category    = net::read_uint8  (rawMob, dataLayout.findFragment(1)._offset);
	_type		 = net::read_uint8  (rawMob, dataLayout.findFragment(1)._offset + 1);
	_health      = net::read_float32(rawMob, healthFragment._offset);
	_positionX   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
	_positionY   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);
	_tier        = std::floor(tierAndCharges);
	_charges     = std::modf(tierAndCharges, &preTier) * 10;
	
	/*std::cout << 
		// current position
		"x1: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset) << " " <<
		"x2: " << std::setw(6) << net::read_float32(rawMob, dataLayout.findFragment(7)._offset + 4) << "\n";
		// target position
		_positionX   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
		_positionY   = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);*/

		//	"mob type:    " << net::read_uint16 (rawMob, dataLayout.findFragment(1 )._offset) << " " << "\n" <<
		//	//"rotation angle ? :      " << net::read_float32(rawMob, dataLayout.findFragment(10)._offset) << " " << "\n" <<
	//	//"health ? :      " << net::read_float32(rawMob, dataLayout.findFragment(13)._offset) << " " << "\n" <<
	//	//"health ? :      " << net::read_float32(rawMob, dataLayout.findFragment(14)._offset) << " " << "\n" <<
	//	"some15:      " << net::read_float32(rawMob, dataLayout.findFragment(15)._offset) << " " << "\n" <<
	//	"some16:      " << net::read_float32(rawMob, dataLayout.findFragment(16)._offset) << " " << "\n" <<
	//	"some17:      " << net::read_uint32 (rawMob, dataLayout.findFragment(17)._offset) << " " << "\n" <<
	//	"some18:      " << net::read_float32(rawMob, dataLayout.findFragment(18)._offset) << " " << "\n" <<
	//	"some19:      " << net::read_float32(rawMob, dataLayout.findFragment(19)._offset) << " " << "\n";
	

	if (_category != mobCategory::basicMob and
        _category != mobCategory::magicMob and
        _category != mobCategory::guard    and
        _category != mobCategory::mobBlackZone) {
		float_t some12 = net::read_float32(rawMob, dataLayout.findFragment(12)._offset);

		if (some12 != 0) {
			std::cout <<
				// ???
				"some12: " << some12 << " " << "\n";
		}
		std::cout <<
			"some11: " << net::read_float32(rawMob, dataLayout.findFragment(11)._offset) << "\n" <<
			"some17: " << net::read_uint32 (rawMob, dataLayout.findFragment(17)._offset) << " " << "\n";
		this->printInfo();
		dataLayout.printInfo(rawMob);
	}
}

Mob::Mob(uint32_t id, uint8_t category, uint8_t type,
	uint32_t health, 
	uint8_t tier, uint8_t enchantment, uint8_t charges,
	float_t positionX, float_t positionY)
{
	_id			 = id;
	_category    = category;
	_type		 = type;
	_health		 = health;
	_tier		 = tier;
	_enchantment = enchantment;
	_charges     = charges;
	_positionX   = positionX;
	_positionY   = positionY;
}

void Mob::printInfo()
{
	std::cout << 
		"id: "			<< std::setw(7) << (unsigned)_id		  << " " <<
		"category: "	<< std::setw(3) << (unsigned)_category    << " " <<
		"type: "	    << std::setw(3) << (unsigned)_type        << " " <<
		"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " <<
		"charges: "		<< std::setw(2) << (unsigned)_charges	  << " " <<
		"health: "      << std::setw(7) << (unsigned)_health	  << " " <<
		"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " <<
		"x: "			<< std::setw(8) << _positionX			  << " " <<
		"y: "			<< std::setw(8) << _positionY			  << "\n";
}

MobList::MobList()
{
	_mobList = {};
}

void MobList::newMob(Mob mob)
{
	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == mob._id) {
			_mobList[i] = mob;
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
