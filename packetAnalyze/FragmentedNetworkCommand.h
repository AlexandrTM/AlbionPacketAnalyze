#ifndef FRAGMENTED_NETWORK_COMMAND_H
#define FRAGMENTED_NETWORK_COMMAND_H

class FragmentedNetworkCommand
{
public:
	FragmentedNetworkCommand();
	FragmentedNetworkCommand(NetworkCommand& networkCommand);

	bool isCommandFull() const;

	inline void push_back(NetworkCommand& networkCommand);
	const NetworkCommand& operator[](size_t elementIndex) const;

private:
	std::vector<NetworkCommand> _fragmentedNetworkCommand;

	bool _isCommandFull;
};

inline void FragmentedNetworkCommand::push_back(NetworkCommand& networkCommand)
{
	_fragmentedNetworkCommand.push_back(networkCommand);
}

#endif