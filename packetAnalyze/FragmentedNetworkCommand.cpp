#include "pch.h"

FragmentedNetworkCommand::FragmentedNetworkCommand()
{
	_fragmentedNetworkCommand = {};
	_isCommandFull = false;
}

FragmentedNetworkCommand::FragmentedNetworkCommand(NetworkCommand& networkCommand)
{
	_fragmentedNetworkCommand = { networkCommand };
	_isCommandFull = false;
}

bool FragmentedNetworkCommand::isCommandFull() const
{
	//uint8_t commandIndexInChain = _networkCommand[23];
	uint8_t commandsNumInChain = _fragmentedNetworkCommand[0][19];
	if (_fragmentedNetworkCommand.size() == commandsNumInChain) {
		return true;
	}
	else {
		return false;
	}
}

const NetworkCommand& FragmentedNetworkCommand::operator[](size_t elementIndex) const
{
	return _fragmentedNetworkCommand[elementIndex];
}
