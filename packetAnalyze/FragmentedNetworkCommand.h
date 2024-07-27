#ifndef FRAGMENTED_NETWORK_COMMAND_H
#define FRAGMENTED_NETWORK_COMMAND_H

class FragmentedNetworkCommand
{
public:
	FragmentedNetworkCommand();

	inline void push_back(NetworkCommand& networkCommand);

	const NetworkCommand& operator[](size_t elementIndex) const;

private:
	std::vector<NetworkCommand> _fragmentedNetworkCommand;

};

#endif