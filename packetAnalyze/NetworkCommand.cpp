#include "pch.h"

enum operationType
{
    operationRequest = 2,
    operationResponse = 3,
    event = 4
};
enum commandType
{
    reliable = 6,
    unreliable = 7,
    fragmented = 8
};
enum operationCodes {
    uctionSellOrders = 76,
    AuctionBuyOrders = 77,
    AuctionAverageValues = 90,
    GetCharacterEquipment = 137

};
enum eventCodes {
    ActiveSpellEffectsUpdate = 10,
    HarvestableObjectList = 35,
    HarvestableObject = 36,
    HarvestStart = 55,
    HarvestFinished = 57,
    EquipmentChanged = 85,
    MobSpawn = 117,
};

std::vector<size_t> nnCodes = { 1,10,35,36,55,57,66,67,85,195 };
std::vector<size_t> nCodes = { 117 };

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
    if (isItemInVector(nCodes, _eventCode) and _operationType == operationType::event) {
        //std::cout << _eventCode << " " << _networkCommand.size() << "\n";
        //this->printCommand(), std::cout << "\n";
        //this->printCommandInOneString(), std::cout << "\n";
    }
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
    std::cout.unsetf(std::ios::hex);
}

void NetworkCommand::fillFragmentedCommand(NetworkCommand command)
{
    if (command.getCommandType() == commandType::fragmented)
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
uint16_t NetworkCommand::findEventCode()
{
    if (_isCommandFull == true) {
        return _networkCommand[_networkCommand.size() - 1];
    }
};