#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H

class NetworkCommand
{
public:
    NetworkCommand(std::vector<uint8_t> rawCommand);
    NetworkCommand(NetworkCommand& command, size_t regionStart);
    NetworkCommand();

    void analyzeCommand(GLFWwindow* window);

    void printCommand();
    void printCommand(size_t regionStart, size_t regionEnd);
    void printCommandInOneString(bool lineBreak = true, bool isHex = true);
    void printCommandInOneString(size_t regionStart, size_t regionEnd, 
        bool lineBreak = true, bool isHex = true);

    void endFragmentedCommand();
    uint32_t findCommandID(std::vector<uint8_t>& rawCommand) const;

    uint8_t getCommandType() const;
    uint8_t getOperationType() const;
    uint16_t getEventCode() const;
    uint32_t getCommandID() const;
    uint8_t getCommandIndexInChain() const;

    uint16_t size();
    void push_back(uint8_t element);
    void clear();

    uint8_t& operator[](size_t elementIndex);
    const uint8_t& operator[](size_t elementIndex) const;
    NetworkCommand& operator+=(NetworkCommand command);
    bool operator!=(NetworkCommand& command);
    bool operator==(NetworkCommand& command);

private:
    std::vector<uint8_t> _networkCommand;

    uint8_t _commandType;
    uint8_t _operationType;
    uint16_t _eventCode;

    uint32_t _commandID;
    uint8_t _commandIndexInChain;

    uint8_t findCommandIndexInChain(std::vector<uint8_t>& rawCommand) const;
    uint8_t findCommandType(std::vector<uint8_t>& rawCommand) const;
    uint8_t findOperationType(std::vector<uint8_t>& rawCommand) const;
    uint16_t findEventCode(std::vector<uint8_t>& rawCommand) const;

};

#endif 