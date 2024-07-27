#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Tins;

typedef std::vector<uint8_t> RawNetworkPacket;

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
bool PacketAnalyze::findStringInString(std::string packet, std::string string, size_t& stringPosition)
{
    for (size_t i = 0; i < packet.length() - string.length() + 1; i += 2) {
        if (packet.substr(i, string.length()) == string) {
            return true;
        }
    }

    return false;
}
bool PacketAnalyze::findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string)
{
    for (size_t i = regionStart; i < regionEnd - string.length() + 1; i += 2) {
        if (packet.substr(i, string.length()) == string) {
            return true;
        }
    }

    return false;
}
bool PacketAnalyze::findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string,
    size_t& stringPosition)
{
    for (size_t i = regionStart; i < regionEnd - string.length() + 1; i += 2) {
        if (packet.substr(i, string.length()) == string) {
            stringPosition = i;
            return true;
        }
    }

    return false;
}

int counter;
int filteredCommands;

std::vector<std::vector<uint16_t>> commandLenghts;
std::vector <NetworkPacket> text;
int _windowPosX, _windowPosY;
GLint _screenWidth, _screenHeight;

uint8_t _mapState = mapState::fullscreenMap;

void PacketAnalyze::run()
{
    initWindow();
    initSniffer();
    mainLoop();
    cleanup();
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
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

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

void PacketAnalyze::mainLoop()
{
    while (!glfwWindowShouldClose(_window)) {
        sniffPacket();

        glfwPollEvents();
    }
}

void PacketAnalyze::cleanup()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
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
    _window = glfwCreateWindow(_videoMode->width - 1, _videoMode->height - 1,
        u8"Packet Analyze", nullptr, nullptr);
    glfwGetWindowSize(_window, &_screenWidth, &_screenHeight);
    glViewport(0, 0, _screenWidth, _screenHeight);
    //glfwSetWindowPos(_window, 0, 0);

    GLFWimage images[1];
    images[0].pixels = stbi_load("mineral_icon.jpg", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(_window, 2, images);
    stbi_image_free(images[0].pixels);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    glfwSetCursorEnterCallback(_window, cursorEnterCallback);
    glfwSetKeyCallback(_window, keyCallback);
    glfwMakeContextCurrent(_window);
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

void PacketAnalyze::framebufferResizeCallback(GLFWwindow* _window, int width, int height) {
    auto app = reinterpret_cast<PacketAnalyze*>(glfwGetWindowUserPointer(_window));
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
    }
}
    
void PacketAnalyze::initSniffer()
{
    SnifferConfiguration albionConfig;
    //albionConfig.set_filter("ip dst 192.168.1.70");
    //albionConfig.set_promisc_mode(true);
    _sniffer = Sniffer(_iface.name(), albionConfig);
}
bool PacketAnalyze::isPacketFiltered(RawNetworkPacket& filteredPacket)
{
    PDU* sniffedPacket = _sniffer.next_packet();

    if (sniffedPacket) {
        const IP& ip = sniffedPacket->rfind_pdu<IP>();
        const UDP& udp = sniffedPacket->rfind_pdu<UDP>();

        if (_albionIPRange.contains(ip.src_addr()) and (udp.sport() == 5056 /*or udp.dport() == 5056*/)) {
            RawPDU rawPacket = sniffedPacket->rfind_pdu<RawPDU>();
            RawNetworkPacket packet;
            readRawPacket(rawPacket, packet);

            if (packet.size() > 0) {
                filteredPacket = packet;
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
        if (isPacketFiltered(packet)) {
            //auto start = std::chrono::high_resolution_clock::now();

            analyzePacket(packet); 

            //auto stop = std::chrono::high_resolution_clock::now();
            //std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
        }
    }
    catch (std::exception& e) {
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

void PacketAnalyze::analyzePacket(RawNetworkPacket rawPacket)
{
    _packet = NetworkPacket::findCommandsInPacket(rawPacket);

    //if (eventCode == eventCodes[counter]) 
    for (size_t i = 0; i < _packet.size(); i++)
    {
        //_packet[i].printCommandInOneString();
        if (_packet[i].getCommandType() == commandType::fragmented) {
            if (_fragmentedCommandsBuffer.isNewFragmentedCommand(_packet[i])) {
                _fragmentedCommandsBuffer.push_back(FragmentedNetworkCommand(_packet[i]));
            }
            else {
                _fragmentedCommandsBuffer.addCommandFragment(_packet[i]);
            }
            for (size_t j = 0; j < _fragmentedCommandsBuffer.size(); j++) {
                if (_fragmentedCommandsBuffer[j].isCommandFull()) {
                    _fragmentedCommandsBuffer[j].sort();

                    /*for (size_t j = 0; j < _fragmentedCommandsBuffer.size(); j++) {
                        _fragmentedCommandsBuffer[j].fillFragmentedCommand(_packet[i]);
                        if (_fragmentedCommandsBuffer[j].isCommandFull()) {
                            _fragmentedCommandsBuffer[j].analyzeCommand(_window);
                            _fragmentedCommandsBuffer.erase(_fragmentedCommandsBuffer.begin() + j);
                            break;
                        }
                        if (_fragmentedCommandsBuffer.size() > 0 and
                            j == _fragmentedCommandsBuffer.size() - 1) {
                            std::cout << "true" << "\n";
                        }
                    }*/
                }
            }
                
        }
        if (_packet[i].getCommandType() == commandType::reliable
         or _packet[i].getCommandType() == commandType::unreliable) {

            //_packet[i].printCommandInOneString();
            _packet[i].analyzeCommand(_window);
            //findUniqueEventCodes(_packet[i]);
        }
    }
}

int main() {
    PacketAnalyze packetAnalyze;
    packetAnalyze.run();

    packetAnalyze.outputColorizedNetworkPacket(text);

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