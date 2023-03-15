#include "pch.h"

// **************************************************************************
// ============================== DataType ==================================
// **************************************************************************


uint8_t DataType::getDataTypeSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int8_list:
        return 1;
    case dataType::int8_string:
        return 1;
    case dataType::int16:
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
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int16:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::int8_list:
        return 5;
    case dataType::int8_string:
        return 3;
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
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int16:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::int8_list:
        return (command[offset + 4] << 8) | command[offset + 5];
    case dataType::int8_string:
        return (command[offset + 2] << 8) | command[offset + 3];
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


template<typename T>
T DataLayout::readDataFragmentEntry(NetworkCommand& command, size_t fragmentID)
{
    T entry;
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentID == fragmentID) {
            ptrdiff_t _offset = _dataLayout[i]._offset;
            uint8_t _dataType = _dataLayout[i]._dataType._dataType;
            switch (_dataType)
            {
            case dataType::int8:
                entry = NetworkCommand::read_byteInt(command, _offset);
            case dataType::int16:
                entry = NetworkCommand::read_shortInt(command, _offset);
            case dataType::int32:
                entry = NetworkCommand::read_int32(command, _offset);
            case dataType::float32:
                entry = NetworkCommand::read_float32(command, _offset);
            case dataType::int64:
                entry = NetworkCommand::read_int64(command, _offset);
            default:
                return entry;
            }
        }
        else {
            return (uint8_t)0;
        }
    }
}

ptrdiff_t DataLayout::findFragmentOffset(uint8_t fragmentID)
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentID == (uint8_t)fragmentID) {
            return _dataLayout[i]._offset;
        }
    }
    return -1;
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
