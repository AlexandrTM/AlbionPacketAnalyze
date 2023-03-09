#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H


class NetworkCommand
{
public:
    NetworkCommand(std::vector<uint8_t> command);

    uint8_t getCommandType();
    uint8_t getOperationType();
    uint16_t getEventCode();

    size_t size();

    uint8_t& operator[](size_t elementNumber);
private:
    uint8_t findCommandType();
    uint8_t findOperationType();
    uint16_t findEventCode();

    uint16_t _commandLenght;
    uint8_t _commandType;
    uint8_t _operationType;
    uint16_t _eventCode;

    std::vector<uint8_t> _networkCommand;
};

#endif 