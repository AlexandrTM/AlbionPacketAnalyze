#include "pch.h"

Mob::Mob()
{
	_id					= 0;
	_uniqueValue        = 0;
	_uniqueName			= "";
	_category			= "";
	_typeCategory       = "";
	_harvestableType    = static_cast<int8_t>(HarvestableType::UNDEFINED);
	_positionX			= 0;
	_positionY			= 0;
	_tier		        = 0;
	_enchantment        = 0;
	_charges            = 0;
	_health		        = 0;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;
}
Mob::Mob(
	uint64_t id, uint16_t uniqueValue,
	std::string uniqueName, std::string category, std::string typeCategory,
	uint32_t health,
	uint8_t  tier, uint8_t enchantment, uint8_t charges,
	float_t  positionX, float_t  positionY
)
{
	_id			        = id;
	_uniqueValue        = uniqueValue;
	_uniqueName         = uniqueName;
	_category			= category;
	_typeCategory       = typeCategory;
	_harvestableType    = static_cast<int8_t>(HarvestableType::UNDEFINED);
	_tier		        = tier;
	_enchantment        = enchantment;
	_charges            = charges;
	_positionX          = positionX;
	_positionY          = positionY;
	_health		        = health;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;
}

Mob::Mob(NetworkCommand& rawMob)
{
	_id					= 0;
	_uniqueValue        = 0;
	_uniqueName			= "";
	_category			= "";
	_typeCategory       = "";
	_harvestableType    = static_cast<int8_t>(HarvestableType::UNDEFINED);
	_positionX			= 0;
	_positionY			= 0;
	_tier		        = 0;
	_enchantment        = 0;
	_charges            = 0;
	_health		        = 0;
	_moveSpeed          = 0;
	_maxEnergy          = 0;
	_energyRegeneration = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawMob);

    _id = net::read_integer(rawMob, dataLayout.findFragment(0));

	DataFragment& uniqueValueFragment     = dataLayout.findFragment(1);
	// fragment 7 current position fragment 8 new position
	DataFragment& positionFragment = dataLayout.findFragment(8);
	DataFragment& healthFragment   = dataLayout.findFragment(14);
	DataFragment& tierFragment     = dataLayout.findFragment(21);

	if (uniqueValueFragment._dataType._size == 2) {
		_uniqueValue = net::read_uint16(rawMob, dataLayout.findFragment(1)._offset) - 14;
	}
	else if (uniqueValueFragment._dataType._size == 1) {
		_uniqueValue = net::read_uint8 (rawMob, dataLayout.findFragment(1)._offset) - 14;
	}
	if (positionFragment._offset != std::numeric_limits<ptrdiff_t>::min()) {
		_positionX      = net::read_float32(rawMob, dataLayout.findFragment(8)._offset);
		_positionY      = net::read_float32(rawMob, dataLayout.findFragment(8)._offset + 4);
	}
	_health             = net::read_float32(rawMob, healthFragment._offset);
	_moveSpeed          = net::read_float32(rawMob, dataLayout.findFragment(11)._offset);
	_maxEnergy          = net::read_float32(rawMob, dataLayout.findFragment(18)._offset);
	_energyRegeneration = net::read_float32(rawMob, dataLayout.findFragment(21)._offset);

	net::getMobData(net::mobsData, _uniqueValue, _tier, _uniqueName, _category, _typeCategory);
	_harvestableType = findHarvestableType();
	//std::cout << "typeCategory: " << _typeCategory << "\n";
	//this->printInfo();
	//dataLayout.printInfo(rawMob, true);
	//dataLayout.printInfo(rawMob, 11, 21, true);

	if 
		((_uniqueName.find("STANDARD")  != std::string::npos)) { _enchantment = 1; }
	else if 
		((_uniqueName.find("UNCOMMON")  != std::string::npos)) { _enchantment = 2; }
	else if 
		((_uniqueName.find("RARE")      != std::string::npos)) { _enchantment = 3; }
	else if 
		((_uniqueName.find("LEGENDARY") != std::string::npos)) { _enchantment = 4; }
}

void Mob::printInfo() const
{
	std::cout <<
		"uniqueName: "         << _uniqueName						<< " " <<
		"health: "             << std::setw(5) << (unsigned)_health << " " <<
		"moveSpeed: "          << _moveSpeed                        << " " <<
		"energyRegeneration: " << _energyRegeneration               << " " <<
		"maxEnergy: "          << _maxEnergy                        << " " <<
		"category: "           << _category						    << " " <<
		"harvestableType: "    << (signed)_harvestableType			<< " " <<
		"tier: "			   << (unsigned)_tier					<< " " <<
		/*"id: " << std::setw(7) << (unsigned)_id << " " <<*/
		//"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " <<
		//"charges: "		<< std::setw(2) << (unsigned)_charges	  << " " <<
		/*"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " <<
		"x: " << std::setw(8) << _positionX << " " <<
		"y: " << std::setw(8) << _positionY << */"\n";
}

int8_t Mob::findHarvestableType() const
{
	if (_category == "hidemob") {
		return static_cast<int8_t>(HarvestableType::HIDE);
	}

	for (const auto& [keyword, type] : harvestableMappings) {
		if (_uniqueName.find(keyword) != std::string::npos) {
			return static_cast<int8_t>(type); // Return the corresponding HarvestableType
		}
	}

	return static_cast<int8_t>(HarvestableType::UNDEFINED);
}
bool Mob::isHarvestable() const
{
	if (_harvestableType != static_cast<int8_t>(HarvestableType::UNDEFINED)) {
		return true;
	}

	return false;
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
