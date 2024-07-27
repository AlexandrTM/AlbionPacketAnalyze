#include "pch.h"

std::vector<size_t> nnCodes = {};
std::vector<size_t> nCodes = {};

NetworkCommand::NetworkCommand(std::vector<uint8_t> rawCommand)
{
    _networkCommand = rawCommand;
    _commandType = findCommandType(rawCommand);
    _operationType = findOperationType(rawCommand);
    _isCommandFull = isCommandFull();
    _eventCode = findEventCode(rawCommand);
    _indexOfLastCommandInChain = 0;
    _commandChainID = findCommandChainID(rawCommand);
}
NetworkCommand::NetworkCommand(NetworkCommand& command, size_t regionStart)
{
    _networkCommand = { command._networkCommand.begin() + regionStart, command._networkCommand.end() };
    _commandType = findCommandType(command._networkCommand);
    _operationType = findOperationType(command._networkCommand);
    _isCommandFull = isCommandFull();
    _eventCode = findEventCode(command._networkCommand);
    _indexOfLastCommandInChain = 0;
    _commandChainID = findCommandChainID(command._networkCommand);
}
NetworkCommand::NetworkCommand() 
{
    _networkCommand = {};
    _commandType = 0;
    _operationType = 0;
    _eventCode = 0;
    _isCommandFull = false;
    _indexOfLastCommandInChain = 0;
    _commandChainID = 0;
}

