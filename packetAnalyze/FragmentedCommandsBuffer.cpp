#include "pch.h"

FragmentedCommandsBuffer::FragmentedCommandsBuffer()
{
    _fragmentedCommandsBuffer = {};
}

FragmentedCommandsBuffer::iterator FragmentedCommandsBuffer::begin() {
    return _fragmentedCommandsBuffer.begin();
}

FragmentedCommandsBuffer::iterator FragmentedCommandsBuffer::end() {
    return _fragmentedCommandsBuffer.end();
}

FragmentedCommandsBuffer::iterator FragmentedCommandsBuffer::erase(iterator it) {
    return _fragmentedCommandsBuffer.erase(it);
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

FragmentedNetworkCommand& FragmentedCommandsBuffer::operator[](size_t elementIndex)
{
    return _fragmentedCommandsBuffer[elementIndex];
}
const FragmentedNetworkCommand& FragmentedCommandsBuffer::operator[](size_t elementIndex) const
{
    return _fragmentedCommandsBuffer[elementIndex];
}
