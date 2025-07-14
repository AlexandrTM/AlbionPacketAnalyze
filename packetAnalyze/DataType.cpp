#include "pch.h"
#include "DataType.h"

uint8_t DataType::getDataTypeSize(uint8_t dataType)
{
    switch (dataType) {
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
    case dataType::dictionary:
        return 0;
    case dataType::listOfType:
        return 0;
    default:
        return 1;
        break;
    }
}
uint8_t DataType::getDataTypeHeaderSize(uint8_t dataType)
{
    switch (dataType) {
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
    case dataType::dictionary:
        return 4;
    case dataType::int8_list:
        return 5;
    case dataType::int8_string:
        return 3;
    case dataType::listOfType:
        return 3;
    default:
        std::cout << "new data type: " << (unsigned)dataType << " ";
        return 1;
        break;
    }
}
uint16_t DataType::getNumOfEntries(NetworkCommand& command, uint8_t dataType, ptrdiff_t offset)
{
    switch (dataType) {
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
    case dataType::dictionary:
        return (command[offset + 3] << 8) | command[offset + 4];;
    case dataType::int8_list:
        return (command[offset + 3] << 8) | command[offset + 4];
    case dataType::int8_string:
        return (command[offset + 1] << 8) | command[offset + 2];
    case dataType::listOfType:
        return (command[offset + 1] << 8) | command[offset + 2];
    default:
        return 0;
        break;
    }
}

DataType::DataType()
{
    _size = 0;
    _headerSize = 0;
    _dataTypeId = 0;
}
DataType::DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType)
{
    _size = dataTypeSize;
    _headerSize = dataTypeHeaderSize;
    _dataTypeId = dataType;
}
