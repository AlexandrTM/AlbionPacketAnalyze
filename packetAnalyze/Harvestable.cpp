#include "pch.h"

float binToFloat(uint32_t x) {
	union {
		uint32_t  x;
		float_t  f;
	} temp;
	temp.x = x;
	return temp.f;
}

// ============================== Harvestable ==================================

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
	uint16_t _id = 0;
	uint8_t _type = 0;
	uint8_t _tier = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;
	uint8_t _charges = 0;
	uint8_t _enchantment = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawHarvestable);
	std::vector<uint8_t> _fragmentIDs{};
	size_t _fragmentOffset;

	for (size_t i = 0; i < _dataLayout.size(); i++) {
		_fragmentIDs.push_back(_dataLayout[i]._fragmentID);
	}

	//_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)0);
	//_id = (rawHarvestable[_fragmentOffset] << 8) | rawHarvestable[_fragmentOffset + 1];
	_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)5);
	_type = rawHarvestable[_fragmentOffset];
	_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)7);
	_tier = rawHarvestable[_fragmentOffset];
	_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)8);
	_positionX = binToFloat((rawHarvestable[_fragmentOffset    ] << 24) |
							(rawHarvestable[_fragmentOffset + 1] << 16) |
							(rawHarvestable[_fragmentOffset + 2] << 8 ) |
							 rawHarvestable[_fragmentOffset + 3]);
	_positionY = binToFloat((rawHarvestable[_fragmentOffset + 4] << 24) |
							(rawHarvestable[_fragmentOffset + 5] << 16) |
							(rawHarvestable[_fragmentOffset + 6] << 8 ) |
							 rawHarvestable[_fragmentOffset + 7]);
	_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)10);
	_charges = rawHarvestable[_fragmentOffset];
	_fragmentOffset = _dataLayout.findFragmentOffset(_fragmentIDs, (uint8_t)11);
	_enchantment = rawHarvestable[_fragmentOffset];
	//std::cout << _positionX << "\n";
	Harvestable(_id, _type, _tier, _positionX, _positionY, _charges, _enchantment).printInfo();
	//std::cout << (unsigned)this->_type;
	//this->printInfo();
	rawHarvestable.printCommandInOneString();
}
Harvestable::Harvestable()
{
	_id = 0;
	_type = 0;
	_tier = 0;
	_enchantment = 0;
	_positionX = 0;
	_positionY = 0;
	_charges = 0;
}
Harvestable::Harvestable(uint16_t id, uint8_t type, uint8_t tier,
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
	std::cout << (unsigned)_id << " " << (unsigned)_type << " " << 
				 (unsigned)_tier << " " << (unsigned)_enchantment << " "
				 << _positionX << " " << _positionY << " " << (unsigned)_charges << "\n";
}

// ============================== HarvestableList ==============================

HarvestableList::HarvestableList()
{
	_harvestableList = {};
}

HarvestableList::HarvestableList(NetworkCommand& rawHarvestableList)
{
	uint8_t _harvestablesNum = rawHarvestableList[20];

	ptrdiff_t _idOffset = _harvestableOffsets._idOffset;
	ptrdiff_t _typeOffset = _harvestableOffsets._typeOffset + _harvestablesNum * 2;
	ptrdiff_t _tierOffset = _harvestableOffsets._tierOffset + _harvestablesNum * 3;
	ptrdiff_t _positionXOffset = _harvestableOffsets._positionXOffset + _harvestablesNum * 4;
	ptrdiff_t _positionYOffset = _harvestableOffsets._positionYOffset + _harvestablesNum * 4;
	ptrdiff_t _chargesOffset = _harvestableOffsets._chargesOffset + _harvestablesNum * 12;

	uint16_t _id = 0;
	uint8_t _type = 0;
	uint8_t _tier = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;
	uint8_t _charges = 0;
	uint8_t _enchantment = 0;

	for (size_t i = 0; i < _harvestablesNum; i++) {
		_id = (rawHarvestableList[_idOffset +     i * 2] << 8) | 
			   rawHarvestableList[_idOffset + 1 + i * 2];
		_type = rawHarvestableList[_typeOffset + i];
		_tier = rawHarvestableList[_tierOffset + i];
		_positionX = binToFloat((rawHarvestableList[_positionXOffset     + i * 8] << 24) |
								(rawHarvestableList[_positionXOffset + 1 + i * 8] << 16) |
								(rawHarvestableList[_positionXOffset + 2 + i * 8] << 8 ) |
								 rawHarvestableList[_positionXOffset + 3 + i * 8]);
		_positionY = binToFloat((rawHarvestableList[_positionYOffset     + i * 8] << 24) |
								(rawHarvestableList[_positionYOffset + 1 + i * 8] << 16) |
								(rawHarvestableList[_positionYOffset + 2 + i * 8] << 8 ) |
								 rawHarvestableList[_positionYOffset + 3 + i * 8]);
		_charges = rawHarvestableList[_chargesOffset + i];

		_harvestableList.push_back(Harvestable(_id, _type, _tier, _positionX, _positionY, _charges, _enchantment));
	}
}     

void HarvestableList::printInfo()
{
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
void HarvestableList::push_back(Harvestable harvestable)
{
	_harvestableList.push_back(harvestable);
}

Harvestable& HarvestableList::operator[](size_t elementIndex)
{
	return _harvestableList[elementIndex];
}
HarvestableList& HarvestableList::operator+=(HarvestableList harvestableList)
{
	for (size_t i = 0; i < harvestableList.size(); i++) {
		_harvestableList.push_back(harvestableList[i]);
	}
	return *this;
}
