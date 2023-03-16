#ifndef PACKET_ANALYZE_H
#define PACKET_ANALYZE_H


class PacketAnalyze {

public:
    void run();

    std::vector<bool> findSameSymbolsInText(NetworkPacket paragraph);
    void colorizeSameText(NetworkPacket paragraph, HANDLE consoleHandle);
    void outputColorizedNetworkPacket(std::vector<NetworkPacket> text);

    void findUniqueEventCodes(NetworkCommand& command);

private:
    GLFWwindow* _window;
    bool framebufferResized;
    std::vector<std::size_t> _amountOfSameCommands;
    Tins::IPv4Range albionIPRange = Tins::IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    Tins::NetworkInterface iface = Tins::NetworkInterface::default_interface();
    Tins::Sniffer sniffer = Tins::Sniffer(iface.name());

    NetworkPacket _packet;
    NetworkCommand _fragmentedCommandBuffer;

    std::vector<size_t> findCommandBordersInPacket(std::string packet);
    bool findStringInString(std::string packet, std::string string, size_t& stringPosition);
    bool findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string);
    bool findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string,
        size_t& stringPosition);

    void mainLoop();

    void cleanup();

    void initWindow();

    static void changeMapState(GLFWwindow* window);

    static void framebufferResizeCallback(GLFWwindow* _window, int width, int height);
    static void cursorEnterCallback(GLFWwindow* window, int entered);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void initSniffer();
    bool isPacketFiltered(std::vector<uint8_t>& filteredPacket);
    void sniffPacket();

    void readRawPacket(Tins::RawPDU pdu, std::vector<uint8_t>& rawPacketPayload);
    void readRawPacket(Tins::RawPDU pdu, size_t regionStart, size_t regionEnd, std::vector<uint8_t>& rawPacketPayload);
    std::vector<uint8_t> readRawPacket(Tins::RawPDU pdu, size_t regionStart, size_t regionEnd);

    void analyzePacket(std::vector<uint8_t> rawPacket);
};

#endif // !PACKET_ANALYZE_H