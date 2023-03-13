#ifndef DATA_FRAGMENT_H
#define DATA_FRAGMENT_H
struct DataFragment
{
    uint8_t _fragmentID;
    uint8_t _dataTypeSize;
    uint8_t _dataHeaderSize;
    uint16_t _numOfEntries;
    ptrdiff_t _offset;


    static DataFragment findDataElementInfo(NetworkCommand& command, ptrdiff_t offset);
    static uint8_t findDataTypeSize(uint8_t dataType);
    static ptrdiff_t findFragmentsNumOffset(NetworkCommand& command);

    DataFragment(uint8_t fragmentID, uint8_t dataTypeSize, 
                uint8_t dataHeaderSize, uint16_t numOfEntries, ptrdiff_t offset);
    DataFragment();
    void printInfo();
};


// **************************************************************************
// ============================== DataLayout =============================
// **************************************************************************


class DataLayout
{
private:
    std::vector<DataFragment> _dataLayout;

public:
    size_t findFragmentOffset(std::vector<uint8_t> fragmentIDs, uint8_t fragmentID);

    void findDataLayout(NetworkCommand& command);

    DataLayout(std::vector<DataFragment> dataFragments);
    DataLayout();

    void printInfo();

    size_t size();
    DataFragment operator[](size_t elementIndex);

};

#endif