#include "pch.h"
#include "NetworkPacket.h"


NetworkPacket::NetworkPacket()
{
    _networkPacket = { {} };
    _packetTime = 0;
}

void NetworkPacket::addCommandsFromPacket(std::vector<uint8_t> rawPacket)
{
    uint8_t _commandsNumInPacket = rawPacket[3];
    ptrdiff_t _stringPosition = _packetHeaderSize;
    for (uint8_t i = 0; i < _commandsNumInPacket; i++) {
        uint16_t _commandLength = ((rawPacket[_stringPosition + 6] << 8) + rawPacket[_stringPosition + 7]) & 0x0fff;
        _networkPacket.push_back(NetworkCommand({ rawPacket.begin() + _stringPosition,
                                    rawPacket.begin() + _stringPosition + _commandLength }));
        _stringPosition += _commandLength;
    }
}
NetworkPacket NetworkPacket::findCommandsInPacket(std::vector<uint8_t> rawPacket)
{
    NetworkPacket _networkPacket;

    uint8_t _commandsNumInPacket = rawPacket[3];
    ptrdiff_t _stringPosition = _packetHeaderSize;
    for (uint8_t i = 0; i < _commandsNumInPacket; i++) {
        uint16_t _commandLength = ((rawPacket[_stringPosition + 6] << 8) + rawPacket[_stringPosition + 7]) & 0x07ff;
        _networkPacket.push_back(NetworkCommand({ rawPacket.begin() + _stringPosition,
                                    rawPacket.begin() + _stringPosition + _commandLength }));
        _stringPosition += _commandLength;
    }
    _networkPacket.findPacketTime(rawPacket);
    return _networkPacket;
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
