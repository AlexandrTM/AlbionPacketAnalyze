#include "pch.h"

FragmentedNetworkCommand::FragmentedNetworkCommand()
{
	_fragmentedNetworkCommand = {};
}

inline void FragmentedNetworkCommand::push_back(NetworkCommand& networkCommand)
{
	_fragmentedNetworkCommand.push_back(networkCommand);
}

const NetworkCommand& FragmentedNetworkCommand::operator[](size_t elementIndex) const
{
	return _fragmentedNetworkCommand[elementIndex];
}
