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
    _commandLenght = _networkCommand.size();
    _commandType = findCommandType();
    _operationType = findOperationType();
    _eventCode = findEventCode();
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

size_t NetworkCommand::size()
{
    return _commandLenght;
}

uint8_t& NetworkCommand::operator[](size_t elementNumber)
{
    return _networkCommand[elementNumber];
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
        //return ((_networkCommand[_commandLenght - 2] << 8) + _networkCommand[_commandLenght - 1]) & 0x0fff;
        return _networkCommand[_commandLenght - 1];
    case commandType::unreliable:
        return _networkCommand[_commandLenght - 1];
    case commandType::fragmented:
        //return (_networkCommand[_commandLenght - 2] << 8) + _networkCommand[_commandLenght - 1];
    default:
        return 0;
    }
};