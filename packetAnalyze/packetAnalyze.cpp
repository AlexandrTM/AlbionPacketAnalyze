#include "pch.h"

#include "PacketAnalyze.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Rankings.h"

using namespace Tins;

std::vector<NetworkPacket> text;
int _windowPosX, _windowPosY;
GLint _screenWidth, _screenHeight;

uint8_t _mapState = mapState::fullscreenMap;

bool _isHikingMode = false;

std::vector<uint16_t> _eventCodes;

std::vector<size_t> PacketAnalyze::findCommandBordersInPacket(std::string packet)
{
    std::vector<size_t> commandBorders;
    size_t packetHeaderSize = 24;
    size_t commandsNumInPacket = strtoul(packet.substr(6, 2).c_str(), nullptr, 16);

    commandBorders.push_back(packetHeaderSize);
    for (size_t i = 0; i < commandsNumInPacket; i++) {
        size_t commandWidth = strtoul(packet.substr(commandBorders[i] + 12, 4).c_str(), nullptr, 16);
        commandBorders.push_back(commandBorders[i] + commandWidth * 2);
    }

    return commandBorders;
}
bool PacketAnalyze::findStringInStringImpl(
    const std::string& string_1,
    const std::string& string_2,
    size_t regionStart,
    size_t regionEnd,
    size_t* stringPosition
) {
    if (string_2.empty() || regionStart >= string_1.size()) {
        return false;
    }

    for (size_t i = regionStart; i < regionEnd - string_2.length() + 1; i += 2) {
        if (string_1.substr(i, string_2.length()) == string_2) {
            if (stringPosition) {
                *stringPosition = i;
            }
            return true;
        }
    }
    return false;
}
bool PacketAnalyze::findStringInString(std::string string_1, std::string string_2, size_t& stringPosition)
{
    return findStringInStringImpl(string_1, string_2, 0, string_1.size(), &stringPosition);
}
bool PacketAnalyze::findStringInString(
    std::string string_1, std::string string_2, 
    size_t regionStart, size_t regionEnd
)
{
    return findStringInStringImpl(string_1, string_2, regionStart, regionEnd, nullptr);
}
bool PacketAnalyze::findStringInString(
    std::string string_1, std::string string_2, 
    size_t regionStart, size_t regionEnd,
    size_t& stringPosition
)
{
    return findStringInStringImpl(string_1, string_2, regionStart, regionEnd, &stringPosition);
}

void PacketAnalyze::run()
{
    //if (_isHikingMode) { initWindow(); }
    initWindow();
    initSniffer();
    //sendPacket();
    mainLoop();
    cleanup();
    //if (_isHikingMode) { cleanup(); }
}

void PacketAnalyze::mainLoop()
{
    float deltaTime = 0.0;
    float timeSinceLaunch = 0.0;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto previousTime = std::chrono::high_resolution_clock::now();

    // Track the time for drawing entities
    auto lastEntitiesDrawTime = std::chrono::high_resolution_clock::now();
    // Track the time for printing visible players
    auto lastPlayersPrintTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(_window)) {
        currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
        previousTime = currentTime;
        timeSinceLaunch += deltaTime;

        sniffPacket();

        glfwPollEvents();

        if (_isHikingMode) {
            // Rate limit drawing
            //if (std::chrono::duration<float>(currentTime - lastEntitiesDrawTime).count() >= (1.0f / 60.0f)) {
                entityList.draw(_window);
                //lastEntitiesDrawTime = currentTime;  // Update last draw time
            //}

            // Rate limit printing visible players (every 3 seconds)
            if (std::chrono::duration<float>(currentTime - lastPlayersPrintTime).count() >= 3.0f) {
                //entityList.printVisiblePlayers();

                lastPlayersPrintTime = currentTime;  // Update last print time
            }
        }
    }
}

