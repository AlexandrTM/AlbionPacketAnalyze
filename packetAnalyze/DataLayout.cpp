#include "pch.h"
#include "DataLayout.h"

DataFragment& DataLayout::findFragment(uint8_t fragmentId)
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentId == (uint8_t)fragmentId) {
            while (_dataLayout[i]._dataType._dataTypeId == dataType::listOfType) {
                i += 1;
            }
            return _dataLayout[i];
        }
    }
    return _defaultDataFragment;
}
std::vector<std::reference_wrapper<DataFragment>> DataLayout::findFragments(uint8_t fragmentId)
{
    std::vector<std::reference_wrapper<DataFragment>> matchingFragments;

    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentId == fragmentId) {
            while (_dataLayout[i]._dataType._dataTypeId == dataType::listOfType) {
                i += 1;
            }
            matchingFragments.push_back(_dataLayout[i]);
        }
    }

    return matchingFragments;
}

uint8_t DataLayout::findNumOfFragments(NetworkCommand& command)
{
    ptrdiff_t offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t numOfFragments = command[offset];

    return numOfFragments;
}
void DataLayout::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t numOfFragments = command[offset];
    /*std::cout << 
        "event code: " << command.getEventCode() << " " <<
        "size:" << (unsigned)command.size() << " " <<
        "num of fragments: " << (unsigned)numOfFragments << " " << "\n";*/
    //command.printCommandInOneString(offset, command.size());
    offset += 1;

    uint8_t fragmentID = 0;

    uint8_t dataType = 0;
    uint8_t dataTypeSize = 0;
    uint8_t dataTypeHeaderSize = 0;
    uint16_t numOfEntries = 0;
    uint16_t sizeOfData = 0;
    uint16_t commandSize = command.size();

    DataFragment dataFragment;

    for (size_t i = 0; i < numOfFragments; i++) {
        if (offset > commandSize) {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << 
                "offset is bigger than command size (hex dec): " << 
                std::hex << commandSize << std::dec << " " << commandSize << "\n" <<
                "offset (hex dec): " << 
                std::hex << offset << std::dec << " " << offset << "\n";
            command.printCommandInOneString();
            SetConsoleTextAttribute(consoleHandle, 7);
            _dataLayout.clear();
            return;
        }
        //std::cout << "start offset: " << offset << " ";
        if (offset < commandSize) {
            fragmentID = command[offset];
            //std::cout << "fragmentId: " << (unsigned)command[offset] << " ";
            offset += 1;
            dataType = command[offset];

            numOfEntries = DataType::getNumOfEntries(command, dataType, offset);
            /*if (dataType == dataType::int16) {
                std::cout << "numOfEntries: " << numOfEntries << " ";
            }*/
            
            if (dataType == dataType::listOfType) {
                offset += 3;
                dataTypeHeaderSize = 3;
                dataTypeSize = DataType::getDataTypeSize(dataType);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                );
                _dataLayout.push_back(dataFragment);

                dataType = command[offset];
                //processDictionary(command, fragmentId, offset, numOfEntries, dataTypeHeaderSize, 0);
                if (numOfEntries == 0) {
                    offset += 1;
                    dataTypeHeaderSize = 1;
                    sizeOfData = 0;

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        sizeOfData,
                        DataType(1, dataTypeHeaderSize, dataType)
                    );

                    _dataLayout.push_back(dataFragment);
                }
                else if (dataType == dataType::listOfType) {
                    uint16_t nestedNumOfEntries = DataType::getNumOfEntries(
                        command, dataType, offset);
                    offset += 3;
                    dataTypeHeaderSize = 3;

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        numOfEntries,
                        DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                    );
                    _dataLayout.push_back(dataFragment);

                    dataType = command[offset];
                    if (dataType == dataType::float32) {
                        dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                        dataTypeSize = DataType::getDataTypeSize(dataType);
                        offset += DataType::getDataTypeHeaderSize(dataType);
                        for (size_t i = 0; i < numOfEntries; i++) {
                            if (i > 0) {
                                offset += 3;
                            }

                            dataFragment = DataFragment(
                                fragmentID,
                                offset,
                                nestedNumOfEntries,
                                DataType(dataTypeSize,
                                    i == 0 ? dataTypeHeaderSize : 3, dataType::int8_string)
                            );

                            _dataLayout.push_back(dataFragment);

                            sizeOfData = dataTypeSize * nestedNumOfEntries;
                            offset += sizeOfData;
                        }
                    }
                }
                else if (dataType == dataType::int8_list || dataType == dataType::int8_string) {
                    uint16_t nestedNumOfEntries = DataType::getNumOfEntries(command, dataType, offset);
                    uint8_t nestedDataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                    dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);

                    offset += 1;
                    for (size_t i = 0; i < numOfEntries; i++) {
                        offset += nestedDataTypeHeaderSize - 1;
                        sizeOfData = DataType::getNumOfEntries(command, dataType, offset - nestedDataTypeHeaderSize);

                        dataFragment = DataFragment(
                            fragmentID,
                            offset,
                            sizeOfData,
                            DataType(1, i == 0 ? dataTypeHeaderSize : nestedDataTypeHeaderSize - 1, dataType)
                        );

                        _dataLayout.push_back(dataFragment);
                        offset += sizeOfData;

                        /*std::cout <<
                            "offset: "         << (unsigned)dataFragment._offset << " " <<
                            std::hex <<
                            "fragment id: "    << (unsigned)fragmentId           << " " <<
                            "data type: "      << (unsigned)dataType             << " " <<
                            std::dec <<
                            "size: "           << (unsigned)dataTypeSize         << " " <<
                            "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                            "num of entries: " << (unsigned)numOfEntries         << " " <<
                            "size of data: "   << (unsigned)sizeOfData           << "\n";*/
                    }
                }
                else {
                    dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                    dataTypeSize = DataType::getDataTypeSize(dataType);
                    offset += DataType::getDataTypeHeaderSize(dataType);

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        numOfEntries,
                        DataType(dataTypeSize, dataTypeHeaderSize, dataType::int8_string)
                    );

                    _dataLayout.push_back(dataFragment);

                    //std::cout << "size of data " << sizeOfData << "\n";
                    sizeOfData = dataTypeSize * numOfEntries;
                    offset += sizeOfData;
                }
            }
            else if (dataType == dataType::dictionary) {
                numOfEntries = DataType::getNumOfEntries(command, dataType, offset);

                offset += 1;
                              dataType     = command[offset];
                              dataTypeSize = DataType::getDataTypeSize(dataType);
                uint8_t nestedDataType     = command[offset + 1];
                uint8_t nestedDataTypeSize = DataType::getDataTypeSize(nestedDataType);

                sizeOfData = 4 + numOfEntries * (dataTypeSize + nestedDataTypeSize);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize + nestedDataTypeSize, 4, dataType::dictionary)
                );

                _dataLayout.push_back(dataFragment);
                /*std::cout <<
                    "offset: "         << (unsigned)dataFragment._offset << " " <<
                    std::hex <<
                    "fragment id: "    << (unsigned)fragmentId           << " " <<
                    "data type: "      << (unsigned)dataType             << " " <<
                    std::dec <<
                    "size: "           << (unsigned)dataTypeSize         << " " <<
                    "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                    "num of entries: " << (unsigned)numOfEntries         << " " <<
                    "size of data: "   << (unsigned)sizeOfData           << "\n";*/

                offset += sizeOfData;
            }
            else {
                dataTypeSize = DataType::getDataTypeSize(dataType);
                dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                offset += DataType::getDataTypeHeaderSize(dataType);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                );

                sizeOfData = dataTypeSize * numOfEntries;
                offset += sizeOfData;
                _dataLayout.push_back(dataFragment);

                /*std::cout <<
                    "offset: "         << (unsigned)dataFragment._offset << " " <<
                    std::hex <<
                    "fragment id: "    << (unsigned)fragmentId           << " " <<
                    "data type: "      << (unsigned)dataType             << " " <<
                    std::dec <<
                    "size: "           << (unsigned)dataTypeSize         << " " <<
                    "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                    "num of entries: " << (unsigned)numOfEntries         << " " <<
                    "size of data: "   << (unsigned)sizeOfData           << "\n";*/
            }
        }
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

