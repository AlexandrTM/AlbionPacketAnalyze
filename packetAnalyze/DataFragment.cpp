#include "pch.h"
#include "DataFragment.h"

ptrdiff_t DataFragment::findFragmentsNumOffset(NetworkCommand& command)
{
    switch (command.getCommandType()) {
    case commandType::reliable:
        if (command.getOperationType() == operationType::response ||
            command.getOperationType() == operationType::binary) {
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
        std::cout << "command type not defined: " << command.getCommandType() << "\n";
        command.printCommandInOneString();
        return -1;
    }
}

DataFragment::DataFragment(uint8_t fragmentID, ptrdiff_t offset, uint16_t numOfEntries, DataType dataType)
{
    _fragmentId = fragmentID;
    _offset = offset;
    _numOfEntries = numOfEntries;
    _dataType = dataType;
}
DataFragment::DataFragment()
{
    _fragmentId = 0;
    _offset = std::numeric_limits<ptrdiff_t>::min();
    _numOfEntries = 0;
    _dataType = {};
}
void DataFragment::printInfo(NetworkCommand& command) const
{
    size_t commandSizeLength = std::to_string(command.size()).length();
    std::cout << "id: " << std::setw(commandSizeLength) << (unsigned)_fragmentId << " "
        << "of: " << std::setw(commandSizeLength) << (unsigned)_offset << " ";
}

void DataFragment::printFragmentInfo(NetworkCommand& command, size_t& currentStringPosition,
    bool printPayload) const
{
    // take dicionaty into account
    ptrdiff_t startOffset = _offset - _dataType._headerSize - 1;
    ptrdiff_t endOffset = _offset + _dataType._size * _numOfEntries;
    size_t fragmentLength = endOffset - startOffset + 3;

    /*std::cout <<
        "data type:" << (unsigned)_dataType._dataType << " " <<
        "size: " << (unsigned)_dataType._size << " " <<
        "header size: " << (unsigned)_dataType._headerSize << " " <<
        "offset: " << (unsigned)_offset << " " <<
        "num of entries: " << (unsigned)_numOfEntries << "\n";*/

    currentStringPosition += fragmentLength;
    if (currentStringPosition > 50) {
        std::cout << "\n";
        currentStringPosition = 0;
    }

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);

    std::cout << "dec " << (unsigned)_fragmentId << " ";

    std::cout.setf(std::ios::hex, std::ios::basefield);
    // fragment id
    if (_fragmentId < 16) {
        std::cout << "0";
    }
    std::cout << (unsigned)_fragmentId;
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
    if (printPayload) {
        SetConsoleTextAttribute(consoleHandle, 7 | FOREGROUND_INTENSITY);

        /*if (_dataType._dataType == dataType::int8_string) {
            for (size_t i = _offset; i < endOffset; i++) {
                std::cout << (unsigned char)command[i];
            }
        }
        else {
            command.printCommandInOneString(_offset, endOffset, false);
        }*/
        if (_dataType._dataTypeId == dataType::float32) {
            std::cout << " " << net::read_float32(command, _offset);
        }
        /*else if (_dataType._dataTypeId == dataType::int8_string) {
            std::cout.unsetf(std::ios::hex);
            for (size_t i = _offset; i < endOffset; i++) {
                std::cout << command[i];
            }
            std::cout.setf(std::ios::hex, std::ios::basefield);
        }*/
        else {
            command.printCommandInOneString(_offset, endOffset, false);
        }
        SetConsoleTextAttribute(consoleHandle, 7);
    }
    std::cout << " ";
    //std::cout << fragmentLength << "\n";
    std::cout.unsetf(std::ios::hex);
}