std::vector<bool> PacketAnalyze::findSameSymbolsInText(NetworkPacket paragraph)
{
    std::vector<bool> sameSymbols = {};

    for (size_t i = 0; i < paragraph[0].size(); i++) {
        int symbolSimilarity = 1;

        for (size_t j = 1; j < paragraph.size(); j++) {
            if (paragraph[j][i] == paragraph[0][i]) {
                symbolSimilarity += 1;
            }
        }
        sameSymbols.push_back(floor((float)symbolSimilarity / text.size()));
    }

    return sameSymbols;
}
void PacketAnalyze::colorizeSameText(NetworkPacket paragraph, HANDLE consoleHandle)
{
    std::vector<bool> sameSymbols = findSameSymbolsInText(paragraph);

    for (size_t i = 0; i < sameSymbols.size(); i++) {
        if (sameSymbols[i] == 1) {
            SetConsoleTextAttribute(consoleHandle, 2);
        }

        std::cout << sameSymbols[i];
        SetConsoleTextAttribute(consoleHandle, 7);
        if (i % 8 == 7) {
            std::cout << " ";
        }
        if (i % 64 == 63 and i != (sameSymbols.size() - 1)) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";

    for (size_t i = 0; i < paragraph.size(); i++) {
        for (size_t j = 0; j < paragraph[i].size(); j++) {
            if (sameSymbols[j] == 1) {
                SetConsoleTextAttribute(consoleHandle, 2);
            }

            std::cout << paragraph[i][j];
            SetConsoleTextAttribute(consoleHandle, 7);
            if (j % 8 == 7) {
                std::cout << " ";
            }
            if (j % 64 == 63 and j != (paragraph[i].size() - 1)) {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }
}
void PacketAnalyze::outputColorizedNetworkPacket(std::vector<NetworkPacket> text)
{
    for (size_t i = 0; i < text.size(); i++) {
        SetConsoleTextAttribute(consoleHandle, 6);
        std::cout << "\"" << _eventCodes[i] << "\"" << " " << _amountOfSameCommands[i] << "\n";
        SetConsoleTextAttribute(consoleHandle, 7);

        if (text[i].size() > 1) {
            colorizeSameText(text[i], consoleHandle);
        }
    }
}

void PacketAnalyze::findUniqueEventCodes(NetworkCommand& command)
{
    if (!std::isElementInVector(_eventCodes, command.getEventCode()))
    {
        _eventCodes.push_back(command.getEventCode());
        _amountOfSameCommands.push_back({});
        text.push_back({});
    }

    _amountOfSameCommands[std::findElementIndex(_eventCodes, command.getEventCode())] += 1;
}

void PacketAnalyze::initWindow()
{
    glfwInit();

    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    //glfwWindowHint(GLFW_HAND_CURSOR, GLFW_FALSE);
    //glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* _videoMode = glfwGetVideoMode(_monitor);

    // big map
    /*_window = glfwCreateWindow(_videoMode->width / 1.715, _videoMode->width / 1.715,
                                u8"Packet Analyze", nullptr, nullptr);
    glfwGetWindowSize(_window, &_screenWidth, &_screenHeight);
    glViewport(0, 0, _screenWidth, _screenHeight);*/

    // full screen map
    _window = glfwCreateWindow(
        _videoMode->width - 1, _videoMode->height - 1,
        "Packet Analyze", nullptr, nullptr
    );
    glfwGetWindowSize(_window, &_screenWidth, &_screenHeight);
    glViewport(0, 0, _screenWidth, _screenHeight);
    //glfwSetWindowPos(_window, 0, 0);

    GLFWimage images[1]{};
    images[0].pixels = stbi_load("mineral_icon.jpg", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(_window, 2, images);
    stbi_image_free(images[0].pixels);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    glfwSetCursorEnterCallback(_window, cursorEnterCallback);
    glfwSetKeyCallback(_window, keyCallback);
    glfwMakeContextCurrent(_window);
}
void PacketAnalyze::cleanup()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void PacketAnalyze::changeMapState(GLFWwindow*  window)
{
    if (_mapState == mapState::miniMap) {
        GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* _videoMode = glfwGetVideoMode(_monitor);
        glfwSetWindowSize(window, _videoMode->width / 4.05f, _videoMode->height / 4.05f);
        glfwGetWindowSize(window, &_screenWidth, &_screenHeight);
        glViewport(0, 0, _screenWidth, _screenHeight);
        glfwSetWindowPos(window, 1073, 570);
    }
    else if (_mapState == mapState::fullscreenMap) {
        GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* _videoMode = glfwGetVideoMode(_monitor);
        glfwSetWindowSize(window, _videoMode->width - 1, _videoMode->height - 1);
        glfwGetWindowSize(window, &_screenWidth, &_screenHeight);
        glViewport(0, 0, _screenWidth, _screenHeight);
        glfwSetWindowPos(window, 0, 0);
    }
}

void PacketAnalyze::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<PacketAnalyze*>(glfwGetWindowUserPointer(window));
    app->_framebufferResized = true;
}
void PacketAnalyze::cursorEnterCallback(GLFWwindow* window, int entered)
{
    if (entered == GLFW_TRUE)
    {
        //glfwSetWindowAttrib(window, GLFW_FOCUSED, GLFW_TRUE);
    }
    else
    {
        //glfwSetWindowAttrib(window, GLFW_FOCUSED, GLFW_TRUE);
    }
}
void PacketAnalyze::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_RIGHT) {
            glfwGetWindowPos(window, &_windowPosX, &_windowPosY);
            glfwSetWindowPos(window, _windowPosX + _screenHeight / 6, _windowPosY);
        }
        if (key == GLFW_KEY_LEFT) {
            glfwGetWindowPos(window, &_windowPosX, &_windowPosY);
            glfwSetWindowPos(window, _windowPosX - _screenHeight / 6, _windowPosY);
        }
        if (key == GLFW_KEY_DOWN) {
            glfwGetWindowPos(window, &_windowPosX, &_windowPosY);
            glfwSetWindowPos(window, _windowPosX, _windowPosY + _screenHeight / 6);
        }
        if (key == GLFW_KEY_UP) {
            glfwGetWindowPos(window, &_windowPosX, &_windowPosY);
            glfwSetWindowPos(window, _windowPosX, _windowPosY - _screenHeight / 6);
        }
        if (glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE) {
            if (key == GLFW_KEY_N and _mapState == mapState::miniMap) {
                _mapState = mapState::fullscreenMap;
                changeMapState(window);
            }
            else if ((key == GLFW_KEY_N or key == GLFW_KEY_ESCAPE) and _mapState == mapState::fullscreenMap) {
                _mapState = mapState::miniMap;
                changeMapState(window);
            }
        }

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_P) {
            Auction::printPlayersAuctionData();
        }
    }
}
    
