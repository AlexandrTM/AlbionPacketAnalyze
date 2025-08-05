#include "pch.h"
#include "Enumerations.h"
//#include "global_data.h"


#ifndef NETWORK_COMMAND_H
#define NETWORK_COMMAND_H

class NetworkCommand
{
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

public:
    NetworkCommand(std::vector<uint8_t> rawCommand);
    NetworkCommand(NetworkCommand& command, size_t beginOffset);
    NetworkCommand();

    void printCommand();
    void printCommand(size_t regionStart, size_t regionEnd);
    static void printCommandInOneString(
        std::vector<uint8_t>& rawCommand, 
        size_t regionStart, size_t regionEnd,
        bool lineBreak = true, bool isHex = true
    );
    static void printCommandInOneString(
        std::vector<uint8_t>& rawCommand,
        bool isLineBreak = true, bool isHex = true
    );
    void printCommandInOneString(bool isLineBreak = true, bool isHex = true);
    void printCommandInOneString(
        size_t regionStart, size_t regionEnd, 
        bool isLineBreak = true, bool isHex = true
    );

    void endFragmentedCommand();
    uint32_t findCommandID(std::vector<uint8_t>& rawCommand) const;

    uint8_t getCommandType() const;
    uint8_t getOperationType() const;
    uint16_t getEventCode() const;
    uint32_t getCommandID() const;
    uint8_t getCommandIndexInChain() const;

    std::vector<uint8_t>& rawNetworkCommand();
    uint16_t size() const;
    void push_back(uint8_t element);
    void clear();

    uint8_t& operator[](size_t elementIndex);
    const uint8_t& operator[](size_t elementIndex) const;
    NetworkCommand& operator+=(NetworkCommand command);
    bool operator!=(NetworkCommand& command);
    bool operator==(NetworkCommand& command);

    using iterator = std::vector<uint8_t>::iterator;
    using const_iterator = std::vector<uint8_t>::const_iterator;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

};

#endif 
