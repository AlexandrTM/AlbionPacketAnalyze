#pragma once

class DataLayout
{
private:
    DataFragment _defaultDataFragment{};
    std::vector<DataFragment> _dataLayout;

public:
    DataFragment& findFragment(uint8_t fragmentId);
    std::vector<std::reference_wrapper<DataFragment>> findFragments(uint8_t fragmentId);

    uint8_t findNumOfFragments(NetworkCommand& command);
    void findDataLayout(NetworkCommand& command);
    void processDictionary(NetworkCommand& command, uint8_t& fragmentId, ptrdiff_t& offset,
        uint16_t numOfEntries, uint8_t& dataTypeHeaderSize, size_t index);

    DataLayout(std::vector<DataFragment> dataFragments);
    DataLayout();

    void printInfo(NetworkCommand& command, bool printPayload = true) const;
    void printInfo(NetworkCommand& command, size_t beginFragment, bool printPayload) const;
    void printInfo(NetworkCommand& command, size_t beginFragment, size_t endFragment, bool printPayload) const;

    size_t size() const;
    DataFragment operator[](size_t elementIndex);

};
