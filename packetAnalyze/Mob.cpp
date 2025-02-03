#include "pch.h"

Mob::Mob()
{
	_id					= 0;
	_category			= 0;
	_typeID				= 0;
	_textType			= "";
	_positionX			= 0;
	_positionY			= 0;
	_health		        = 0;
	_tier		        = 0;
	_enchantment        = 0;
	_charges            = 0;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;
}
Mob::Mob(uint64_t id, uint8_t category, uint8_t type, std::string textType,
	uint32_t health,
	uint8_t tier, uint8_t enchantment, uint8_t charges,
	float_t positionX, float_t positionY)
{
	_id			        = id;
	_category           = category;
	_typeID             = type;
	_textType           = textType;
	_health		        = health;
	_tier		        = tier;
	_enchantment        = enchantment;
	_charges            = charges;
	_positionX          = positionX;
	_positionY          = positionY;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;
}

Mob::Mob(NetworkCommand& rawMob)
{
	_id					= 0;
	_category			= 0;
	_typeID				= 0;
	_textType			= "";
	_positionX			= 0;
	_positionY			= 0;
	_health		        = 0;
	_tier		        = 0;
	_enchantment        = 0;
	_charges            = 0;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawMob);

    _id = net::read_integer(rawMob, dataLayout.findFragment(0));

	DataFragment& typeFragment     = dataLayout.findFragment(1);
	// fragment 7 current position fragment 8 new position
	DataFragment& positionFragment = dataLayout.findFragment(8);
	DataFragment& healthFragment   = dataLayout.findFragment(14);
	DataFragment& tierFragment     = dataLayout.findFragment(21);

	// not tier of charges actually some kind of mob subtype
	//float_t tierAndCharges = net::read_float32(rawMob, dataLayout.findFragment(11)._offset);

	_category           = net::read_uint8  (rawMob, dataLayout.findFragment(1)._offset);
	if (typeFragment._offset != std::numeric_limits<ptrdiff_t>::min()) {
		_typeID         = net::read_uint8  (rawMob, dataLayout.findFragment(1)._offset + 1);
	}
	_health             = net::read_float32(rawMob, healthFragment._offset);
	if (positionFragment._offset != std::numeric_limits<ptrdiff_t>::min()) {
		_positionX      = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
		_positionY      = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);
	}
	_moveSpeed          = net::read_float32(rawMob, dataLayout.findFragment(11)._offset);
	_maxEnergy          = net::read_float32(rawMob, dataLayout.findFragment(18)._offset);
	_energyRegeneration = net::read_float32(rawMob, dataLayout.findFragment(21)._offset);

	MobDescription mobDescription = getMobDescription(_category, _typeID);
	_tier = mobDescription._tier;
	_textType = mobDescription._textType;

	//if (_health > 20) {
	if (_category == mobCategory::resource || _category == mobCategory::resourceElemental) {
		dataLayout.printInfo(rawMob, true);
		this->printInfo();
		net::extractMatchingMobs(net::mobsData, _moveSpeed, _maxEnergy, _energyRegeneration);
	}
	//}

	if ((_category == mobCategory::resource || _category == mobCategory::resourceElemental) &&
		_health > 20) {

		//dataLayout.printInfo(rawMob, 11, 21, true);

		/*std::cout <<
			"some2:      " << net::read_uint8 (rawMob, dataLayout.findFragment(2)._offset) << " " << "\n" <<
			"some3:      " << net::read_float32(rawMob, dataLayout.findFragment(3)._offset) << " " << "\n" <<
			"some4:      " << net::read_float32(rawMob, dataLayout.findFragment(4)._offset) << " " << "\n" <<
			"some5:      " << net::read_uint32(rawMob, dataLayout.findFragment(5)._offset) << " " << "\n" <<
			"some6:      " << net::read_float32(rawMob, dataLayout.findFragment(6)._offset) << " " << "\n" <<
			"some9:      " << net::read_float32(rawMob, dataLayout.findFragment(9)._offset) << " " << "\n"
			"rotation angle ? (10): " << net::read_float32(rawMob, dataLayout.findFragment(10)._offset) << " " << "\n" <<
			"some12:      " << net::read_float32(rawMob, dataLayout.findFragment(12)._offset) << " " << "\n" <<
			"health ? (13): " << net::read_float32(rawMob, dataLayout.findFragment(13)._offset) << " " << "\n" <<
			"some15:      " << net::read_float32(rawMob, dataLayout.findFragment(15)._offset) << " " << "\n" <<
			"some16:      " << net::read_float32(rawMob, dataLayout.findFragment(16)._offset) << " " << "\n" <<
			"some17:      " << net::read_uint32 (rawMob, dataLayout.findFragment(17)._offset) << " " << "\n" <<|
			"some19:      "<< net::read_float32(rawMob, dataLayout.findFragment(19)._offset) << " " << "\n";"*/
	}

	if 
		((_textType.find("UNCOMMON")  != std::string::npos)) { _enchantment = 1; }
	else if 
		((_textType.find("RARE")      != std::string::npos)) { _enchantment = 2; }
	else if 
		((_textType.find("EPIC")      != std::string::npos)) { _enchantment = 3; }
	else if 
		((_textType.find("LEGENDARY") != std::string::npos)) { _enchantment = 4; }

	if (_textType == "MOB_NOT_FOUND") {
		//DataFragment& some12Fragment = dataLayout.findFragment(12);

		//float_t some12 = net::read_float32(rawMob, some12Fragment._offset);

		//if (some12 != 0) {
		//	std::cout <<
		//		// ???
		//		"some12: " << some12 << " " << "\n";
		//	//rawMob.printCommandInOneString();
		//}

		//std::cout << "some11: " << net::read_float32(rawMob, dataLayout.findFragment(11)._offset) << "\n";
		if (_category == mobCategory::resource || _category == mobCategory::resourceElemental) {
			//this->printInfo();
		}
	}
}

