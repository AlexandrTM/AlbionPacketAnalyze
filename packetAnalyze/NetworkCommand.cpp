#include "pch.h"

std::vector<uint16_t> nnCodes = {};
std::vector<uint16_t> nCodes = { /*38, 37, 36, 35, 34, 33, 32, 41, 42, 44*/ };
std::vector<std::string> cityLocations = { "0000", "0301" };

NetworkCommand::NetworkCommand(std::vector<uint8_t> rawCommand)
{
    _networkCommand = rawCommand;
    _commandType = findCommandType(rawCommand);
    _operationType = findOperationType(rawCommand);
    _eventCode = findEventCode(rawCommand);
    _commandIndexInChain = findCommandIndexInChain(rawCommand);
    _commandID = findCommandID(rawCommand);
}
NetworkCommand::NetworkCommand(NetworkCommand& command, size_t regionStart)
{
    _networkCommand = { command._networkCommand.begin() + regionStart, command._networkCommand.end() };
    _commandType = findCommandType(command._networkCommand);
    _operationType = findOperationType(command._networkCommand);
    _eventCode = findEventCode(command._networkCommand);
    _commandIndexInChain = findCommandIndexInChain(command._networkCommand);
    _commandID = findCommandID(command._networkCommand);
}
NetworkCommand::NetworkCommand() 
{
    _networkCommand = {};
    _commandType = 0;
    _operationType = 0;
    _eventCode = 0;
    _commandIndexInChain = 0;
    _commandID = 0;
}

