#include "pch.h"

// *****************************************************************************
// ============================== HarvestableFilter ============================
// *****************************************************************************

HarvestableFilter::HarvestableFilter(std::vector<uint8_t> trackingTiers,
	std::vector<uint8_t> trackingEnchantments,
	std::vector<uint8_t> trackingCharges)
{
	_trackingTiers = trackingTiers;
	_trackingEnchantments = trackingEnchantments;
	_trackingCharges = trackingCharges;
}
HarvestableFilter::HarvestableFilter()
{
	_trackingTiers = {};
	_trackingEnchantments = {};
	_trackingCharges = {};
}
size_t HarvestableListFilter::size()
{
	return _harvestableListFilter.size();
}
HarvestableFilter& HarvestableListFilter::operator[](size_t elementIndex)
{
	return _harvestableListFilter[elementIndex];
}

// *****************************************************************************
// ============================== Harvestable ==================================
// *****************************************************************************


struct HarvestableOffsets
{
	ptrdiff_t _idOffset = 22;
	ptrdiff_t _typeOffset = 28;
	ptrdiff_t _tierOffset = 34;
	ptrdiff_t _positionXOffset = 39;
	ptrdiff_t _positionYOffset = 43;
	ptrdiff_t _chargesOffset = 45;
} _harvestableOffsets;

Harvestable::Harvestable(NetworkCommand& rawHarvestable)
{
	_id			 = 0;
	_type        = 0;
	_tier        = 0;
	_positionX   = 0;
	_positionY   = 0;
	_charges     = 0;
	_enchantment = 0;
	
	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawHarvestable);

	uint8_t idSize = dataLayout.findFragment(0)._dataType._size;
	ptrdiff_t idOffset = dataLayout.findFragment(0)._offset;
	if 
		(idSize == 4) { _id = net::read_uint32(rawHarvestable, idOffset); }
	else if 
		(idSize == 2) { _id = net::read_uint16(rawHarvestable, idOffset); }
	else if 
		(idSize == 1) { _id = net::read_uint8 (rawHarvestable, idOffset); }
	else if 
		(idSize == 8) { _id = net::read_uint64(rawHarvestable, idOffset); }

	_type        = net::read_uint8  (rawHarvestable, dataLayout.findFragment(5) ._offset);
	_tier        = net::read_uint8  (rawHarvestable, dataLayout.findFragment(7) ._offset);
	_positionX   = net::read_float32(rawHarvestable, dataLayout.findFragment(8) ._offset);
	_positionY   = net::read_float32(rawHarvestable, dataLayout.findFragment(8) ._offset + 4);
	_charges     = net::read_uint8  (rawHarvestable, dataLayout.findFragment(10)._offset);
	_enchantment = net::read_uint8  (rawHarvestable, dataLayout.findFragment(11)._offset);

	//this->printInfo();
}
static uint64_t previousHarvestTime = 0;
static uint32_t previousHarvestID = 0;
void Harvestable::harvestStart(NetworkCommand& rawHarvestable)
{
	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawHarvestable);

	uint64_t id = 0;
	uint64_t harvestTime = 0;

	uint8_t idSize = dataLayout.findFragment(0)._dataType._size;
	ptrdiff_t idOffset = dataLayout.findFragment(0)._offset;
	if 
		(idSize == 4) { id = net::read_uint32(rawHarvestable, idOffset); }
	else if 
		(idSize == 2) { id = net::read_uint16(rawHarvestable, idOffset); }
	else if 
		(idSize == 1) { id = net::read_uint8 (rawHarvestable, idOffset); }
	else if 
		(idSize == 8) { id = net::read_uint64(rawHarvestable, idOffset); }

	harvestTime = net::read_uint64(rawHarvestable, dataLayout.findFragment(1)._offset);

	if (id == previousHarvestID) {
		std::cout << "harvest time: " << (float_t)(harvestTime - previousHarvestTime) / 1e7 << "\n";
	}

	previousHarvestID = id;
	previousHarvestTime = harvestTime;
}
void Harvestable::harvestFinished(NetworkCommand& rawHarvestable)
{
	DataLayout dataLayout{};
	dataLayout.findDataLayout(rawHarvestable);

	uint64_t id = 0;
	uint64_t harvestStartTime = 0;
	uint64_t harvestEndTime = 0;

	uint8_t idSize = dataLayout.findFragment(0)._dataType._size;
	ptrdiff_t idOffset = dataLayout.findFragment(0)._offset;
	if 
		(idSize == 4) { id = net::read_uint32(rawHarvestable, idOffset); }
	else if 
		(idSize == 2) { id = net::read_uint16(rawHarvestable, idOffset); }
	else if 
		(idSize == 1) { id = net::read_uint8 (rawHarvestable, idOffset); }
	else if 
		(idSize == 8) { id = net::read_uint64(rawHarvestable, idOffset); }

	harvestStartTime = net::read_uint64(rawHarvestable, dataLayout.findFragment(1)._offset);
	harvestEndTime = net::read_uint64(rawHarvestable, dataLayout.findFragment(2)._offset);

	std::cout << "harvest time: " << (float_t)(harvestEndTime - harvestStartTime) / 1e7 << "\n";
}
Harvestable::Harvestable()
{
	_id = 0;
	_type = 0;
	_tier = 0;
	_positionX = 0;
	_positionY = 0;
	_charges = 0;
	_enchantment = 0;
}
Harvestable::Harvestable(uint64_t id, uint8_t type, uint8_t tier,
	float_t positionX, float_t positionY,
	uint8_t charges, uint8_t enchantment)
{
	_id = id;
	_type = type;
	_tier = tier;
	_positionX = positionX;
	_positionY = positionY;
	_charges = charges;
	_enchantment = enchantment;
}