void DataLayout::printInfo(NetworkCommand& command, bool printPayload) const
{
    if (_dataLayout.size() == 0) {
        return;
    }

    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout << 
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayout[i].printFragmentInfo(command, currentStringPosition, printPayload);

        /*if (i < _dataLayout.size() - 1) {
            command.printCommandInOneString(_dataLayout[i]._offset, _dataLayout[i + 1]._offset, false);
        }
        else {
            command.printCommandInOneString(_dataLayout[i]._offset, command.size(), false);
        }
        std::cout << " ";*/
    }
    std::cout << "\n\n";
}
void DataLayout::printInfo(NetworkCommand& command, size_t beginFragment, bool printPayload) const
{
    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout <<
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    for (const DataFragment& dataFragment : _dataLayout) {
        if (dataFragment._fragmentId >= beginFragment) {
            dataFragment.printFragmentInfo(command, currentStringPosition, printPayload);
        }
    }
    std::cout << "\n\n";
}
void DataLayout::printInfo(NetworkCommand& command, size_t beginFragment, size_t endFragment, bool printPayload) const
{
    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout <<
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    // смена авалона разные связи и одна и таже связь
    std::vector<uint8_t> nonWantedElements = { 
        0, 1, 2, 3, 4, 5, 6, 7, 11, 12, 14, 15, 16, 17, 19, 20, 21, 22, 24, 25, 26, 27, 29, 30, 
        32, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 
        48, 49, 50, 51, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 66, 67, 68, 69, 70, 72, 74, 
        75, 76, 77, 78, 79, 
        81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 95, 96, 98, 99, 100, 101, 102, 103, 
        104, 105, 106, 114, 115, 116, 118, 119, 253
    };
    for (const DataFragment& dataFragment : _dataLayout) {
        if (dataFragment._fragmentId >= beginFragment && dataFragment._fragmentId <= endFragment
             && !std::isElementInVector(nonWantedElements, dataFragment._fragmentId)) {
            dataFragment.printFragmentInfo(command, currentStringPosition, printPayload);
        }
    }
    std::cout << "\n\n";
}

size_t DataLayout::size() const
{
    return _dataLayout.size();
}
DataFragment DataLayout::operator[](size_t elementIndex)
{
    return _dataLayout[elementIndex];
}

