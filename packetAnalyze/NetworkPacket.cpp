#include "pch.h"
#include "NetworkPacket.h"


NetworkPacket::NetworkPacket()
{
    _networkPacket = { {} };
    _packetTime = 0;
}

void NetworkPacket::addCommandsFromPacket(std::vector<uint8_t> rawPacket)
{
    uint8_t commandsNumInPacket = rawPacket[3];
    ptrdiff_t stringPosition = _packetHeaderSize;
    for (uint8_t i = 0; i < commandsNumInPacket; i++) {
        uint16_t commandLength = (rawPacket[stringPosition + 6] << 8) + rawPacket[stringPosition + 7];
        _networkPacket.push_back(NetworkCommand({ rawPacket.begin() + stringPosition,
                                    rawPacket.begin() + stringPosition + commandLength }));
        stringPosition += commandLength;
    }
}
NetworkPacket NetworkPacket::findCommandsInPacket(std::vector<uint8_t> rawPacket)
{
    NetworkPacket networkPacket;

    uint8_t commandsNumInPacket = rawPacket[3];
    ptrdiff_t stringPosition = _packetHeaderSize;
    for (uint8_t i = 0; i < commandsNumInPacket; i++) {
        uint16_t commandLength = (rawPacket[stringPosition + 6] << 8) + rawPacket[stringPosition + 7];
        if (rawPacket.begin() + stringPosition + commandLength < rawPacket.end()) {
            networkPacket.push_back(NetworkCommand({ rawPacket.begin() + stringPosition,
                                        rawPacket.begin() + stringPosition + commandLength }));
            stringPosition += commandLength;
        }
        else if ((rawPacket.end() - (rawPacket.begin() + stringPosition)) > 0) {
            networkPacket.push_back(NetworkCommand({rawPacket.begin() + stringPosition, rawPacket.end()}));
        }
        else {
            break;
        }
    }
    networkPacket.findPacketTime(rawPacket);
    return networkPacket;
}
void NetworkPacket::findPacketTime(std::vector<uint8_t> rawPacket)
{
    if (rawPacket.size() >= 8) {
        _packetTime = net::read_int32(rawPacket, 4);
    }
    else {
        _packetTime = 0;
    }
}

NetworkCommand& NetworkPacket::operator[](size_t elementIndex)
{
    return _networkPacket[elementIndex];
}
NetworkPacket& NetworkPacket::operator+=(NetworkPacket packet)
{
    for (size_t i = 0; i < packet.size(); i++) {
        _networkPacket.push_back(packet[i]);
    }
    return *this;
}

void NetworkPacket::push_back(NetworkCommand command)
{
    _networkPacket.push_back(command);
}
uint16_t NetworkPacket::size()
{
    return _networkPacket.size();
}
void NetworkPacket::clear()
{
    _networkPacket.clear();
    _packetTime = 0;
}