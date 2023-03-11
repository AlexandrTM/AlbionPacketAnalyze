#include "pch.h"
#include "NetworkPacket.h"


NetworkPacket::NetworkPacket()
{
    _networkPacket = { {} };
    _packetTime = findPacketTime();
}

NetworkPacket NetworkPacket::findCommandsInPacket(NetworkCommand rawPacket)
{
    NetworkPacket _commandsInPacket;
    uint8_t _commandsNumInPacket = rawPacket[3];

    ptrdiff_t _stringPosition = _packetHeaderSize;
    for (uint8_t i = 0; i < _commandsNumInPacket; i++) {
        uint16_t _commandLength = ((rawPacket[_stringPosition + 6] << 8) + rawPacket[_stringPosition + 7]) & 0x0fff;
        _commandsInPacket.push_back(NetworkCommand({ (uint8_t)(rawPacket[0] + _stringPosition),
                                    (uint8_t)(rawPacket[0] + _stringPosition + _commandLength) }));
        _stringPosition += _commandLength;
    }
    return _commandsInPacket;
}
uint32_t NetworkPacket::findPacketTime()
{
    if (_networkPacket[0].size() >= 8) {
        return (_networkPacket[0][4] << 24) + (_networkPacket[0][5] << 16) + (_networkPacket[0][6] << 8) + _networkPacket[0][7];
    }
    else {
        return 0;
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
}

uint16_t NetworkPacket::size()
{
    return _networkPacket.size();
}
void NetworkPacket::push_back(NetworkCommand command)
{
    _networkPacket.push_back(command);
}
