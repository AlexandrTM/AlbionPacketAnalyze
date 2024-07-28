#ifndef DATA_FRAGMENT_H
#define DATA_FRAGMENT_H


struct net
{
    static inline uint8_t read_uint8(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            return command[offset];
        }
        return 0;
    }
    static inline uint16_t read_uint16(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint16_t _dataEntry = 0;
            for (size_t i = 0; i < 2; i++) {
                _dataEntry += (command[offset + i] << (8 * (1 - i)));
            }
            return _dataEntry;
        }
        return 0;
    }
    static inline uint32_t read_uint32(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint32_t _dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                _dataEntry += (command[offset + i] << (8 * (3 - i)));
            }
            return _dataEntry;
        }
        return 0;
    }
    static inline uint32_t read_uint32(std::vector<uint8_t> rawCommand, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint32_t _dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                _dataEntry += (rawCommand[offset + i] << (8 * (3 - i)));
            }
            return _dataEntry;
        }
        return 0;
    }
    static inline float_t read_float32(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint32_t _dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                _dataEntry += (command[offset + i] << (8 * (3 - i)));
            }
            return std::binToFloat(_dataEntry);
        }
        return 0;
    }
    static inline float_t read_float32big(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint32_t _dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                _dataEntry += (command[offset + 3 - i] << (8 * (3 - i)));
            }
            return std::binToFloat(_dataEntry);
        }
        return 0;
    }
    static inline uint64_t read_uint64(NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != -1) {
            uint64_t _dataEntry = 0;
            for (size_t i = 0; i < 8; i++) {
                _dataEntry += (static_cast<unsigned __int64>(command[offset + i]) << (8 * (7 - i)));
            }
            return _dataEntry;
        }
        return 0;
    }
};


// **************************************************************************
// ============================== DataType ==================================
// **************************************************************************


struct DataType
{
    uint8_t _size;
    uint8_t _headerSize;
    uint8_t _dataType;

    DataType();
    DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType);

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
    void printInfo(NetworkCommand& command) const;
    void printFragmentInfo(NetworkCommand& command, size_t& currentPrintPosition) const;
};


// **************************************************************************
// ============================== DataLayout ================================
// **************************************************************************


class DataLayout
{
private:
    std::vector<DataFragment> _dataLayout;

public:
    DataFragment findFragment(uint8_t fragmentID) const;

    template<typename T>
    T readDataFragmentEntry(NetworkCommand& command, size_t fragmentID);

    void findDataLayout(NetworkCommand& command);
    void processDictionary(NetworkCommand& command, uint8_t& fragmentID, ptrdiff_t& offset, 
        uint16_t numOfEntries, uint8_t& dataTypeHeaderSize, size_t index);

    DataLayout(std::vector<DataFragment> dataFragments);
    DataLayout();

    void printInfo(NetworkCommand& command) const;

    size_t size();
    DataFragment operator[](size_t elementIndex);

};

#endif
