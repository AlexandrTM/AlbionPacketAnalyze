#include "Enumerations.h"

#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

static const size_t _networkPacketHeaderSize = 12;

class NetworkPacket
{
private:
    std::vector<NetworkCommand> _networkPacket;
    uint32_t _packetTime;

public:
    std::vector<uint8_t> _packetHeader;
    NetworkPacketInfo _packetInfo;

    NetworkPacket();

    void addCommandsFromPacket(std::vector<uint8_t> rawPacket);
    static NetworkPacket findCommandsInPacket(std::vector<uint8_t> rawPacket);
    void findPacketTime(std::vector<uint8_t> rawPacket);

    NetworkCommand& operator[](size_t elementIndex);
    NetworkPacket& operator+=(NetworkPacket packet);

    uint16_t size();
    void push_back(NetworkCommand command);
    void clear();
};

#endif