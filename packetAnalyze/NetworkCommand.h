#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H

class NetworkCommand
{
private:
    std::vector<uint8_t> _networkCommand;

    uint8_t _commandType;
    uint8_t _operationType;
    uint16_t _eventCode;

    bool _isCommandFull;
    int16_t _indexOfLastCommandInChain;
    uint32_t _commandChainID;

    uint8_t findCommandType();
    uint8_t findOperationType();
    uint16_t findEventCode();

public:
    NetworkCommand(std::vector<uint8_t> command);
    NetworkCommand(NetworkCommand& command, size_t regionStart);
    NetworkCommand();

    void analyzeCommand(GLFWwindow* window);

    void printCommand();
    void printCommand(size_t regionStart, size_t regionEnd);
    void printCommandInOneString();
    void printCommandInOneString(size_t regionStart, size_t regionEnd);

    void fillFragmentedCommand(NetworkCommand command);
    uint32_t findCommandChainID();
    bool isLastCommandInChain();
    bool isFirstCommandInChain();
    bool isNextCommandInChain(NetworkCommand& command);
    bool isCommandFull();

    uint8_t getCommandType();
    uint8_t getOperationType();
    uint16_t getEventCode();

    uint16_t size();
    void push_back(uint8_t element);
    void clear();

    uint8_t& operator[](size_t elementIndex);
    const uint8_t& operator[](size_t elementIndex) const;
    NetworkCommand& operator+=(NetworkCommand command);
    bool operator!=(NetworkCommand& command);
    bool operator==(NetworkCommand& command);

};

#endif 