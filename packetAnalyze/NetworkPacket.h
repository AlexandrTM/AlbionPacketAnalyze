#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

class NetworkPacket
{
private:
    std::vector<NetworkCommand> _networkPacket;
    uint32_t _packetTime;

    size_t _packetHeaderSize = 12;

public:
    NetworkPacket();

    NetworkPacket findCommandsInPacket(NetworkCommand rawPacket);
    uint32_t findPacketTime();

    NetworkCommand& operator[](size_t elementIndex);
    NetworkPacket& operator+=(NetworkPacket packet);

    uint16_t size();
    void push_back(NetworkCommand command);
};

#endif