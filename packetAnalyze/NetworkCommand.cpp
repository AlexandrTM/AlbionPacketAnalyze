#include "pch.h"

std::vector<size_t> nnCodes = {};
std::vector<size_t> nCodes = {};

NetworkCommand::NetworkCommand(std::vector<uint8_t> command)
{
    _networkCommand = command;
    _commandType = findCommandType();
    _operationType = findOperationType();
    _isCommandFull = isCommandFull();
    _eventCode = findEventCode();
}
NetworkCommand::NetworkCommand(NetworkCommand& command, size_t regionStart)
{
    _networkCommand = { command._networkCommand.begin() + regionStart, command._networkCommand.end() };
}
NetworkCommand::NetworkCommand() 
{
    _networkCommand = {};
    _commandType = 0;
    _operationType = 0;
    _isCommandFull = false;
    _eventCode = 0;
}

EntityList _entityList{};
void NetworkCommand::analyzeCommand(GLFWwindow* window)
{
    //this->printCommandInOneString();
    if (_operationType == operationType::event) {
        switch (_eventCode)
        {
        case eventCode::harvestableObjectList:
            _entityList._harvestableList.update(HarvestableList(*this)); break;
        case eventCode::harvestableObject:
            _entityList._harvestableList.update(Harvestable(*this)); break;
        case eventCode::harvestableChangeState:
            _entityList._harvestableList.updateState(*this); break;
        /*case eventCode::newPlayer:
            _entityList._playerList.newPlayer(Player(*this)); break;
        case eventCode::playerLeave:
           _entityList._playerList.playerLeave(Player(*this)); break;*/
        case 66:
            //_entityList._playerList.update(Player::playerMove(*this)); 
            break;
        case 65:
            //_entityList._playerList.update(Player::playerMove(*this)); 
            break;
        default:
            break;
        }
        /*if (_networkCommand.size() == 67) {
            _entityList._playerList.update(Player::playerMove(*this));
        }*/
    }
    if (_operationType == operationType::operationResponse) {
        DataLayout dataLayout;
        //std::cout << _eventCode << "\n";
        switch (_eventCode) 
        {
        case operationCode::move:
            _entityList._player = PlayerSelf(*this); break;
        case operationCode::changeLocation:
            _entityList.changeLocation(); break;
        case operationCode::auctionSellOrders:
            //this->printCommandInOneString();
            break;
        case operationCode::auctionBuyOrders:
            /*dataLayout.findDataLayout(*this);
            dataLayout.printInfo();
            this->printCommandInOneString();*/
            break;
        case operationCode::auctionGetFinishedOrders:
            //this->printCommandInOneString();
            break;
        case operationCode::auctionAverageValues:

            Auction::FindAuctionAverageValues(*this);
            //std::cout << this->size() << "\n";
            break;
        case 88:
            //this->printCommandInOneString();
            break;
        default:
            break;
        }
    }
    if (_operationType == operationType::operationRequest) {

        if (_eventCode == operationCode::move) {
            _entityList._player.update(*this);
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

uint8_t _previousCommandIndexInChain = 0;
void NetworkCommand::fillFragmentedCommand(NetworkCommand command)
{
    uint8_t commandsNumInChain = command._networkCommand[19];
    uint8_t commandIndexInChain = command._networkCommand[23];

    if (command.isFirstCommandInChain()) {
        *this += command;
    }
    else if (_previousCommandIndexInChain + 1 == commandIndexInChain) {
        *this += NetworkCommand(command, 32);
        _previousCommandIndexInChain += 1;
    }

    if (command.isLastCommandInChain()) {
        _commandType = commandType::fragmented;
        _operationType = findOperationType();
        _isCommandFull = true;
        _eventCode = findEventCode();
        _previousCommandIndexInChain = 0;
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
    return *this;
}
bool NetworkCommand::operator!=(NetworkCommand& command)
{
    if (_networkCommand != command._networkCommand) {
        return true;
    }
    else {
        return false;
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
    if (_isCommandFull == true) {
        if (_networkCommand[_networkCommand.size() - 3] == dataType::int16) {
            //this->printCommandInOneString();
            return ((_networkCommand[_networkCommand.size() - 2] << 8) |
                        _networkCommand[_networkCommand.size() - 1]) & 0x03ff;
        }
        /*else if (_commandType == commandType::unreliable and _networkCommand.size() == 67) {
            return _networkCommand[_networkCommand.size() - 1] & 0x000f;
        }*/
        else {
            return _networkCommand[_networkCommand.size() - 1];
        }
    }
    else {
        return 0;
    }
};