void Mob::printInfo() const
{
	std::cout << 
		"MobDescription(" << 
		std::left << std::setw(3) << (unsigned)_typeID << 
		", " << std::setw(3) << (unsigned)_category << 
		", " << (unsigned)_tier << 
		", \"" << _textType << "\"), ";
	std::cout <<
		"// health: "          << std::setw(5) << (unsigned)_health << " " <<
		"moveSpeed: "          << _moveSpeed << " " <<
		"energyRegeneration: " << _energyRegeneration << " " <<
		"maxEnergy: "          << _maxEnergy <<
		/*"type: " << std::setw(3) << (unsigned)_typeID << " " <<
		"category: " << std::setw(2) << (unsigned)_category << " " <<
		"id: " << std::setw(7) << (unsigned)_id << " " <<
		//"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " <<
		//"charges: "		<< std::setw(2) << (unsigned)_charges	  << " " <<
		"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " <<
		"x: " << std::setw(8) << _positionX << " " <<
		"y: " << std::setw(8) << _positionY <<*/ "\n";
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

void MobList::update(HealthUpdateHandler healthUpdate)
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
	uint64_t id = 0;
	uint8_t enchantment = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(mobChangeState);

    id = net::read_integer(mobChangeState, dataLayout.findFragment(0));

	enchantment = net::read_uint8(mobChangeState, dataLayout.findFragment(1)._offset);

	for (size_t i = 0; i < _mobList.size(); i++) {
		if (_mobList[i]._id == id) {
			_mobList[i]._enchantment = enchantment;
			return;
		}
	}
	// _mobList.push_back(Mob::Mob(id, 0, 0, enchantment, 0, 0)); if isMobComplete
}

size_t MobList::size() const
{
	return _mobList.size();
}
Mob& MobList::operator[](size_t elementIndex)
{
	return _mobList[elementIndex];
}

MobFilter::MobFilter(std::vector<uint8_t> trackingTiers,
	std::vector<uint8_t> trackingEnchantments)
{
	_trackingTiers = trackingTiers;
	_trackingEnchantments = trackingEnchantments;
}
MobFilter::MobFilter()
{
	_trackingTiers = {};
	_trackingEnchantments = {};
}
size_t MobListFilter::size() const
{
	return _mobListFilter.size();
}

MobFilter& MobListFilter::operator[](size_t elementIndex)
{
	return _mobListFilter[elementIndex];
}
