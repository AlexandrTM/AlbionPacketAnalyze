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

FragmentedNetworkCommand::iterator FragmentedNetworkCommand::begin() {
	return _fragmentedNetworkCommand.begin();
}

FragmentedNetworkCommand::iterator FragmentedNetworkCommand::end() {
	return _fragmentedNetworkCommand.end();
}

FragmentedNetworkCommand::iterator FragmentedNetworkCommand::erase(iterator it) {
	return _fragmentedNetworkCommand.erase(it);
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

bool FragmentedNetworkCommand::isNextCommandInChain(NetworkCommand& a, NetworkCommand& b) const
{
	uint32_t a_commandIndexInChain = a.getCommandIndexInChain();
	uint32_t b_commandIndexInChain = b.getCommandIndexInChain();
	return (a_commandIndexInChain + 1 == b_commandIndexInChain) ? true : false;
}

void FragmentedNetworkCommand::sort()
{
	std::sort(_fragmentedNetworkCommand.begin(), _fragmentedNetworkCommand.end(), 
		[this](const NetworkCommand& a, const NetworkCommand& b) -> bool {
			return a.getCommandIndexInChain() < b.getCommandIndexInChain();
		});
	/*for (size_t i = 0; i < _fragmentedNetworkCommand.size(); i++) {
		std::cout << (unsigned)_fragmentedNetworkCommand[i].getCommandIndexInChain() << " ";
	}
	std::cout << "\n";*/
}

void FragmentedNetworkCommand::connectFragments()
{
	for (size_t i = 1; i < _fragmentedNetworkCommand.size(); i++) {
		_fragmentedNetworkCommand[0] += NetworkCommand(_fragmentedNetworkCommand[i], 32);
	}
}

NetworkCommand& FragmentedNetworkCommand::operator[](size_t elementIndex)
{
	return _fragmentedNetworkCommand[elementIndex];
}

const NetworkCommand& FragmentedNetworkCommand::operator[](size_t elementIndex) const
{
	return _fragmentedNetworkCommand[elementIndex];
}
