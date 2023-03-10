#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H


class NetworkCommand
{
public:
    NetworkCommand(std::vector<uint8_t> command);
    NetworkCommand();

    bool isLastCommandInChain();

    uint8_t getCommandType();
    uint8_t getOperationType();
    uint16_t getEventCode();
    void setEventCode(uint16_t eventCode);

    void push_back(uint8_t element);
    uint16_t size();
    void clear();

    uint8_t& operator[](size_t elementIndex);
    const uint8_t& operator[](size_t elementIndex) const;
    NetworkCommand& operator+=(NetworkCommand command);
private:
    uint16_t findEventCode();
    uint8_t findCommandType();
    uint8_t findOperationType();
    uint8_t _commandType;
    uint8_t _operationType;
    uint16_t _eventCode;

    std::vector<uint8_t> _networkCommand;
};

#endif 