#ifndef DATA_FRAGMENT_H
#define DATA_FRAGMENT_H


// **************************************************************************
// ============================== DataType ==================================
// **************************************************************************


struct DataType
{
    uint8_t _dataTypeSize;
    uint8_t _dataHeaderSize;
    uint8_t _dataType;

    DataType();
    DataType(uint8_t dataTypeSize, uint8_t dataHeaderSize, uint8_t dataType);

    static uint8_t getDataTypeSize(uint8_t dataType);
    static uint8_t getDataTypeHeaderSize(uint8_t dataType);
};


// **************************************************************************
// ============================== DataFragment ==============================
// **************************************************************************


struct DataFragment
{
    uint8_t _fragmentID;
    ptrdiff_t _offset;
    uint16_t _numOfEntries;
    DataType _dataType;


    static ptrdiff_t findFragmentsNumOffset(NetworkCommand& command);
    static uint16_t findNumOfEntries(NetworkCommand& command, uint8_t dataType, ptrdiff_t offset);

    DataFragment(uint8_t fragmentID, ptrdiff_t offset, uint16_t numOfEntries, DataType dataType);
    DataFragment();
    void printInfo();
};


// **************************************************************************
// ============================== DataLayout ================================
// **************************************************************************


class DataLayout
{
private:
    std::vector<DataFragment> _dataLayout;

public:
    size_t findFragmentOffset(std::vector<uint8_t> fragmentIDs, uint8_t fragmentID);
    std::map<uint8_t, size_t> getLayoutInfo();
    size_t findFragmentsAvailable();

    template<typename T>
    T readDataFragmentEntry(NetworkCommand& command, uint8_t fragmentID);

    void findDataLayout(NetworkCommand& command);

    DataLayout(std::vector<DataFragment> dataFragments);
    DataLayout();

    void printInfo();

    size_t size();
    DataFragment operator[](size_t elementIndex);

};

#endif
