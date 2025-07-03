#include "pch.h"
#include "NetworkCommand.h"

std::vector<uint16_t> nnCodes = {};
std::vector<uint16_t> nCodes = { /*55, 71*/49 };
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
NetworkCommand::NetworkCommand(NetworkCommand& command, size_t beginOffset)
{
    _networkCommand = { command._networkCommand.begin() + beginOffset, command._networkCommand.end() };
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
std::string itemData = "";
size_t counter = 0;
void NetworkCommand::analyzeCommand(
    GLFWwindow* window, 
    std::vector<uint8_t>& packetHeader, 
    NetworkPacketInfo& packetInfo,
    bool isHikingMode
)
{
    DataLayout dataLayout{};
    /*if (std::isElementInVector(nCodes, _eventCode) and this->size() != 67) {
        dataLayout.findDataLayout(*this);
        dataLayout.printInfo(*this);
    }*/
    if (isHikingMode) {
        if (_operationType == operationType::event) {
            switch (_eventCode)
            {
            case eventCode::HealthUpdate:
                //_entityList._playerList.update(HealthUpdateHandler::HealthUpdateHandler(*this)); // need to add health handling
                _entityList._currentLocation._mobList.update(HealthUpdateHandler::HealthUpdateHandler(*this));
                break;
            case eventCode::NewSimpleHarvestableObjectList:
                _entityList._currentLocation._harvestableList.update(HarvestableList(*this));
                break;
            case eventCode::NewHarvestableObject:
                _entityList._currentLocation._harvestableList.update(Harvestable(*this));
                break;
            case eventCode::HarvestableChangeState:
                _entityList._currentLocation._harvestableList.updateState(*this);
                break;
            case eventCode::MobChangeState:
                _entityList._currentLocation._mobList.mobChangeState(*this);
                //_entityList._playerList.update(Player::playerMove(*this)); 
                break;
            case eventCode::NewMob:
                _entityList._currentLocation._mobList.newMob(Mob::Mob(*this));
                break;
            case eventCode::NewFishingZoneObject:
                _entityList._currentLocation._fishNodeList.update(FishNode(*this));
                break;
            case eventCode::HarvestStart:
                //Harvestable::harvestStart(*this);
                break;
            case eventCode::HarvestFinished:
                Harvestable::harvestFinished(*this);
                break;
            case 66: // crafting finished on station
                break;
            case eventCode::NewCharacter:
                //if (!std::isElementInVector(cityLocations, _entityList._currentLocation._locationID)) {
                _entityList._currentLocation._playerList.newPlayer(Player(*this));
                //}
                break;
            case eventCode::Leave:
                _entityList._currentLocation._playerList.playerLeave(Player(*this));
                break;
            case eventCode::PlayerMove:
                //_entityList._playerList.update(Player::playerMove(*this)); 
                break;
            default:
                break;
            }
            if (_networkCommand.size() == 67/* and _networkCommand[66] & (2 << 0)*/) {
                if (dataLayout.findNumOfFragments(*this) == 2) {
                    //EntityMove::updateEntityListMove(*this, _entityList._currentLocation._playerList);
                    EntityMove::updateEntityListMove(*this, _entityList._currentLocation._mobList);
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
        case operationCode::Join:
            Location::changeLocation(
                *this, 
                _entityList._locationList,
                _entityList._currentLocation, 
                false
            );
            break;
        case operationCode::Move:
            _entityList._playerSelf = PlayerSelf(*this);
            break;
        /*case operationCode::ChangeCluster: // other player changing location not only me
            _entityList.ChangeCluster(*this); 
            break;*/
        case operationCode::AuctionSellOrders:
            //Auction::auctionOrders(*this, true, _entityList._currentLocation, true);
            break;
        case operationCode::AuctionBuyOrders:
            //Auction::auctionOrders(*this, false, _entityList._currentLocation, true);
            break;
        case operationCode::RealEstateGetAuctionData:
            break;
        case operationCode::AuctionGetFinishedAuctions: // non standard format
            /*dataLayout.findDataLayout(*this);
            dataLayout.printInfo(*this);
            this->printCommandInOneString();*/
            break;
        case operationCode::AuctionGetFinishedAuctionsCount: // non standard format
            break;
        case operationCode::AuctionGetMyOpenAuctions: // non standard format
            break;
        case operationCode::AuctionGetItemAverageStats:
            //start = std::chrono::high_resolution_clock::now();
            Auction::findAuctionAverageValues(*this, itemData, ",");
            /*stop = std::chrono::high_resolution_clock::now();
            std::cout << 
                "time to write acution average values: " << 
                std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";*/
            break;
        case operationCode::GetClusterMapInfo:
            //MapCluster::findClusterData(*this);
            break;
        default:
            break;
        }
        //std::cout << this->getEventCode() << "\n";
    }
    if (_operationType == operationType::request) {
        switch (_eventCode)
        {
        case operationCode::AuctionGetItemAverageStats:
            // packetInfo.to_string();
	        // NetworkCommand::printCommandInOneString(networkPacketHeader, false, true);
            Auction::GetItemData(*this, _entityList._currentLocation, itemData, false);
            break;
        case operationCode::Move:
            _entityList._playerSelf.update(*this);
            //std::cout << _entityList._playerSelf._positionX << " " << _entityList._playerSelf._positionY << "\n";
            break;
        case operationCode::ChangeCluster:
            /*dataLayout.findDataLayout(*this);
            dataLayout.printInfo(*this);*/
            _entityList.changeLocation(*this);
            break;
        case operationCode::AuctionBuyOrders:
            break;
        default:
            break;
        }  
    }
    if (_operationType == operationType::not_defined) {
    }
    if (isHikingMode) {
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
void NetworkCommand::printCommandInOneString(std::vector<uint8_t>& rawCommand, bool lineBreak, bool isHex)
{
    if (isHex) {
        std::cout.setf(std::ios::hex, std::ios::basefield);
    }
    for (size_t i = 0; i < rawCommand.size(); i++) {
        if (rawCommand[i] < 16)
            std::cout << "0";
        std::cout << unsigned(rawCommand[i]);
    }
    if (lineBreak) {
        std::cout << "\n";
    }
    else {}
    if (isHex) {
        std::cout.unsetf(std::ios::hex);
    }
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
NetworkCommand::iterator NetworkCommand::begin()
{
    return _networkCommand.begin();
}
NetworkCommand::iterator NetworkCommand::end()
{
    return _networkCommand.end();
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
        return eventCode::None;
    }
}
