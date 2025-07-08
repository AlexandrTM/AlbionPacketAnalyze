#pragma once

struct DataFragment
{
    uint8_t _fragmentId;
    ptrdiff_t _offset;
    uint16_t _numOfEntries;
    DataType _dataType;

    static ptrdiff_t findFragmentsNumOffset(NetworkCommand& command);

    DataFragment(uint8_t fragmentID, ptrdiff_t offset, uint16_t numOfEntries, DataType dataType);
    DataFragment();
    void printInfo(NetworkCommand& command) const;
    void printFragmentInfo(NetworkCommand& command, size_t& currentStringPosition,
        bool printPayload = true) const;
};
