#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

class NetworkPacket
{
private:
    std::vector<NetworkCommand> _networkPacket;
    uint32_t _packetTime;

    static const size_t _packetHeaderSize = 12;

public:
    NetworkPacket();

    void addCommandsFromPacket(std::vector<uint8_t> rawPacket);
    static NetworkPacket findCommandsInPacket(std::vector<uint8_t> rawPacket);
    uint32_t findPacketTime();

    NetworkCommand& operator[](size_t elementIndex);
    NetworkPacket& operator+=(NetworkPacket packet);

    uint16_t size();
    void push_back(NetworkCommand command);
    void clear();
};

#endif