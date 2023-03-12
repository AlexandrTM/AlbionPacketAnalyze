#include "pch.h"

float binToFloat(uint32_t x) {
	union {
		uint32_t  x;
		float_t  f;
	} temp;
	temp.x = x;
	return temp.f;
}

Harvestable::Harvestable()
{
	_id = 0;
	_type = 0;
	_tier = 0;
	_positionX = 0;
	_positionY = 0;
	_charges = 0;
}

Harvestable::Harvestable(uint16_t id, uint8_t type, uint8_t tier, 
						 float_t positionX, float_t positionY, uint8_t charges)
{
	_id = id;
	_type = type;
	_tier = tier;
	_positionX = positionX;
	_positionY = positionY;
	_charges = charges;
}

void Harvestable::printInfo()
{
	std::cout << (unsigned)_id << " " << (unsigned)_type << " " << (unsigned)_tier << " "
		<< binToFloat(_positionX) << " " << binToFloat(_positionY) << " " << (unsigned)_charges << "\n";
}

// ================== HarvestableList ==================

HarvestableList::HarvestableList()
{
	_harvestableList = {};
}

HarvestableList::HarvestableList(NetworkCommand& harvestableRawList)
{
	uint8_t _harvestablesNum = harvestableRawList[20];
	ptrdiff_t _idOffset = 22;
	ptrdiff_t _typeOffset = 22 + 6 + _harvestablesNum * 2;
	ptrdiff_t _tierOffset = 22 + 12 + _harvestablesNum * 3;
	ptrdiff_t _positionXOffset = 22 + 17 + _harvestablesNum * 4;
	ptrdiff_t _positionYOffset = 22 + 21 + _harvestablesNum * 4;
	ptrdiff_t _chargesOffset = 22 + 23 + _harvestablesNum * 12;

	for (size_t i = 0; i < _harvestablesNum; i++) {
		uint16_t _id = 0; 
		uint8_t _type = 0; 
		uint8_t _tier = 0; 
		float_t _positionX = 0; 
		float_t _positionY = 0; 
		uint8_t _charges = 0; 

		_id = (harvestableRawList[_idOffset +     i * 2] << 8) + 
			   harvestableRawList[_idOffset + 1 + i * 2];
		_type = harvestableRawList[_typeOffset + i];
		_tier = harvestableRawList[_tierOffset + i];
		_positionX = (harvestableRawList[_positionXOffset     + i * 8] << 24) + 
					 (harvestableRawList[_positionXOffset + 1 + i * 8] << 16) +
					 (harvestableRawList[_positionXOffset + 2 + i * 8] << 8 ) +  
					  harvestableRawList[_positionXOffset + 3 + i * 8];
		_positionY = (harvestableRawList[_positionYOffset     + i * 8] << 24) + 
					 (harvestableRawList[_positionYOffset + 1 + i * 8] << 16) +
					 (harvestableRawList[_positionYOffset + 2 + i * 8] << 8 ) +  
					  harvestableRawList[_positionYOffset + 3 + i * 8];
		_charges = harvestableRawList[_chargesOffset + i];

		*this += Harvestable(_id, _type, _tier, _positionX, _positionY, _charges);
		_harvestableList[i].printInfo();
	}
	harvestableRawList.printCommandInOneString();
}

void HarvestableList::clear()
{
	_harvestableList.clear();
}
size_t HarvestableList::size()
{
	return _harvestableList.size();
}

Harvestable& HarvestableList::operator[](size_t elementIndex)
{
	return _harvestableList[elementIndex];
}
HarvestableList& HarvestableList::operator+=(Harvestable harvestable)
{
	_harvestableList.push_back(harvestable);
	return *this;
}
