//#ifndef PACKET_ANALYZE_H
//#define PACKET_ANALYZE_H
//
////class PacketAnalyze {
////
////public:
////    void run()
////    {
////        initWindow();
////        initSniffer();
////        mainLoop();
////        cleanup();
////    }
////
////    std::vector<bool> findSameSymbolsInText(NetworkPacket paragraph);
////    void colorizeSameText(NetworkPacket paragraph, HANDLE consoleHandle);
////    void outputColorizedNetworkPacket(std::vector<NetworkPacket> text);
////
////    void findUniqueEventCodes(NetworkCommand& command);
////
////private:
////    bool framebufferResized;
////    std::vector<std::size_t> _amountOfSameCommands;
////
////    NetworkPacket _packet;
////    NetworkCommand _fragmentedCommandBuffer;
////
////
////    std::vector<size_t> findCommandBordersInPacket(std::string packet);
////    bool findStringInString(std::string packet, std::string string, size_t& stringPosition);
////    bool findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string);
////    bool findStringInString(std::string packet, size_t regionStart, size_t regionEnd, std::string string,
////        size_t& stringPosition);
////
////    void mainLoop();
////
////    void cleanup();
////
////    void initWindow();
////
////    static void changeMapState();
////
////    static void framebufferResizeCallback(GLFWwindow* _window, int width, int height);
////    static void cursorEnterCallback(GLFWwindow* window, int entered);
////    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
////
////    void initSniffer();
////    bool isPacketFiltered(RawNetworkPacket& filteredPacket);
////    void sniffPacket();
////
////    void readRawPacket(RawPDU pdu, RawNetworkPacket& rawPacketPayload);
////    void readRawPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, RawNetworkPacket& rawPacketPayload);
////    RawNetworkPacket readRawPacket(RawPDU pdu, size_t regionStart, size_t regionEnd);
////
////
////    void analyzePacket(RawNetworkPacket rawPacket);
////};
//
//#endif // !PACKET_ANALYZE_H