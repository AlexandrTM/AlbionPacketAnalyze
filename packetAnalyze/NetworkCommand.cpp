#include "pch.h"

// lenghts for event code 36
//"100" 2
//"106" 3
//"99" 1
//"70" 10
//"64" 1
//"67" 3

std::vector<size_t> nnCodes = {};
std::vector<size_t> nCodes = {};

NetworkCommand::NetworkCommand(std::vector<uint8_t> command)
{
    _networkCommand = command;
    _commandType = findCommandType();
    _operationType = findOperationType();
    _isCommandFull = isCommandFull();
    _eventCode = findEventCode();
}
NetworkCommand::NetworkCommand() 
{
    _networkCommand = {};
    _commandType = 0;
    _operationType = 0;
    _isCommandFull = false;
    _eventCode = 0;
}

void NetworkCommand::analyzeCommand()
{
    HarvestableList _harvestableList{};
    if (_operationType == operationType::event) {
        if (_eventCode == eventCode::harvestableObjectList) {
            _harvestableList += HarvestableList(*this);
            //_harvestableList.printInfo();
        }
        if (_eventCode == eventCode::harvestableObject) {
            this->findDataFragments();
            _harvestableList.push_back(Harvestable(*this));
            //_harvestableList.printInfo();
            //this->printCommandInOneString();
        }
        //this->printCommand(), std::cout << "\n";
        //this->printCommandInOneString(), std::cout << "\n";
    }
    //_harvestableList.clear();
}
void NetworkCommand::findDataFragments()
{
    ptrdiff_t _offset = findFragmentsNumOffset();
    uint8_t _fragmentsNum = _networkCommand[_offset];

    uint8_t _fragmentID;
    uint8_t _dataType;
    uint8_t _dataTypeSize;
    uint8_t _dataHeaderSize;
    uint16_t _numOfEntries;

    uint16_t _dataSize;

    for (size_t i = 0; i < _fragmentsNum - 1; i++) { // excluding event code
        _fragmentID = _networkCommand[_offset + 1];
        _dataType = _networkCommand[_offset + 2];

        DataTypeInfo _dataTypeInfo = DataTypeInfo::findDataTypeInfo(_dataType, *this, _offset);

        _dataSize = _dataTypeInfo._dataHeaderSize + 
                    _dataTypeInfo._dataTypeSize * _dataTypeInfo._numOfEntries + 1;
        _offset += _dataSize;

        _dataTypeInfo.printInfo();

        //std::cout << (unsigned)_fragmentID << " " << (unsigned)_dataType << " " << (unsigned)_dataSize << "\n";
    }
    this->printCommandInOneString();
}
DataTypeInfo DataTypeInfo::findDataTypeInfo(uint8_t dataType, NetworkCommand& command, ptrdiff_t offset)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return DataTypeInfo(1, 1, 1);
    case dataType::byteIntList:
        return DataTypeInfo(1, 5, (command[offset + 5] << 8) |
                                   command[offset + 6]);
    case dataType::shortInt:
        return DataTypeInfo(2, 1, 1);
    case dataType::int32:
        return DataTypeInfo(4, 1, 1);
    case dataType::float32:
        return DataTypeInfo(4, 1, 1);
    case dataType::int64:
        return DataTypeInfo(8, 1, 1);
    case dataType::dictionary:
        return DataTypeInfo(DataTypeInfo::findDataTypeSize(command[offset + 5]), 4, (command[offset + 3] << 8) |
                                                                                     command[offset + 4]);
    default:
        break;
    }
}
uint8_t DataTypeInfo::findDataTypeSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::byteInt:
        return 1;
    case dataType::byteIntList:
        return 1;
    case dataType::shortInt:
        return 2;
    case dataType::float32:
        return 4;
    case dataType::int32:
        return 4;
    case dataType::int64:
        return 8;
    default:
        break;
    }
}
ptrdiff_t NetworkCommand::findFragmentsNumOffset()
{
    switch (this->getCommandType())
    {
    case commandType::reliable:
        return 16;
    case commandType::unreliable:
        return 20;
    case commandType::fragmented:
        return 36;
    default:
        return -1;
    }
}
DataTypeInfo::DataTypeInfo(uint8_t dataTypeSize, uint8_t dataHeaderSize, uint16_t numOfEntries)
{
    _dataTypeSize = dataTypeSize;
    _dataHeaderSize = dataHeaderSize;
    _numOfEntries = numOfEntries;
}
void DataTypeInfo::printInfo()
{
    std::cout << (unsigned)_dataTypeSize << " " << (unsigned)_dataHeaderSize << " " << (unsigned)_numOfEntries << "\n";
}