EntityList _entityList{};
size_t counter = 0;
void NetworkCommand::analyzeCommand(GLFWwindow* window, bool hikingMode)
{
    DataLayout dataLayout{};
    //if (_eventCode == 0) {} // joining server
    //else if (std::isElementInVector(nCodes, _eventCode)) {
    //    _dataLayout.findDataLayout(*this);
    //    _dataLayout.printInfo(*this);
    //}
    if (hikingMode) {
        if (_operationType == operationType::event) {
            switch (_eventCode)
            {
            case eventCode::healthUpdate:
                //_entityList._playerList.update(HealthUpdate::HealthUpdate(*this)); // need to add health handling
                _entityList._currentLocation._mobList.update(HealthUpdate::HealthUpdate(*this));
                //dataLayout.findDataLayout(*this);
                //dataLayout.printInfo(*this);
                break;
            case eventCode::harvestableObjectList:
                _entityList._currentLocation._harvestableList.update(HarvestableList(*this));
                break;
            case eventCode::harvestableObject:
                _entityList._currentLocation._harvestableList.update(Harvestable(*this));
                break;
            case eventCode::harvestableChangeState:
                _entityList._currentLocation._harvestableList.updateState(*this);
                break;
            case eventCode::harvestStart:
                //Harvestable::harvestStart(*this);
                break;
            case eventCode::harvestFinished:
                Harvestable::harvestFinished(*this);
                break;
            case eventCode::newPlayer:
                /*dataLayout.findDataLayout(*this);
                dataLayout.printInfo(*this);*/
                //if (!std::isElementInVector(cityLocations, _entityList._currentLocation._locationID)) {
                _entityList._currentLocation._playerList.newPlayer(Player(*this));
                //}
                break;
            case eventCode::playerLeave:
                _entityList._currentLocation._playerList.playerLeave(Player(*this));
                break;
            case eventCode::playerMove:
                //_entityList._playerList.update(Player::playerMove(*this)); 
                break;
            case eventCode::mobChangeState:
                _entityList._currentLocation._mobList.mobChangeState(*this);
                //_entityList._playerList.update(Player::playerMove(*this)); 
                break;
            case eventCode::newMob:
                _entityList._currentLocation._mobList.newMob(Mob::Mob(*this));
                break;
            default:
                break;
            }
            if (_networkCommand.size() == 67 and _networkCommand[66] & (2 << 0)) {
                if (dataLayout.findNumOfFragments(*this) == 2) {
                    _entityList._currentLocation._playerList.update(EntityMove::EntityMove(*this));
                    _entityList._currentLocation._mobList.update(EntityMove::EntityMove(*this));
                }
                else {
                    /*dataLayout.findDataLayout(*this);
                    dataLayout.printInfo(*this);*/
                }
            }
            else {
                /*dataLayout.findDataLayout(*this);
                dataLayout.printInfo(*this);*/
            }
        }
    }
    if (_operationType == operationType::response) {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point stop;
        /*dataLayout.findDataLayout(*this);
        dataLayout.printInfo(*this);*/
        /*std::cout << "commandChainID: " << this->getCommandID() << " " <<
                     "event code: " << _eventCode << "\n";*/
        switch (_eventCode) 
        {
        case operationCode::joinLocation:
            Location::changeLocation(*this, _entityList._locationList, 
                _entityList._currentLocation, true);
            //dataLayout.findDataLayout(*this);
            //dataLayout.printInfo(*this);
            break;
        case operationCode::move:
            /*_dataLayout.findDataLayout(*this);
            _dataLayout.printInfo(*this);*/
            //this->printCommandInOneString();
            _entityList._player = PlayerSelf(*this);
            break;
        /*case operationCode::changeLocation: // other player changing location not only me
            _entityList.changeLocation(*this); 
            break;*/
        case operationCode::auctionSellOrders:
            //this->printCommandInOneString();
            //_dataLayout.findDataLayout(*this);
            //_dataLayout.printInfo(*this);
            break;
        case operationCode::auctionBuyOrders:
            break;
        case operationCode::auctionFinishedOrders: // non standard format
            //this->printCommandInOneString();
            /*_dataLayout.findDataLayout(*this);
            _dataLayout.printInfo(*this);*/
            break;
        case operationCode::auctionMyOwnAuctions: // non standard format
            /*this->printCommandInOneString();
            _dataLayout.findDataLayout(*this);
            _dataLayout.printInfo(*this);*/
            break;
        case operationCode::auctionAverageValues:
            //start = std::chrono::high_resolution_clock::now();
            Auction::findAuctionAverageValues(*this, ",");
            /*stop = std::chrono::high_resolution_clock::now();
            std::cout << 
                "time to write acution average values: " << 
                std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";*/
            break;
        case operationCode::getClusterMapInfo:
            //_dataLayout.findDataLayout(*this);
            //_dataLayout.printInfo(*this);
            //MapCluster::findClusterData(*this);
            //this->printCommandInOneString();
            break;
        default:
            break;
        }
        //std::cout << this->getEventCode() << "\n";
        //this->printCommandInOneString();
    }
    if (_operationType == operationType::request) {
        //this->printCommandInOneString();
        if (_eventCode == auctionSellOrders) {
            dataLayout.findDataLayout(*this);
            dataLayout.printInfo(*this, true);
        }
        switch (_eventCode)
        {
        case operationCode::move:
            _entityList._player.update(*this);
            //std::cout << _entityList._player._positionX << " " << _entityList._player._positionY << "\n";
            break;
        case operationCode::changeLocation:
            /*dataLayout.findDataLayout(*this);
            dataLayout.printInfo(*this);*/
            _entityList.changeLocation(*this);
            break;
        case operationCode::auctionBuyOrders:
            /*dataLayout.findDataLayout(*this);
            if (dataLayout.size() == 14) {
                Auction::findProductName(*this, dataLayout);
            }*/
            break;
        }
            
    }
    if (_operationType == operationType::not_defined) {
        //this->printCommandInOneString();
        /*DataLayout dataLayout{};
        dataLayout.findDataLayout(*this);
        dataLayout.printInfo(*this);*/
    }
    if (hikingMode) {
        _entityList.draw(window);
    }
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
void NetworkCommand::printCommandInOneString(bool lineBreak, bool isHex)
{
    if (isHex) {
        std::cout.setf(std::ios::hex, std::ios::basefield);
    }
    for (size_t i = 0; i < _networkCommand.size(); i++)
    {
        if (_networkCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(_networkCommand[i]);
    }
    if (lineBreak) {
        std::cout << "\n";
    }
    else {}
    if (isHex) {
        std::cout.unsetf(std::ios::hex);
    }
}
void NetworkCommand::printCommandInOneString(size_t regionStart, size_t regionEnd, 
    bool lineBreak, bool isHex)
{
    if (isHex) {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        for (size_t i = regionStart; i < regionEnd; i++)
        {
            if (_networkCommand[i] < 16) {
                std::cout << "0";
            }
            std::cout << unsigned(_networkCommand[i]);
        }
        if (lineBreak) {
            std::cout << "\n";
        }
        else {}
        std::cout.unsetf(std::ios::hex);
    }
    else {
        for (size_t i = regionStart; i < regionEnd; i++)
        {
            std::cout << unsigned(_networkCommand[i]);
        }
        if (lineBreak) {
            std::cout << "\n";
        }
        else {}
    }
}

void NetworkCommand::endFragmentedCommand()
{
    _commandType = commandType::fragmented;
    //std::cout << "operationType: " << (unsigned)_operationType << "\n";
    _eventCode = findEventCode(_networkCommand);
    //std::cout << "eventCode: " << (unsigned)_eventCode << "\n";
}

uint8_t NetworkCommand::findCommandIndexInChain(std::vector<uint8_t>& rawCommand) const
{
    if (_commandType == commandType::fragmented) {
        return rawCommand[23];
    }
    else {
        return 0;
    }
}
uint32_t NetworkCommand::findCommandID(std::vector<uint8_t>& rawCommand) const
{
    if (_commandType == commandType::fragmented) {
        return net::read_uint32(rawCommand, 12);
    }
    else if (_commandType == commandType::reliable) {
        return net::read_uint32(rawCommand, 8);
    }
    else {
        return 0;
    }
}

uint8_t NetworkCommand::getCommandIndexInChain() const
{
    return _commandIndexInChain;
}
uint32_t NetworkCommand::getCommandID() const
{
    return _commandID;
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
    uint32_t commandChainID1 = this->findCommandID(command._networkCommand);
    uint32_t commandChainID2 = command.findCommandID(command._networkCommand);

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
        /*if (rawCommand[0] == 1) {
            std::cout.setf(std::ios::hex, std::ios::basefield);
            for (size_t i = 0; i < rawCommand.size(); i++) {
                if (rawCommand[i] < 16) {
                    std::cout << "0";
                }
                std::cout << (unsigned)rawCommand[i];
            }
            std::cout.unsetf(std::ios::hex);
            std::cout << "\n";
        }*/
        return rawCommand[0];
    }
    else {
        /*std::cout << "Command type not defined. " << 
            "Command type: " << (unsigned)rawCommand[0] << "\n";*/
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
    case commandType::something:
        return operationType::not_defined;
    default:
        /*std::cout << "commandType not defined. " <<
            "commandType: " << (unsigned)_commandType << "\n";*/
        return operationType::not_defined;
    }
}
uint16_t NetworkCommand::findEventCode(std::vector<uint8_t>& rawCommand) const
{
    if (rawCommand.size() >= 4) {
        if (rawCommand[rawCommand.size() - 3] == dataType::int16) {
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
        return eventCode::none;
    }
}
