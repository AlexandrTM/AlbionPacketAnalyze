#ifndef PACKET_ANALYZE_H
#define PACKET_ANALYZE_H

#include "Enumerations.h"
#include "entity_list_instance.h"

class PacketAnalyze {

public:
    void run();

    std::vector<bool> findSameSymbolsInText(NetworkPacket paragraph);
    void colorizeSameText(NetworkPacket paragraph, HANDLE consoleHandle);
    void outputColorizedNetworkPacket(std::vector<NetworkPacket> text);

    void findUniqueEventCodes(NetworkCommand& command);

private:
    GLFWwindow* _window;
    bool _framebufferResized;
    std::vector<std::size_t> _amountOfSameCommands;
    Tins::IPv4Range _albionIPRange = Tins::IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    Tins::NetworkInterface _iface = Tins::NetworkInterface::default_interface();
    Tins::Sniffer _sniffer = Tins::Sniffer(_iface.name());

    NetworkPacket _packet;
    FragmentedCommandsBuffer _fragmentedCommandsBuffer;

    std::string itemData = "";
    size_t counter = 0;

    std::vector<size_t> findCommandBordersInPacket(std::string packet);
    bool findStringInStringImpl(
        const std::string& string_1,
        const std::string& string_2,
        size_t regionStart,
        size_t regionEnd,
        size_t* stringPosition
    );
    bool findStringInString(std::string string_1, std::string string_2, size_t& stringPosition);
    bool findStringInString(
        std::string string_1, std::string string_2,
        size_t regionStart, size_t regionEnd
    );
    bool findStringInString(
        std::string string_1, std::string string_2,
        size_t regionStart, size_t regionEnd,
        size_t& stringPosition
    );

    void mainLoop();

    void cleanup();

    void initWindow();

    static void changeMapState(GLFWwindow* window);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void cursorEnterCallback(GLFWwindow* window, int entered);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void initSniffer();
    bool isPacketFiltered(RawNetworkPacket& filteredPacket, NetworkPacketInfo& packetInfo);
    void sniffPacket();
    void sendPacket();

    void readRawPacket(Tins::RawPDU pdu, RawNetworkPacket& rawPacketPayload);
    void readRawPacket(Tins::RawPDU pdu, size_t regionStart, size_t regionEnd, 
        RawNetworkPacket& rawPacketPayload);
    std::vector<uint8_t> readRawPacket(Tins::RawPDU pdu, size_t regionStart, size_t regionEnd);

    void analyzePacket(RawNetworkPacket rawPacket, NetworkPacketInfo& packetInfo);

    void analyzeCommand(
        GLFWwindow* window,
        NetworkCommand& command,
        std::vector<uint8_t>& packetHeader,
        NetworkPacketInfo& packetInfo,
        bool isHikingMode
    );
};

#endif // !PACKET_ANALYZE_H