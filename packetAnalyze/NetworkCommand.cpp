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

NetworkCommand::NetworkCommand(std::vector<uint8_t> command)
{
    _networkCommand = command;
    _commandType = findCommandType();
    _operationType = findOperationType();
    _eventCode = findEventCode();
}

NetworkCommand::NetworkCommand() 
{
    _networkCommand = {};
    _commandType = 0;
    _operationType = 0;
    _eventCode = 0;
}

bool NetworkCommand::isLastCommandInChain()
{
    uint8_t _commandsNumInChain = _networkCommand[19];
    uint8_t _commandIndexInChain = _networkCommand[23];
    //std::cout << (unsigned)_commandsNumInChain << " " << (unsigned)_commandIndexInChain << "\n";
    if (_commandsNumInChain == _commandIndexInChain + 1) {
        _eventCode = _networkCommand[_networkCommand.size() - 1];
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

void NetworkCommand::setEventCode(uint16_t eventCode)
{
    _eventCode = eventCode;
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
uint16_t NetworkCommand::findEventCode()
{
    switch (_commandType)
    {
    case commandType::reliable:
        //return ((_networkCommand[_networkCommand.size() - 2] << 8) + _networkCommand[_networkCommand.size() - 1]) & 0x0fff;
        return _networkCommand[_networkCommand.size() - 1];
    case commandType::unreliable:
        return _networkCommand[_networkCommand.size() - 1];
    case commandType::fragmented:
        if (isLastCommandInChain()) {
            return _networkCommand[_networkCommand.size() - 1];
        }
    default:
        return 0;
    }
};