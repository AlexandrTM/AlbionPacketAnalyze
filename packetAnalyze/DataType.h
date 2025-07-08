#pragma once

struct DataType
{
    uint8_t _size;
    uint8_t _headerSize;
    uint8_t _dataTypeId;

    DataType();
    DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType);

    static uint8_t getDataTypeSize(uint8_t dataType);
    static uint8_t getDataTypeHeaderSize(uint8_t dataType);
    static uint16_t getNumOfEntries(NetworkCommand& command, uint8_t dataType, ptrdiff_t offset);
};
