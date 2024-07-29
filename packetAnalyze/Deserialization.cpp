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
        return 1;
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
        return 3;
    default:
        return 0;
        break;
    }
}

DataType::DataType()
{
    _size = 0;
    _headerSize = 0;
    _dataType = 0;
}
DataType::DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType)
{
    _size = dataTypeSize;
    _headerSize = dataTypeHeaderSize;
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
        return 0;
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
void DataFragment::printInfo(NetworkCommand& command) const
{
    size_t commandSizeLength = std::to_string(command.size()).length();
    std::cout << "id: " << std::setw(commandSizeLength) << (unsigned)_fragmentID << " "
              << "of: " << std::setw(commandSizeLength) << (unsigned)_offset << " ";
}

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
void DataFragment::printFragmentInfo(NetworkCommand& command, size_t& currentPrintPosition) const
{
    // take dicionaty into account
    ptrdiff_t startOffset = _offset - _dataType._headerSize - 1;
    ptrdiff_t endOffset = _offset + _dataType._size * _numOfEntries;
    size_t fragmentLength = endOffset - startOffset + 3;

    std::cout << 
        "data type:" << (unsigned)_dataType._dataType << " " <<
        "size: " << (unsigned)_dataType._size << " " <<
        "header size: " << (unsigned)_dataType._headerSize << " " <<
        "offset: " << (unsigned)_offset << " " <<
        "num of entries: " << (unsigned)_numOfEntries << "\n";
    
    currentPrintPosition += fragmentLength;
    if (currentPrintPosition > 40) {
        std::cout << "\n";
        currentPrintPosition = 0;
    }

    std::cout.setf(std::ios::hex, std::ios::basefield);
    // fragment id
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    if (_fragmentID < 16) {
        std::cout << "0";
    }
    std::cout << (unsigned)_fragmentID;
    SetConsoleTextAttribute(consoleHandle, 7);
    //std::cout << " ";
    // fragment data type
    SetConsoleTextAttribute(consoleHandle, 6);
    /*if (_dataType._dataType < 16) {
        std::cout << "0";
    }
    std::cout << (unsigned)_dataType._dataType;*/
    command.printCommandInOneString(_offset - _dataType._headerSize, _offset, false);
    SetConsoleTextAttribute(consoleHandle, 7);
    //std::cout << " ";
    //std::cout << "dataType size: " << (unsigned)_dataType._size << " ";
    // fragment payload
    SetConsoleTextAttribute(consoleHandle, 7 | FOREGROUND_INTENSITY);

    /*if (_dataType._dataType == dataType::int8_string) {
        for (size_t i = _offset; i < endOffset; i++) {
            std::cout << (unsigned char)command[i];
        }
    }
    else {
        command.printCommandInOneString(_offset, endOffset, false);
    }*/
    command.printCommandInOneString(_offset, endOffset, false);
    SetConsoleTextAttribute(consoleHandle, 7);
    std::cout << " ";
    //std::cout << fragmentLength << "\n";
    std::cout.unsetf(std::ios::hex);
}


// **************************************************************************
// ============================== DataLayout ================================
// **************************************************************************


DataFragment DataLayout::findFragment(uint8_t fragmentID) const
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
            std::cout << "offset: " << std::hex << offset << std::dec << " " << offset << "\n";
            command.printCommandInOneString();
            SetConsoleTextAttribute(consoleHandle, 6);
        }
        //std::cout << "start offset: " << offset << " ";
        if (offset < commandSize) {
            fragmentID = command[offset];
            //std::cout << "fragmentID: " << (unsigned)command[offset] << " ";
            offset += 1;
            dataType = command[offset];

            numOfEntries = DataFragment::findNumOfEntries(command, dataType, offset);
            /*if (dataType == dataType::int16) {
                std::cout << "numOfEntries: " << numOfEntries << " ";
            }*/
            
            dataTypeHeaderSize = 0;
            if (dataType == dataType::dictionary) {
                offset += 3;
                dataTypeHeaderSize += 3;
                dataType = command[offset];

                //processDictionary(command, fragmentID, offset, numOfEntries, dataTypeHeaderSize, 0);
                if (numOfEntries == 0) {
                    offset += 1;
                    dataTypeHeaderSize += 1;
                    sizeOfData = 0;

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        sizeOfData,
                        DataType(1, dataTypeHeaderSize, dataType::int8_string)
                    );

                    _dataLayout.push_back(dataFragment);
                }
                else if (dataType == dataType::dictionary) {
                    uint16_t nestedNumOfEntries = DataFragment::findNumOfEntries(
                        command, dataType, offset);

                    offset += 3;
                    dataTypeHeaderSize += 3;
                    dataType = command[offset];
                    if (dataType == dataType::float32) {
                        dataTypeHeaderSize += DataType::getDataTypeHeaderSize(dataType);
                        dataTypeSize = DataType::getDataTypeSize(dataType);
                        offset += DataType::getDataTypeHeaderSize(dataType);
                        for (size_t i = 0; i < numOfEntries; i++) {
                            dataFragment = DataFragment(
                                fragmentID,
                                offset,
                                nestedNumOfEntries,
                                DataType(dataTypeSize,
                                    i == 0 ? dataTypeHeaderSize : 3, dataType::int8_string)
                            );

                            _dataLayout.push_back(dataFragment);

                            sizeOfData = dataTypeSize * nestedNumOfEntries;
                            if (i > 0) {
                                sizeOfData += 3;
                            }
                            offset += sizeOfData;
                        }
                    }
                }
                else if (dataType == dataType::int8_list || dataType == dataType::int8_string) {
                    uint16_t nestedNumOfEntries = DataFragment::findNumOfEntries(command, dataType, offset);
                    uint8_t nestedDataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                    dataTypeHeaderSize += DataType::getDataTypeHeaderSize(dataType);

                    offset += 1;
                    for (size_t i = 0; i < numOfEntries; i++) {
                        offset += nestedDataTypeHeaderSize - 1;
                        sizeOfData = DataFragment::findNumOfEntries(command, dataType, offset - nestedDataTypeHeaderSize);

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
                            "fragment id: "    << (unsigned)fragmentID           << " " <<
                            "data type: "      << (unsigned)dataType             << " " <<
                            std::dec <<
                            "size: "           << (unsigned)dataTypeSize         << " " <<
                            "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                            "num of entries: " << (unsigned)numOfEntries         << " " <<
                            "size of data: "   << (unsigned)sizeOfData           << "\n";*/
                    }
                }
                else {
                    dataTypeHeaderSize += DataType::getDataTypeHeaderSize(dataType);
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
                    "fragment id: "    << (unsigned)fragmentID           << " " <<
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
void DataLayout::processDictionary(NetworkCommand& command, uint8_t& fragmentID, ptrdiff_t& offset, 
    uint16_t numOfEntries, uint8_t& dataTypeHeaderSize, size_t index)
{
    uint8_t dataType = 0;
    uint16_t sizeOfData = 0;
    uint16_t nestedNumOfEntries = 0;
    uint8_t nestedDataType = 0;

    //std::cout << "offset: " << offset << "\n";
    dataType = command[offset];
    if (numOfEntries == 0) {
        offset += 1;
        dataTypeHeaderSize += 1;
        nestedNumOfEntries = 0;
    }
    else {
        dataTypeHeaderSize += DataType::getDataTypeHeaderSize(dataType);
        nestedNumOfEntries = DataFragment::findNumOfEntries(command, dataType, offset);
        offset += DataType::getDataTypeHeaderSize(dataType);
        nestedDataType = command[offset];
    }

    if (dataType == dataType::dictionary) {
        if (dataType == dataType::int8_list || 
            dataType == dataType::int8_string) {
            for (size_t i = 0; i < numOfEntries; i++) {
                //std::cout << numOfEntries << nestedNumOfEntries << "\n";
                processDictionary(command, fragmentID, offset, nestedNumOfEntries, dataTypeHeaderSize, i);
            }
        }    
        else {
            processDictionary(command, fragmentID, offset, nestedNumOfEntries, dataTypeHeaderSize, 0);
        }
    }
    else if (dataType == dataType::int8_list || dataType == dataType::int8_string) {
        uint8_t dataTypeSize = DataType::getDataTypeSize(dataType);
        if (dataType == dataType::int8_string and numOfEntries > 0 and index > 0) {
            offset += 2;
            nestedNumOfEntries = DataFragment::findNumOfEntries(command, dataType, offset - 3);
            //std::cout << "nestedNumOfEntries: " << std::hex << nestedNumOfEntries << std::dec << "\n";
        }
        else if (dataType == dataType::int8_string and numOfEntries == 0) {
            nestedNumOfEntries = 0;
        }

        /*std::cout <<
            "offset: "         << (unsigned)offset             << " " <<
            std::hex <<
            "fragment id: "    << (unsigned)fragmentID         << " " <<
            "data type: "      << (unsigned)dataType           << " " <<
            std::dec <<
            "size: "           << (unsigned)dataTypeSize       << " " <<
            "header size: "    << (unsigned)dataTypeHeaderSize << " " <<
            "num of entries: " << (unsigned)numOfEntries       << " " <<
            "size of data: "   << (unsigned)sizeOfData         << "\n";*/
    }
    else if (dataType == dataType::float32) {
        uint8_t dataTypeSize = 4;

        DataFragment dataFragment = DataFragment(
            fragmentID,
            offset,
            numOfEntries,
            DataType(dataTypeSize, dataTypeHeaderSize, dataType)
        );

        sizeOfData = dataTypeSize * numOfEntries + 3 * (numOfEntries - 1);
        offset += sizeOfData;
        _dataLayout.push_back(dataFragment);
    }
    else {
        uint8_t dataTypeSize = DataType::getDataTypeSize(dataType);
        //offset += DataType::getDataTypeHeaderSize(dataType);

        DataFragment dataFragment = DataFragment(
            fragmentID,
            offset,
            numOfEntries,
            DataType(dataTypeSize, dataTypeHeaderSize, dataType)
        );

        sizeOfData = dataTypeSize * nestedNumOfEntries;
        offset += sizeOfData;
        _dataLayout.push_back(dataFragment);

        /*std::cout <<
            "offset: "         << (unsigned)dataFragment._offset     << " " <<
            std::hex <<
            "fragment id: "    << (unsigned)fragmentID << " " <<
            "data type: "      << (unsigned)dataType                 << " " <<
            std::dec <<
            "size: "           << (unsigned)dataTypeSize             << " " <<
            "header size: "    << (unsigned)dataTypeHeaderSize       << " " <<
            "num of entries: " << (unsigned)numOfEntries             << " " <<
            "size of data: "   << (unsigned)sizeOfData               << "\n";*/
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

void DataLayout::printInfo(NetworkCommand& command) const
{
    // num of fragments
    uint16_t numOfFragmensOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout << 
        "num of fragments: " << (unsigned)command[numOfFragmensOffset] << " " <<
        "event code: " << command.getEventCode() << "\n";
    size_t currentPrintPosition = 0;
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayout[i].printFragmentInfo(command, currentPrintPosition);

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

size_t DataLayout::size()
{
    return _dataLayout.size();
}
DataFragment DataLayout::operator[](size_t elementIndex)
{
    return _dataLayout[elementIndex];
}