bool NetworkCommand::isItemInVector(std::vector<size_t>& vector, size_t item)
{
    if ((std::find(std::begin(vector), std::end(vector), item) != std::end(vector))) {
        return true;
    }
    else {
        return false;
    }
}

void NetworkCommand::printCommand()
{
    std::cout.setf(std::ios::hex, std::ios::basefield);
    for (size_t i = 0; i < _networkCommand.size(); i++)
    {
        if (_networkCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(_networkCommand[i]);
        if (i % 4 == 3)
            std::cout << " ";
        if (i % 32 == 31 and i != (_networkCommand.size() - 1))
            std::cout << "\n";
    }
    std::cout.unsetf(std::ios::hex);
}
void NetworkCommand::printCommand(size_t regionStart, size_t regionEnd)
{
    std::cout.setf(std::ios::hex, std::ios::basefield);
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        if (_networkCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(_networkCommand[i]);
        if ((i - regionStart) % 4 == 3)
            std::cout << " ";
        if ((i - regionStart) % 32 == 31 and i != (_networkCommand.size() - regionStart - 1))
            std::cout << "\n";
    }
    std::cout.unsetf(std::ios::hex);
}
void NetworkCommand::printCommandInOneString()
{
    std::cout.setf(std::ios::hex, std::ios::basefield);
    for (size_t i = 0; i < _networkCommand.size(); i++)
    {
        if (_networkCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(_networkCommand[i]);
    }
    std::cout << "\n";
    std::cout.unsetf(std::ios::hex);
}
void NetworkCommand::printCommandInOneString(size_t regionStart, size_t regionEnd)
{
    std::cout.setf(std::ios::hex, std::ios::basefield);
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        if (_networkCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(_networkCommand[i]);
    }
    std::cout << "\n";
    std::cout.unsetf(std::ios::hex);
}

bool NetworkCommand::isCommandFull()
{
    switch (_commandType)
    {
    case commandType::reliable:
        return true;
    case commandType::unreliable:
        return true;
    case commandType::fragmented:
        return _isCommandFull;
    default:
        return false;
    }
}
void NetworkCommand::fillFragmentedCommand(NetworkCommand command)
{
    if (command.isLastCommandInChain()) {
        *this += command;
        _commandType = commandType::fragmented;
        _operationType = findOperationType();
        _isCommandFull = true;
        _eventCode = findEventCode();
    }
    else {
        *this += command;
    }
}
bool NetworkCommand::isLastCommandInChain()
{
    uint8_t _commandsNumInChain = _networkCommand[19];
    uint8_t _commandIndexInChain = _networkCommand[23];
    if (_commandsNumInChain == _commandIndexInChain + 1) {
        return true;
    }
    else {
        return false;
    }
}

uint8_t NetworkCommand::getCommandType() 
{ 
    return _commandType;
}
uint8_t NetworkCommand::getOperationType() 
{
    return _operationType;
}
uint16_t NetworkCommand::getEventCode() 
{ 
    return _eventCode;
}

void NetworkCommand::push_back(uint8_t element)
{
    _networkCommand.push_back(element);
}
uint16_t NetworkCommand::size()
{
    return _networkCommand.size();
}
void NetworkCommand::clear()
{
    _networkCommand.clear();
    _commandType = 0;
    _operationType = 0;
    _eventCode = 0;
    _isCommandFull = false;
}

uint8_t& NetworkCommand::operator[](size_t elementIndex)
{
    return _networkCommand[elementIndex];
}
const uint8_t& NetworkCommand::operator[](size_t elementIndex) const
{
    return _networkCommand[elementIndex];
}
NetworkCommand& NetworkCommand::operator+=(NetworkCommand command)
{
    for (size_t i = 0; i < command.size(); i++) {
        _networkCommand.push_back(command[i]);
    }
    return *this;
}

uint8_t NetworkCommand::findCommandType()
{
    if (_networkCommand.size() > 0) {
        return _networkCommand[0];
    }
    else {
        return 0;
    }
}
uint8_t NetworkCommand::findOperationType()
{
    switch (_commandType)
    {
    case commandType::reliable:
        return _networkCommand[13];
    case commandType::unreliable:
        return _networkCommand[17];
    case commandType::fragmented:
        return _networkCommand[33];
    default:
        return 0;
    }
}
uint16_t NetworkCommand::findEventCode()
{
    if (_isCommandFull == true) {
        if (_networkCommand[_networkCommand.size() - 3] == dataType::shortInt) {
            return ((_networkCommand[_networkCommand.size() - 2] << 8) |
                        _networkCommand[_networkCommand.size() - 1]) & 0x0fff;
        }
        else {
            return _networkCommand[_networkCommand.size() - 1];
        }
    }
    else {
        return 0;
    }
};