void PacketAnalyze::initSniffer()
{
    SnifferConfiguration albionConfig;
    //albionConfig.set_filter("ip dst 192.168.1.70");
    // Destination Address: 193.169.238.126
    //albionConfig.set_immediate_mode(true);
    //albionConfig.set_promisc_mode(true);
    _sniffer = Sniffer(_iface.name(), albionConfig);
}
bool PacketAnalyze::isPacketFiltered(RawNetworkPacket& filteredPacket, NetworkPacketInfo& packetInfo)
{
    PDU* sniffedPacket = _sniffer.next_packet();

    if (sniffedPacket) {
        const IP& ip = sniffedPacket->rfind_pdu<IP>();
        const UDP& udp = sniffedPacket->rfind_pdu<UDP>();

        if (/*_albionIPRange.contains(ip.src_addr()) and */(udp.sport() == 5056 or udp.dport() == 5056)) {
            /*Tins::IPv4Address src_ip = ip.src_addr();
            Tins::IPv4Address dst_ip = ip.dst_addr();
            
            uint16_t src_port = udp.sport();
            uint16_t dst_port = udp.dport();*/
            
            RawPDU rawPacket = sniffedPacket->rfind_pdu<RawPDU>();
            RawNetworkPacket packet;
            readRawPacket(rawPacket, packet);

            if (packet.size() > 0) {
                filteredPacket = packet;
                //packetInfo = { src_ip, dst_ip, src_port, dst_port };
                return true;
            }
        }
    }
    return false;
}
void PacketAnalyze::sniffPacket()
{
    try {
        RawNetworkPacket packet;
        NetworkPacketInfo packetInfo;
        if (isPacketFiltered(packet, packetInfo)) {
            //auto start = std::chrono::high_resolution_clock::now();

            analyzePacket(packet, packetInfo);

            //auto stop = std::chrono::high_resolution_clock::now();
            //std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
        }
    }
    catch (std::exception& e) {
    }
}