void Harvestable::printInfo()
{
	std::cout << 
		"id: "			<< std::setw(7) << (unsigned)_id		  << " " <<
		"type: "		<< std::setw(2) << (unsigned)_type		  << " " <<
		std::setw(7)    << getHarvestableTextType(_type)		  << " " <<
		"tier: "		<< std::setw(1) << (unsigned)_tier		  << " " <<
		"enchantment: " << std::setw(1) << (unsigned)_enchantment << " " <<
		"charges: "     << std::setw(2) << (unsigned)_charges	  << " " <<
		"x: "			<< std::setw(6) << _positionX			  << " " <<
		"y: "			<< std::setw(6) << _positionY			  << "\n";
}
std::string Harvestable::getHarvestableTextType(uint8_t type)
{
	std::string textType = "\"UNDEFINED\"";
	if (type >= resourceType::WOOD and
		type <= (resourceType::ROCK - 1)) {
		textType = "\"WOOD\"";
	}
	if (type >= resourceType::ROCK and
		type <= (resourceType::FIBER - 1)) {
		textType = "\"ROCK\"";
	}
	if (type >= resourceType::FIBER and
		type <= (resourceType::HIDE - 1)) {
		textType = "\"FIBER\"";
	}
	if (type >= resourceType::HIDE and
		type <= (resourceType::ORE - 1)) {
		textType = "\"HIDE\"";
	}
	if (type >= resourceType::ORE and
		type <= (resourceType::OTHER - 1)) {
		textType = "\"ORE\"";
	}
	return textType;
}


// *****************************************************************************
// ============================== HarvestableList ==============================
// *****************************************************************************