EntityList _entityList{};
size_t counter = 0;
void NetworkCommand::analyzeCommand(GLFWwindow* window)
{
    //this->printCommandInOneString();
    if (_operationType == operationType::event) {
        switch (_eventCode)
        {
        case eventCode::harvestableObjectList:
            _entityList._harvestableList.update(HarvestableList(*this)); 
            break;
        case eventCode::harvestableObject:
            _entityList._harvestableList.update(Harvestable(*this)); 
            break;
        case eventCode::harvestableChangeState:
            _entityList._harvestableList.updateState(*this); 
            break;
        case eventCode::newPlayer:
            _entityList._playerList.newPlayer(Player(*this)); 
            break;
        case eventCode::playerLeave:
           _entityList._playerList.playerLeave(Player(*this)); 
           break;
        case 65:
            _entityList._playerList.update(Player::playerMove(*this)); 
            break;
        case 64:
            _entityList._playerList.update(Player::playerMove(*this)); 
            break;
        default:
            break;
        }
        if (_networkCommand.size() == 67) {
            _entityList._playerList.update(Player::playerMove(*this));
        }
    }
    if (_operationType == operationType::operationResponse) {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point stop;
        //std::cout << _eventCode << "\n";
        switch (_eventCode) 
        {
        case operationCode::move:
            //_entityList._player = PlayerSelf(*this);
            break;
        case operationCode::changeLocation:
            //_entityList.changeLocation(); 
            break;
        case operationCode::auctionSellOrders:
            //this->printCommandInOneString();
            break;
        case operationCode::auctionBuyOrders:
            break;
        case operationCode::auctionGetFinishedOrders:
            break;
        case operationCode::auctionAverageValues:
            //start = std::chrono::high_resolution_clock::now();
            //Auction::findAuctionAverageValues(*this);
            //counter += 1;
            //std::cout << counter << "\n";
            //stop = std::chrono::high_resolution_clock::now();
            //std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
            break;
        case 88:
            //this->printCommandInOneString();
            break;
        default:
            break;
        }
        //std::cout << this->getEventCode() << "\n";
        //this->printCommandInOneString();
    }
    if (_operationType == operationType::operationRequest) {
        switch (_eventCode)
        {
        case operationCode::move:
            //_entityList._player.update(*this);
            //std::cout << _entityList._player._positionX << " " << _entityList._player._positionY << "\n";
            break;
        case operationCode::auctionBuyOrders:
            /*DataLayout dataLayout;
            dataLayout.findDataLayout(*this);
            if (dataLayout.size() == 14) {
                Auction::findProductName(*this, dataLayout);
            }*/
            break;
        }
            
    }
    //std::cout << _entityList._harvestableList.size() << "\n";
    //_entityList.draw(window);
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


void NetworkCommand::fillFragmentedCommand(NetworkCommand command)
{
    /*if (command.isFirstCommandInChain()) {
        *this += command;
    }
    else */if (command.isNextCommandInChain(*this)) {
        *this += NetworkCommand(command, 32);
        _indexOfLastCommandInChain += 1;

        if (command.isLastCommandInChain()) {
            _commandType = commandType::fragmented;
            _operationType = findOperationType(command._networkCommand);
            _isCommandFull = true;
            _eventCode = findEventCode(command._networkCommand);
            _indexOfLastCommandInChain = 0;
        }
    }
}
uint32_t NetworkCommand::findCommandChainID(std::vector<uint8_t>& rawCommand) const
{
    if (_commandType == commandType::fragmented) {
        return net::read_uint32(rawCommand, 12);
    }
    else {
        return 0;
    }
}
bool NetworkCommand::isLastCommandInChain()
{
    uint8_t commandsNumInChain = _networkCommand[19];
    uint8_t commandIndexInChain = _networkCommand[23];
    if (commandsNumInChain == commandIndexInChain + 1) {
        return true;
    }
    else {
        return false;
    }
}
bool NetworkCommand::isFirstCommandInChain()
{
    uint8_t commandIndexInChain = _networkCommand[23];
    if (commandIndexInChain == 0) {
        return true;
    }
    else {  
        return false;
    }
}
bool NetworkCommand::isNextCommandInChain(NetworkCommand& command)
{
    uint32_t commandChainID = net::read_uint32(_networkCommand, 12);
    uint8_t commandIndexInChain = _networkCommand[23];
    if (command._indexOfLastCommandInChain + 1 == commandIndexInChain and
        command._commandChainID == commandChainID) {
        return true;
    }
    else {
        return false;
    }
}
bool NetworkCommand::isCommandFull() const
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

uint32_t NetworkCommand::getCommandChainID() const
{
    return this->_commandChainID;
}
uint8_t NetworkCommand::getCommandType() const 
{ 
    return _commandType;
}
uint8_t NetworkCommand::getOperationType() const 
{
    return _operationType;
}
uint16_t NetworkCommand::getEventCode() const 
{ 
    return _eventCode;
}

void NetworkCommand::push_back(uint8_t element)
{
    _networkCommand.push_back(element);
}
uint16_t NetworkCommand::size()
{
    return static_cast<uint16_t>(_networkCommand.size());
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
bool NetworkCommand::operator!=(NetworkCommand& command)
{
    return !(_networkCommand == command._networkCommand);
}
bool NetworkCommand::operator==(NetworkCommand& command)
{
    uint32_t commandChainID1 = this->findCommandChainID(command._networkCommand);
    uint32_t commandChainID2 = command.findCommandChainID(command._networkCommand);

    if (commandChainID1 == commandChainID2) {
        return true;
    }
    else {
        //std::cout << "false" << "\n";
        return false;
    }
}

uint8_t NetworkCommand::findCommandType(std::vector<uint8_t>& rawCommand) const
{
    if (rawCommand.size() > 0) {
        return rawCommand[0];
    }
    else {
        return 0;
    }
}
uint8_t NetworkCommand::findOperationType(std::vector<uint8_t>& rawCommand) const
{
    switch (_commandType)
    {
    case commandType::reliable:
        return rawCommand[13];
    case commandType::unreliable:
        return rawCommand[17];
    case commandType::fragmented:
        return rawCommand[33];
    default:
        return 0;
    }
}
uint16_t NetworkCommand::findEventCode(std::vector<uint8_t>& rawCommand) const
{
    if (_isCommandFull == true) {
        if (rawCommand[rawCommand.size() - 3] == dataType::int16) {
            //this->printCommandInOneString();
            return ((rawCommand[rawCommand.size() - 2] << 8) |
                rawCommand[rawCommand.size() - 1]) & 0x03ff;
        }
        /*else if (_commandType == commandType::unreliable and rawCommand.size() == 67) {
            return rawCommand[rawCommand.size() - 1] & 0x000f;
        }*/
        else {
            return rawCommand[rawCommand.size() - 1];
        }
    }
    else {
        return 0;
    }
};