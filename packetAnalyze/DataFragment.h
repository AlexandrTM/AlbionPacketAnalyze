#ifndef DATA_FRAGMENT_H
#define DATA_FRAGMENT_H

class DataFragments
{
private:
    std::vector<DataFragment> _dataFragments;

public:
    void findDataFragments(NetworkCommand& command);
    DataFragments(std::vector<DataFragment> dataFragments);
    DataFragments();

};


// ============================== DataFragment ==============================


struct DataFragment
{
    uint8_t _dataTypeSize;
    uint8_t _dataHeaderSize;
    uint16_t _numOfEntries;


    static DataFragment findDataFragmentInfo(uint8_t dataType, NetworkCommand& command, ptrdiff_t offset);
    static uint8_t findDataTypeSize(uint8_t dataType);
    static ptrdiff_t findFragmentsNumOffset(NetworkCommand& command);

    DataFragment(uint8_t dataTypeSize, uint8_t dataHeaderSize, uint16_t numOfEntries);
    DataFragment();
    void printInfo();
};

#endif