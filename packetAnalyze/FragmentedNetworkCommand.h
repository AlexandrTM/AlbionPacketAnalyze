#ifndef FRAGMENTED_NETWORK_COMMAND_H
#define FRAGMENTED_NETWORK_COMMAND_H

class FragmentedNetworkCommand
{
public:
	FragmentedNetworkCommand();
	FragmentedNetworkCommand(NetworkCommand& networkCommand);

	using iterator = std::vector<NetworkCommand>::iterator;

	iterator begin();
	iterator end();
	iterator erase(iterator it);

	bool isCommandFull() const;
	bool isNextCommandInChain(NetworkCommand& a, NetworkCommand& b) const;
	void sort();
	void connectFragments();

	inline void push_back(NetworkCommand& networkCommand);
	NetworkCommand& operator[](size_t elementIndex);
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