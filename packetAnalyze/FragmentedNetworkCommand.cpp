#include "pch.h"

FragmentedNetworkCommand::FragmentedNetworkCommand()
{
	_fragmentedNetworkCommand = {};
}

FragmentedNetworkCommand::FragmentedNetworkCommand(NetworkCommand& networkCommand)
{
	_fragmentedNetworkCommand = { networkCommand };
}

const NetworkCommand& FragmentedNetworkCommand::operator[](size_t elementIndex) const
{
	return _fragmentedNetworkCommand[elementIndex];
}
