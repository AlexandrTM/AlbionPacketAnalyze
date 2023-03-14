#include "pch.h"


// **************************************************************************
// ============================== DataType ==================================
// **************************************************************************


uint8_t DataType::getDataTypeSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return 1;
    case dataType::byteIntList:
        return 1;
    case dataType::shortInt:
        return 2;
    case dataType::float32:
        return 4;
    case dataType::int32:
        return 4;
    case dataType::int64:
        return 8;
    default:
        break;
    }
}
uint8_t DataType::getDataTypeHeaderSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return 1;
    case dataType::shortInt:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::byteIntList:
        return 5;
    case dataType::dictionary:
        return 4;
    default:
        break;
    }
}

DataType::DataType()
{
    _dataTypeSize = 0;
    _dataHeaderSize = 0;
    _dataType = 0;
}
DataType::DataType(uint8_t dataTypeSize, uint8_t dataHeaderSize, uint8_t dataType)
{
    _dataTypeSize = dataTypeSize;
    _dataHeaderSize = dataHeaderSize;
    _dataType = dataType;
}


// **************************************************************************
// ============================== DataFragment ==============================
// **************************************************************************


uint16_t DataFragment::findNumOfEntries(NetworkCommand& command, uint8_t dataType, ptrdiff_t offset)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return 1;
    case dataType::shortInt:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::byteIntList:
        return (command[offset + 4] << 8) | command[offset + 5];
    case dataType::dictionary:
        return (command[offset + 2] << 8) | command[offset + 3];
    default:
        break;
    }
}
ptrdiff_t DataFragment::findFragmentsNumOffset(NetworkCommand& command)
{
    switch (command.getCommandType())
    {
    case commandType::reliable:
        return 16;
    case commandType::unreliable:
        return 20;
    case commandType::fragmented:
        return 36;
    default:
        return -1;
    }
}

DataFragment::DataFragment(uint8_t fragmentID, ptrdiff_t offset, uint16_t numOfEntries, DataType dataType)
{
    _fragmentID = fragmentID;
    _offset = offset;
    _numOfEntries = numOfEntries;
    _dataType = dataType;
}
DataFragment::DataFragment()
{
    _fragmentID = 0;
    _offset = 0;
    _numOfEntries = 0;
    _dataType = {};
}
void DataFragment::printInfo()
{
    std::cout << (unsigned)_fragmentID << " " << (unsigned)_offset << "\n";
}


// **************************************************************************
// ============================== DataLayout ================================
// **************************************************************************


std::map<uint8_t, size_t> DataLayout::getLayoutInfo()
{
    std::map<uint8_t, size_t> _dataLayoutInfo{};

    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayoutInfo.insert(std::make_pair(_dataLayout[i]._fragmentID, _dataLayout[i]._offset));
        //std::cout << _dataLayout[i]._offset;
        //std::cout << " " << (unsigned)_dataLayout[i]._fragmentID << "\n";
    }
    std::cout << _dataLayout.size() << " " << _dataLayoutInfo.size() << "\n";
    return _dataLayoutInfo;
}
size_t DataLayout::findFragmentsAvailable()
{

}

template<typename T>
T DataLayout::readDataFragmentEntry(NetworkCommand& command, uint8_t fragmentID)
{
    T entry;
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentID == fragmentID) {
            entry = NetworkCommand::readDataEntry(command, _dataLayout[i]._offset, 
                                                  _dataLayout[i]._dataType._dataTypeSize);
            switch (_dataLayout[i]._dataType._dataType)
            {
            case dataType::float32:
                return std::binToFloat(entry);
            default:
                return entry;
            }
        }
        else {
            return T(0);
        }
    }
}

size_t DataLayout::findFragmentOffset(std::vector<uint8_t> fragmentIDs, uint8_t fragmentID)
{
    if (std::isElementInVector(fragmentIDs, (uint8_t)fragmentID)) {
        DataFragment _dataFragment = _dataLayout[std::findElementIndex(fragmentIDs, (uint8_t)fragmentID)];
        return _dataFragment._offset;
    }
    else {
        return 0;
    }
}

void DataLayout::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t _offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t _numOfFragments = command[_offset];
    _offset += 1;

    uint16_t _dataSize;
    for (size_t i = 0; i < _numOfFragments - 1; i++) { // excluding event code
        uint8_t _fragmentID = command[_offset];
        uint8_t _dataType = command[_offset + 1];

        uint8_t _dataTypeHeaderSize = DataType::getDataTypeHeaderSize(_dataType);
        uint16_t _numOfEntries = DataFragment::findNumOfEntries(command, _dataType, _offset);

        if (_dataType == dataType::dictionary) { _dataType = (command[_offset + 4]); }
        uint8_t _dataTypeSize = DataType::getDataTypeSize(_dataType);

        DataFragment _dataFragment = DataFragment(_fragmentID, _offset + _dataTypeHeaderSize + 1,
                                        _numOfEntries, DataType(_dataTypeSize, _dataTypeHeaderSize, _dataType));

        _dataSize = _dataTypeSize * _numOfEntries + _dataTypeHeaderSize + 1;
        _offset += _dataSize;

        _dataLayout.push_back(_dataFragment);
    }
}
DataLayout::DataLayout(std::vector<DataFragment> dataFragments)
{
    _dataLayout = dataFragments;
}
DataLayout::DataLayout()
{
    _dataLayout = {};
}

void DataLayout::printInfo()
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayout[i].printInfo();
    }
}

size_t DataLayout::size()
{
    return _dataLayout.size();
}
DataFragment DataLayout::operator[](size_t elementIndex)
{
    return _dataLayout[elementIndex];
}
