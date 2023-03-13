#include "pch.h"

size_t DataLayout::findFragmentOffset(std::vector<uint8_t> fragmentIDs, uint8_t fragmentID)
{
    if (std::isElementInVector(fragmentIDs, (uint8_t)fragmentID)) {
        DataFragment _dataFragment = _dataLayout[std::findElementIndex(fragmentIDs, (uint8_t)fragmentID)];
        //std::cout << _dataFragment._offset << "\n";
        return _dataFragment._offset + _dataFragment._dataHeaderSize + 1;
    }
    else {
        return 0;
    }
}

void DataLayout::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t _offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t _fragmentsNum = command[_offset];
    _offset += 1;

    uint16_t _dataSize;

    for (size_t i = 0; i < _fragmentsNum - 1; i++) { // excluding event code

        DataFragment _dataFragment = DataFragment::findDataElementInfo(command, _offset);

        _dataSize = _dataFragment._dataHeaderSize +
                    _dataFragment._dataTypeSize * _dataFragment._numOfEntries + 1;
        _offset += _dataSize;

        _dataLayout.push_back(_dataFragment);
        //std::cout << (unsigned)_fragmentID << " " << (unsigned)_dataType << " " << (unsigned)_dataSize << "\n";
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


// **************************************************************************
// ============================== DataFragment ==============================
// **************************************************************************


DataFragment DataFragment::findDataElementInfo(NetworkCommand& command, ptrdiff_t offset)
{
    switch (command[offset + 1])
    {
    case dataType::byteInt:
        return DataFragment(command[offset], 1, 1, 1, offset);
    case dataType::byteIntList:
        return DataFragment(command[offset], 1, 5, (command[offset + 4] << 8) |
                                                   command[offset + 5], offset);
    case dataType::shortInt:
        return DataFragment(command[offset], 2, 1, 1, offset);
    case dataType::int32:
        return DataFragment(command[offset], 4, 1, 1, offset);
    case dataType::float32:
        return DataFragment(command[offset], 4, 1, 1, offset);
    case dataType::int64:
        return DataFragment(command[offset], 8, 1, 1, offset);
    case dataType::dictionary:
        return DataFragment(command[offset], DataFragment::findDataTypeSize(command[offset + 4]), 4, 
                          (command[offset + 2] << 8) | command[offset + 3], offset);
    default:
        break;
    }
}
uint8_t DataFragment::findDataTypeSize(uint8_t dataType)
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

DataFragment::DataFragment(uint8_t fragmentID, uint8_t dataTypeSize, 
                         uint8_t dataHeaderSize, uint16_t numOfEntries, ptrdiff_t offset)
{
    _fragmentID = fragmentID;
    _dataTypeSize = dataTypeSize;
    _dataHeaderSize = dataHeaderSize;
    _numOfEntries = numOfEntries;
    _offset = offset;
}
DataFragment::DataFragment()
{
    _fragmentID = 0;
    _dataTypeSize = 0;
    _dataHeaderSize = 0;
    _numOfEntries = 0;
    _offset = 0;
}
void DataFragment::printInfo()
{
    std::cout << (unsigned)_fragmentID << " " << (unsigned)_dataTypeSize << " " << 
                 (unsigned)_dataHeaderSize << " " << (unsigned)_numOfEntries << " " << 
                 (unsigned)_offset << "\n";
}