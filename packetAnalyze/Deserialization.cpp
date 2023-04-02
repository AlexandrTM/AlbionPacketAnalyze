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
    _dataTypeHeaderSize = 0;
    _dataType = 0;
}
DataType::DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType)
{
    _dataTypeSize = dataTypeSize;
    _dataTypeHeaderSize = dataTypeHeaderSize;
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
        return (command[offset + 3] << 8) | command[offset + 4];
    case dataType::int8_string:
        return (command[offset + 1] << 8) | command[offset + 2];
    case dataType::dictionary:
        return (command[offset + 1] << 8) | command[offset + 2];
    default:
        break;
    }
}
ptrdiff_t DataFragment::findFragmentsNumOffset(NetworkCommand& command)
{
    switch (command.getCommandType())
    {
    case commandType::reliable:
        if (command.getOperationType() == operationType::operationResponse) {
            return 19;
        }
        else {
            return 16;
        }
    case commandType::unreliable:
        return 20;
    case commandType::fragmented:
        return 39;
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
    _offset = -1;
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
            ptrdiff_t offset = _dataLayout[i]._offset;
            uint8_t dataType = _dataLayout[i]._dataType._dataType;
            switch (dataType)
            {
            case dataType::int8:
                entry = NetworkCommand::read_byteInt(command, offset);
            case dataType::int16:
                entry = NetworkCommand::read_shortInt(command, offset);
            case dataType::int32:
                entry = NetworkCommand::read_int32(command, offset);
            case dataType::float32:
                entry = NetworkCommand::read_float32(command, offset);
            case dataType::int64:
                entry = NetworkCommand::read_int64(command, offset);
            default:
                return entry;
            }
        }
        else {
            return (uint8_t)0;
        }
    }
}

DataFragment DataLayout::findFragment(uint8_t fragmentID)
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentID == (uint8_t)fragmentID) {
            return _dataLayout[i];
        }
    }
    return DataFragment{};
}

void DataLayout::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t numOfFragments = command[offset];
    offset += 1;

    uint8_t fragmentID;

    uint8_t dataType;
    uint8_t dataTypeSize;
    uint8_t dataTypeHeaderSize;
    uint16_t numOfEntries;
    uint16_t sizeOfData = 0;

    DataFragment dataFragment;

    //std::cout << (unsigned)numOfFragments << "\n";
    //command.printCommandInOneString();
    for (size_t i = 0; i < numOfFragments; i++) {
        fragmentID = command[offset];
        offset += 1;
        dataType = command[offset];

        numOfEntries = DataFragment::findNumOfEntries(command, dataType, offset);

        if (dataType == dataType::dictionary) { 
            offset += 3;
            dataType = command[offset]; 
            if (dataType == dataType::int8_string) {

                offset += 1;
                for (size_t i = 0; i < numOfEntries; i++) {

                    offset += 2;
                    dataFragment = DataFragment(
                                                fragmentID,
                                                offset,
                                                sizeOfData,
                                                DataType(1, 3, dataType::int8_string)
                                                );

                    _dataLayout.push_back(dataFragment);

                    //std::cout << "size of data " << sizeOfData << "\n";
                    sizeOfData = DataFragment::findNumOfEntries(command, dataType, offset - 3);
                    offset += sizeOfData;
                }
                continue;
            }
        }
        dataTypeSize = DataType::getDataTypeSize(dataType);
        dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
        offset += dataTypeHeaderSize;

        dataFragment = DataFragment(
                                    fragmentID, 
                                    offset,
                                    numOfEntries, 
                                    DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                                    );

        sizeOfData = dataTypeSize * numOfEntries;
        offset += sizeOfData;

        _dataLayout.push_back(dataFragment);
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
