#include "pch.h"
#include "DataFragment.h"

void DataFragments::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t _offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t _fragmentsNum = command[_offset];
    _offset += 1;

    uint8_t _fragmentID;
    uint8_t _dataType;

    uint16_t _dataSize;

    for (size_t i = 0; i < _fragmentsNum - 1; i++) { // excluding event code
        _fragmentID = command[_offset];
        _dataType = command[_offset + 1];

        DataFragment _dataFragment = DataFragment::findDataFragmentInfo(_fragmentID, _dataType, command, _offset);

        _dataSize = _dataFragment._dataHeaderSize +
                    _dataFragment._dataTypeSize * _dataFragment._numOfEntries + 1;
        _offset += _dataSize;

        _dataLayout.push_back(_dataFragment);
        //std::cout << (unsigned)_fragmentID << " " << (unsigned)_dataType << " " << (unsigned)_dataSize << "\n";
    }
}
DataFragments::DataFragments(std::vector<DataFragment> dataFragments)
{
    _dataLayout = dataFragments;
}
DataFragments::DataFragments()
{
    _dataLayout = {};
}

void DataFragments::printInfo()
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayout[i].printInfo();
    }
}

size_t DataFragments::size()
{
    return _dataLayout.size();
}

DataFragment DataFragments::operator[](size_t elementIndex)
{
    return _dataLayout[elementIndex];
}


// **************************************************************************
// ============================== DataFragment ==============================
// **************************************************************************


DataFragment DataFragment::findDataFragmentInfo(uint8_t fragmentID, uint8_t dataType, NetworkCommand& command, ptrdiff_t offset)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return DataFragment(fragmentID, 1, 1, 1);
    case dataType::byteIntList:
        return DataFragment(fragmentID, 1, 5, (command[offset + 4] << 8) |
                                               command[offset + 5]);
    case dataType::shortInt:
        return DataFragment(fragmentID, 2, 1, 1);
    case dataType::int32:
        return DataFragment(fragmentID, 4, 1, 1);
    case dataType::float32:
        return DataFragment(fragmentID, 4, 1, 1);
    case dataType::int64:
        return DataFragment(fragmentID, 8, 1, 1);
    case dataType::dictionary:
        return DataFragment(fragmentID, DataFragment::findDataTypeSize(command[offset + 4]), 
                            4, 
                            (command[offset + 2] << 8) | command[offset + 3]);
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

DataFragment::DataFragment(uint8_t fragmentID, uint8_t dataTypeSize, uint8_t dataHeaderSize, uint16_t numOfEntries)
{
    _fragmentID = fragmentID;
    _dataTypeSize = dataTypeSize;
    _dataHeaderSize = dataHeaderSize;
    _numOfEntries = numOfEntries;
}
DataFragment::DataFragment()
{
    _dataTypeSize = 0;
    _dataHeaderSize = 0;
    _numOfEntries = 0;
}
void DataFragment::printInfo()
{
    std::cout << (unsigned)_dataTypeSize << " " << (unsigned)_dataHeaderSize << " " << (unsigned)_numOfEntries << "\n";
}