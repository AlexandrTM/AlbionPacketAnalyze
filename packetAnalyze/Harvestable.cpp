#include "pch.h"


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
	uint32_t _id = 0;
	uint8_t _type = 0;
	uint8_t _tier = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;
	uint8_t _charges = 0;
	uint8_t _enchantment = 0;
	
	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(rawHarvestable);

	if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 4) {
		_id = net::read_int32(rawHarvestable, _dataLayout.findFragment(0)._offset); }
	else if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 2) {
		_id = net::read_int16(rawHarvestable, _dataLayout.findFragment(0)._offset); }
	_type = net::read_int8(rawHarvestable, _dataLayout.findFragment(5)._offset);
	_tier = net::read_int8(rawHarvestable, _dataLayout.findFragment(7)._offset);
	_positionX = net::read_float32(rawHarvestable, _dataLayout.findFragment(8)._offset);
	_positionY = net::read_float32(rawHarvestable, _dataLayout.findFragment(8)._offset + 4);
	_charges = net::read_int8(rawHarvestable, _dataLayout.findFragment(10)._offset);
	_enchantment = net::read_int8(rawHarvestable, _dataLayout.findFragment(11)._offset);
	
	this->_id = _id;
	this->_type = _type;
	this->_tier = _tier;
	this->_enchantment = _enchantment;
	this->_positionX = _positionX;
	this->_positionY = _positionY;
	this->_charges = _charges;
	//this->printInfo();
	//rawHarvestable.printCommandInOneString();
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
Harvestable::Harvestable(uint32_t id, uint8_t type, uint8_t tier,
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
	std::cout << std::setw(5) << (unsigned)_id << " " << (unsigned)_type << " " <<
				 (unsigned)_tier << " " << (unsigned)_enchantment << " " <<
		std::setw(3) << (unsigned)_charges << " " <<
		std::setw(8) << _positionX << " " << std::setw(8) << _positionY << "\n";
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
	uint8_t _harvestablesNum = rawHarvestableList[20];

	ptrdiff_t _idOffset = _harvestableOffsets._idOffset;
	ptrdiff_t _typeOffset = _harvestableOffsets._typeOffset + _harvestablesNum * 2;
	ptrdiff_t _tierOffset = _harvestableOffsets._tierOffset + _harvestablesNum * 3;
	ptrdiff_t _positionXOffset = _harvestableOffsets._positionXOffset + _harvestablesNum * 4;
	ptrdiff_t _positionYOffset = _harvestableOffsets._positionYOffset + _harvestablesNum * 4;
	ptrdiff_t _chargesOffset = _harvestableOffsets._chargesOffset + _harvestablesNum * 12;

	uint32_t _id = 0;
	uint8_t _type = 0;
	uint8_t _tier = 0;
	float_t _positionX = 0;
	float_t _positionY = 0;
	uint8_t _charges = 0;
	uint8_t _enchantment = 0;

	for (size_t i = 0; i < _harvestablesNum; i++) {
		_id = net::read_int16(rawHarvestableList, _idOffset + i * 2);
		_type = net::read_int8(rawHarvestableList, _typeOffset + i);
		_tier = net::read_int8(rawHarvestableList, _tierOffset + i);
		_positionX = net::read_float32(rawHarvestableList, _positionXOffset + i * 8);
		_positionY = net::read_float32(rawHarvestableList, _positionYOffset + i * 8);
		_charges = net::read_int8(rawHarvestableList, _chargesOffset + i);

		this->update(Harvestable(_id, _type, _tier, _positionX, _positionY, _charges, _enchantment));
	}
}

void HarvestableList::updateState(NetworkCommand& updateState)
{
	uint32_t _id = 0;
	uint8_t _charges = 0;
	uint8_t _enchantment = 0;

	DataLayout _dataLayout{};
	_dataLayout.findDataLayout(updateState);

	if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 4) {
		_id = net::read_int32(updateState, _dataLayout.findFragment(0)._offset); }
	else if (_dataLayout.findFragment(0)._dataType._dataTypeSize == 2) {
		_id = net::read_int16(updateState, _dataLayout.findFragment(0)._offset); }
	_charges = net::read_int8(updateState, _dataLayout.findFragment(1)._offset);
	_enchantment = net::read_int8(updateState, _dataLayout.findFragment(2)._offset);

	for (size_t i = 0; i < _harvestableList.size(); i++) {
		if (_harvestableList[i]._id == _id) {
			_harvestableList[i]._charges = _charges;
			_harvestableList[i]._enchantment = _enchantment;
			break;
		}
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
void HarvestableList::update(Harvestable harvestable)
{
	if (_harvestableList.size() == 0) {
		_harvestableList.push_back(harvestable);
	}
	else {
		for (size_t i = 0; i < _harvestableList.size(); i++) {
			//std::cout << i << " " << _harvestableList.size() << "\n";
			if (_harvestableList[i]._id == harvestable._id) {
				_harvestableList[i] = harvestable;
				break;
			}
			else if (i == (_harvestableList.size() - 1)) {
				_harvestableList.push_back(harvestable);
			}
		}
	}
}
void HarvestableList::update(HarvestableList harvestableList)
{
	for (size_t i = 0; i < harvestableList.size(); i++) {
		this->update(harvestableList[i]);
	}
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
