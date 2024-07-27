#include "pch.h"

FragmentedCommandsBuffer::FragmentedCommandsBuffer()
{
    _fragmentedCommandsBuffer = {};
}

bool FragmentedCommandsBuffer::isNewFragmentedCommand(NetworkCommand& networkCommand) const
{
    for (size_t i = 0; i < _fragmentedCommandsBuffer.size(); i++) {
        if (_fragmentedCommandsBuffer[i][0].getCommandChainID() ==
            networkCommand.getCommandChainID()) {
            return false;
        }
    }
    return true;
}

void FragmentedCommandsBuffer::addCommandFragment(NetworkCommand& networkCommand)
{
    for (size_t i = 0; i < _fragmentedCommandsBuffer.size(); i++) {
        if (_fragmentedCommandsBuffer[i][0].getCommandChainID() == networkCommand.getCommandChainID()) {
            _fragmentedCommandsBuffer[i].push_back(networkCommand);
            break;
        }
    }
}

const FragmentedNetworkCommand& FragmentedCommandsBuffer::operator[](size_t elementIndex) const
{
    return _fragmentedCommandsBuffer[elementIndex];
}
