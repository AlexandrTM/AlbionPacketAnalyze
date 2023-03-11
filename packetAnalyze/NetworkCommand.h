#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H


class NetworkCommand
{
private:
    uint8_t findCommandType();
    uint8_t findOperationType();
    uint16_t findEventCode();

    uint8_t _commandType;
    uint8_t _operationType;
    uint16_t _eventCode;
    bool _isCommandFull;

    std::vector<uint8_t> _networkCommand;

public:
    NetworkCommand(std::vector<uint8_t> command);
    NetworkCommand();

    void analyzeCommand();
    bool isItemInVector(std::vector<size_t>& vector, size_t item);

    void printCommand();
    void printCommand(size_t regionStart, size_t regionEnd);
    void printCommandInOneString();
    void printCommandInOneString(size_t regionStart, size_t regionEnd);

    void fillFragmentedCommand(NetworkCommand command);
    bool isLastCommandInChain();
    bool isCommandFull();

    uint8_t getCommandType();
    uint8_t getOperationType();
    uint16_t getEventCode();

    uint16_t size();
    void push_back(uint8_t element);
    void clearCommand();

    uint8_t& operator[](size_t elementIndex);
    const uint8_t& operator[](size_t elementIndex) const;
    NetworkCommand& operator+=(NetworkCommand command);
};

#endif 