HarvestableList::HarvestableList()
{
	_harvestableList = {};
}
HarvestableList::HarvestableList(NetworkCommand& rawHarvestableList)
{
	uint8_t harvestablesNum = rawHarvestableList[20];

	ptrdiff_t idOffset		  =	_harvestableOffsets._idOffset;
	ptrdiff_t typeOffset      =	_harvestableOffsets._typeOffset		 + harvestablesNum * 2;
	ptrdiff_t tierOffset      =	_harvestableOffsets._tierOffset      + harvestablesNum * 3;
	ptrdiff_t positionXOffset = _harvestableOffsets._positionXOffset + harvestablesNum * 4;
	ptrdiff_t positionYOffset = _harvestableOffsets._positionYOffset + harvestablesNum * 4;
	ptrdiff_t chargesOffset   = _harvestableOffsets._chargesOffset   + harvestablesNum * 12;

	uint64_t id			 = 0;
	uint8_t  type		 = 0;
	uint8_t  tier		 = 0;
	float_t  positionX   = 0;
	float_t  positionY   = 0;
	uint8_t  charges     = 0;
	uint8_t  enchantment = 0;

	for (size_t i = 0; i < harvestablesNum; i++) {
		id		  =	net::read_uint16 (rawHarvestableList, idOffset		  + i * 2);
		type	  =	net::read_uint8  (rawHarvestableList, typeOffset	  + i);
		tier	  =	net::read_uint8  (rawHarvestableList, tierOffset	  + i);
		positionX = net::read_float32(rawHarvestableList, positionXOffset + i * 8);
		positionY = net::read_float32(rawHarvestableList, positionYOffset + i * 8);
		charges   = net::read_uint8  (rawHarvestableList, chargesOffset   + i);

		_harvestableList.push_back(
			Harvestable(id, type, tier, positionX, positionY, charges, enchantment));
	}

	//this->printInfo();
}

void HarvestableList::updateState(NetworkCommand& updateState)
{
	uint64_t id			= 0;
	uint8_t charges		= 0;
	uint8_t enchantment = 0;

	DataLayout dataLayout{};
	dataLayout.findDataLayout(updateState);

	uint8_t idSize     = dataLayout.findFragment(0)._dataType._size;
	ptrdiff_t idOffset = dataLayout.findFragment(0)._offset;
	if 
		(idSize == 4) { id = net::read_uint32(updateState, idOffset); }
	else if 
		(idSize == 2) { id = net::read_uint16(updateState, idOffset); }
	else if
		(idSize == 8) { id = net::read_uint64(updateState, idOffset); }
	else if 
		(idSize == 1) { id = net::read_uint8 (updateState, idOffset); }

	charges		= net::read_uint8(updateState, dataLayout.findFragment(1)._offset);
	enchantment = net::read_uint8(updateState, dataLayout.findFragment(2)._offset);

	for (size_t i = 0; i < _harvestableList.size(); i++) {
		if (_harvestableList[i]._id == id) {
			_harvestableList[i]._charges     = charges;
			_harvestableList[i]._enchantment = enchantment;
			break;
		}
	}
}

void HarvestableList::printInfo()
{
	std::cout << "Num of harvestables: " << _harvestableList.size() << "\n";
	for (size_t i = 0; i < _harvestableList.size(); i++) {
		_harvestableList[i].printInfo();
	}
}

void HarvestableList::clear()
{
	_harvestableList.clear();
}
size_t HarvestableList::size()
{
	return _harvestableList.size();
}
void HarvestableList::update(Harvestable harvestable)
{
	for (size_t i = 0; i < _harvestableList.size(); i++) {
		//std::cout << i << " " << _harvestableList.size() << "\n";
		if (_harvestableList[i]._id == harvestable._id) {
			_harvestableList[i]._charges     = harvestable._charges;
			_harvestableList[i]._enchantment = harvestable._enchantment;
			return;
		}
	}
	_harvestableList.push_back(harvestable);
}
void HarvestableList::update(HarvestableList harvestableList)
{
	for (size_t i = 0; i < harvestableList.size(); i++) {
		this->update(harvestableList[i]);
	}
}
HarvestableList::iterator HarvestableList::begin() {
	return _harvestableList.begin();
}

HarvestableList::iterator HarvestableList::end() {
	return _harvestableList.end();
}
Harvestable& HarvestableList::operator[](size_t elementIndex)
{
	return _harvestableList[elementIndex];
}
HarvestableList& HarvestableList::operator+=(HarvestableList harvestableList)
{
	this->update(harvestableList);
	return *this;
}