void PacketAnalyze::sendPacket()
{
    try {
        // Convert the payload from hex string to a byte vector
        std::string hex_string = "959100050093c1e15e6ad63c0600010400000028000001c8f302010006006264016b038b026201036202ff6900000169fd6b0059";
        std::vector<uint8_t> payload;
        for (size_t i = 0; i < hex_string.length(); i += 2) {
            payload.push_back(std::stoi(hex_string.substr(i, 2), nullptr, 16));
        }

        // Define source and destination IPs and ports
        std::string source_ip = "192.168.1.3";
        uint16_t source_port = 52197;
        std::string destination_ip = "193.169.238.97";
        uint16_t destination_port = 5056;

        // Create the packet: IP layer -> UDP layer -> RawPDU (payload)
        UDP udp = UDP(destination_port, source_port) / RawPDU(payload);

        // IP layer with destination IP only (source IP is automatically determined)
        IP ip = IP(destination_ip) / udp;

        // Send the packet using a specific interface
        PacketSender sender;
        sender.send(ip);

        std::cout << "Packet sent successfully!" << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void PacketAnalyze::readRawPacket(RawPDU pdu, RawNetworkPacket& rawPacketPayload)
{
    rawPacketPayload = pdu.payload();
}
void PacketAnalyze::readRawPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, RawNetworkPacket& rawPacketPayload)
{
    for (size_t i = regionStart; i < regionEnd; i++) {
        rawPacketPayload.push_back(pdu.payload()[i]);
    }
}
RawNetworkPacket PacketAnalyze::readRawPacket(RawPDU pdu, size_t regionStart, size_t regionEnd)
{
    RawNetworkPacket rawPacketPayload;

    for (size_t i = regionStart; i < regionEnd; i++) {
        rawPacketPayload.push_back(pdu.payload()[i]);
    }
    return rawPacketPayload;
}

void PacketAnalyze::analyzePacket(RawNetworkPacket rawPacket, NetworkPacketInfo& packetInfo)
{
    _packet = NetworkPacket::findCommandsInPacket(rawPacket);
    _packet._packetInfo = packetInfo;

    for (size_t i = 0; i < _packet.size(); i++)
    {
        if (_packet[i].getCommandType() == commandType::fragmented) {
            if (_fragmentedCommandsBuffer.isNewFragmentedCommand(_packet[i])) {
                _fragmentedCommandsBuffer.push_back(FragmentedNetworkCommand(_packet[i]));
            }
            else {
                _fragmentedCommandsBuffer.addCommandFragment(_packet[i]);
            }
            for (size_t j = 0; j < _fragmentedCommandsBuffer.size();) {
                if (_fragmentedCommandsBuffer[j].isCommandFull()) {
                    //std::cout << _fragmentedCommandsBuffer.size() << "\n";
                    _fragmentedCommandsBuffer[j].sort();
                    _fragmentedCommandsBuffer[j].connectFragments();
                    _fragmentedCommandsBuffer[j][0].endFragmentedCommand();
                    analyzeCommand(
                        _window, _fragmentedCommandsBuffer[j][0], _packet._packetHeader, _packet._packetInfo, _isHikingMode
                    );
                    _fragmentedCommandsBuffer.erase(_fragmentedCommandsBuffer.begin() + j);
                    //std::cout << _fragmentedCommandsBuffer.size() << "\n";
                }
                else {
                    j++;
                }
            }
        }
        if (_packet[i].getCommandType() == commandType::reliable
         or _packet[i].getCommandType() == commandType::unreliable) {

            analyzeCommand(
                _window, _packet[i], _packet._packetHeader, _packet._packetInfo, _isHikingMode
            );
        }
    }
}

//---------------NewLocationMarker = 378----------------
// NewHuntTrack = 551
// NewSmugglerCrateDeliveryStation = 605,
// NewUnreadMails = 200,
// NewRandomDungeonExit = 319,

// ava portal event code 32

// teleporation event codes 18, 19, 23, 24
std::vector<uint16_t> nnCodes = { 
    1, 8, 11, 18, 19, 21, 23, 24, 30, 35, 90, 103, 104, 154, 
    160, 163, 216, 221, 245, 266, 295, 296, 364, 591,  };
std::vector<uint16_t> nCodes = { 
    63, 366, 367, 154, 151, 216, 30, 323, 19, 11, 221, 254
    //190, 192, 166, 256, 257 // real estate
    //256, 257
};
std::vector<std::string> cityLocations = { "0000", "0301" };

 	/*
 	NewSimpleItem = 32,
 	NewEquipmentItemLegendarySoul = 37,
 	NewTreasureDestinationObject = 41,
 	NewMonolithObject = 109,
 	NewOrbObject = 111,
 	NewCastleObject = 112,
 	NewChainSpell = 115,
 	NewTreasureChest = 117,
 	NewRealEstate = 190,
 	NewTravelpoint = 212,
 	NewExit = 214,
 	NewTeleportStone = 225,
 	NewExpeditionExit = 247,
 	NewExpeditionNarrator = 248,
 	NewQuestGiverObject = 252,
 	NewMountObject = 306,
 	NewExpeditionAgent = 309,
 	NewExpeditionCheckPoint = 310,
 	NewPortalEntrance = 317,
 	NewPortalExit = 318,
 	NewRandomDungeonExit = 319,
 	NewFloatObject = 354,
 	NewOutpostObject = 360,
 	NewUnlockedPersonalSeasonRewards = 370,
 	NewLocationMarker = 378,
 	NewTutorialBlocker = 379,
 	NewTileSwitch = 380,
 	NewInformationProvider = 381,
 	NewDynamicGuildLogo = 382,
 	NewDecoration = 383,
 	NewLootChest = 387,
 	NewShrine = 391,
 	NewRandomResourceBlocker = 438,
 	NewHomeObject = 439,
 	NewTunnelExit = 443,
 	NewCorruptedShrine = 453,
 	NewTunnelExitTemp = 517,
 	NewCagedObject = 525,
 	NewUnrestrictedPvpZone = 537,
 	NewDynamicCompound = 562,
 	NewBuildingBaseEvent = 577,
 	NewPiledObject = 603,
 	NewKillTrophyFurnitureBuilding = 612,
 	NewTileSwitchTrigger = 614,
 	NewMultiRewardObject = 615,
    */

void PacketAnalyze::analyzeCommand(
    GLFWwindow* window,
    NetworkCommand& command,
    std::vector<uint8_t>& packetHeader,
    NetworkPacketInfo& packetInfo,
    bool isHikingMode
)
{
    DataLayout dataLayout{};

    //std::vector<uint8_t>& raw = command.rawNetworkCommand();
    //std::string rawStr(raw.begin(), raw.end());
    //if (command.size() != 67 and !std::isElementInVector(nCodes, command.getEventCode())) {
    //    //if (rawStr.find((char*)"5369676F75") != std::string::npos) {
    //        dataLayout.findDataLayout(command);
    //        dataLayout.printInfo(command);
    //    //}
    //    // 544e4c TNL
    //    //dataLayout.printInfo(command, 0, 255, true);
    //}
    
    if (command.getOperationType() == operationType::event) {
        if (isHikingMode) {
            if (entityList._isChangingLocation) {

            }
            else {

            }

            switch (command.getEventCode()) {
            case eventCode::HealthUpdate:
                //entityList._playerList.update(HealthUpdateHandler(*this)); // need to add health handling
                entityList._currentLocation._mobList.update(HealthUpdateHandler(command));
                break;
            case eventCode::NewSimpleHarvestableObjectList:
                /*dataLayout.findDataLayout(command);
                dataLayout.printInfo(command);
                command.printCommandInOneString();*/
                entityList._currentLocation._harvestableList.update(HarvestableList(command));
                break;
            case eventCode::NewHarvestableObject:
                entityList._currentLocation._harvestableList.update(Harvestable(command));
                break;
            case eventCode::HarvestableChangeState:
                entityList._currentLocation._harvestableList.updateState(command);
                break;
            case eventCode::MobChangeState:
                entityList._currentLocation._mobList.mobChangeState(command);
                //entityList._playerList.update(Player::playerMove(command)); 
                break;
            case eventCode::NewMob:
                entityList._currentLocation._mobList.newMob(Mob(command));
                break;
            case eventCode::NewFishingZoneObject:
                entityList._currentLocation._fishNodeList.update(FishNode(command));
                break;
            case eventCode::HarvestStart:
                //Harvestable::harvestStart(command);
                break;
            case eventCode::HarvestFinished:
                Harvestable::harvestFinished(command);
                break;
            case 66: // crafting finished on station
                break;
            case eventCode::NewCharacter:
                //if (entityList._currentLocation._type != "city") {
                    entityList._currentLocation._playerList.newPlayer(Player(command));
                //}
                break;
            case eventCode::Leave:
                entityList._currentLocation._playerList.playerLeave(Player(command));
                break;
            case eventCode::PlayerMove:
                //entityList._currentLocation._playerList.update(Player::playerMove(command)); 
                break;
            case eventCode::Teleport:
                //dataLayout.findDataLayout(command);
                //dataLayout.printInfo(command);
            /*case eventCode::BuyJourney:
                dataLayout.findDataLayout(command);
                dataLayout.printInfo(command); */
                break;
            case eventCode::NewSimpleItem:
                /*dataLayout.findDataLayout(command);
                dataLayout.printInfo(command);*/
                break;
            default:
                break;
            }
            if (command.rawNetworkCommand().size() == 67/* and _networkCommand[66] & (2 << 0)*/) {
                if (dataLayout.findNumOfFragments(command) == 2) {
                    //EntityMove::updateEntityListMove(command, entityList._currentLocation._playerList);
                    EntityMove::updateEntityListMove(command, entityList._currentLocation._mobList);
                }
                else {
                    /*dataLayout.findDataLayout(command);
                    dataLayout.printInfo(command);*/
                }
            }
            else {
                /*dataLayout.findDataLayout(command);
                dataLayout.printInfo(command);*/
            }
        }
    }
    if (command.getOperationType() == operationType::response) {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point stop;
        //dataLayout.findDataLayout(command);
        //dataLayout.printInfo(command);
        /*std::cout << "commandChainID: " << this->getCommandID() << " " <<
                     "event code: " << command.getEventCode() << "\n";*/
        bool isSafePortal = false;
        switch (command.getEventCode()) {
        case operationCode::Join:
            entityList.endChangeLocation(command, false);
            Location::findLocationData(entityList._currentLocation);
            if (entityList._previousLocation._tier == 0) {
                Location::findLocationData(entityList._previousLocation);
            }
            //std::cout << "location type: " << entityList._currentLocation._type << "\n";
            // S.A.F.E portal
            isSafePortal = (entityList._previousLocation._type == "city" || entityList._previousLocation._type == "world") && entityList._currentLocation._type == "world";
            if (!isSafePortal) {
                net::makeLocationsConnection(
                    entityList._previousLocation,
                    entityList._currentLocation,
                    entityList._isChangingLocation
                );
            }
            net::updatePlayerData(entityList._currentLocation, "player_data.json");
            entityList._isChangingLocation = false;
            break;
        case operationCode::Move:
            //if (entityList._currentLocation._type != "city") {
                entityList._playerSelf.update(command);
            //}
            break;
            /*case operationCode::ChangeCluster: // other player changing location not only me
                entityList.ChangeCluster(command);
                break;*/
        case operationCode::AuctionSellOrders:
            Auction::findAuctionOrders(command, entityList._currentLocation, true);
            break;
        case operationCode::AuctionBuyOrders:
            Auction::findAuctionOrders(command, entityList._currentLocation, true);
            break;
        case operationCode::RealEstateGetAuctionData:
            break;
        case operationCode::AuctionGetFinishedAuctions: // non standard format
            Auction::findAuctionOrders(command, entityList._currentLocation, false);
            break;
        case operationCode::AuctionGetFinishedAuctionsCount: // non standard format
            break;
        case operationCode::AuctionGetMyOpenAuctions: // non standard format
            Auction::findAuctionOrders(command, entityList._currentLocation, false);
            break;
        case operationCode::AuctionGetItemAverageStats:
            //start = std::chrono::high_resolution_clock::now();
            Auction::findAuctionAverageValues(command, itemData, ",");
            /*stop = std::chrono::high_resolution_clock::now();
            std::cout <<
                "time to write acution average values: " <<
                std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";*/
            break;
        case operationCode::GetRankings:
            //Rankings::getRankings(command);
        case operationCode::GetClusterMapInfo:
            //MapCluster::findClusterData(command);
            break;
        default:
            break;
        }
        //std::cout << this->getEventCode() << "\n";
    }
    if (command.getOperationType() == operationType::request) {
        switch (command.getEventCode()) {
        case operationCode::AuctionGetItemAverageStats:
            // packetInfo.to_string();
            // NetworkCommand::printCommandInOneString(networkPacketHeader, false, true);
            Auction::GetItemData(command, entityList._currentLocation, itemData, false);
            break;
        case operationCode::Move:
            //if (entityList._currentLocation._type != "city") {
                entityList._playerSelf.update(command);
            //}
            //std::cout << entityList._playerSelf._positionX << " " << entityList._playerSelf._positionY << "\n";
            break;
        case operationCode::ChangeCluster:
            entityList.beginChangeLocation(command, false);
            entityList._isChangingLocation = true;
            break;
        case operationCode::AuctionBuyOrders:
            break;
        default:
            break;
        }
    }

    if (command.getOperationType() == operationType::not_defined) {
    }
}

int main() {
    PacketAnalyze packetAnalyze;

    //net::findUniqueValues(net::mobsData, "Mobs", "Mob", "@mobtypecategory");
    //net::findUniqueValues(net::mobsData, "Mobs", "Mob", "@category");

    //net::findUniqueValues(net::harvestablesData, "AO-Harvestables", "Harvestable", "@name");
    //net::findUniqueValues(net::mobsData, "Mobs", "Mob", "@uniquename");
    //net::findUniqueValuesByCriterion(net::mobsData, "Mobs", "Mob", "@uniquename", "@category");

    //sortMobDescriptions(mobDescriptions);
    //net::formatItemsData();
    //net::searchLocationsTemplates(-349.5, 122.5);
    //net::parseObjectsFromTemplate("templates/DEAD/618_L1_M3_S5.template.xml");
    //net::parseLocationsAndConnections("world_extended.xml");
    if (_isHikingMode) {
        net::removeOutdatedTemporaryConnections("location_connections.json");
    }
    net::findLocationsStatistics("cluster");
    //packetAnalyze.run();

    //packetAnalyze.outputColorizedNetworkPacket(text);
    
    //for (size_t j = 0; j < 10; j++) {
    //    auto start = std::chrono::high_resolution_clock::now();
    //    LARGE_INTEGER count;
    //    size_t iterations = 20000000;
    //    for (size_t i = 0; i < iterations; ++i) {
    //        std::hex;
    //        std::cout << "";
    //        //QueryPerformanceCounter(&count);
    //        std::dec;
    //    }
    //    auto stop = std::chrono::high_resolution_clock::now();
    //    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / iterations 
    //        << "\n";
    //}

    return 0;
}