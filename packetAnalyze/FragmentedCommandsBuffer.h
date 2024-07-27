#ifndef FRAGMENTED_COMMANDS_BUFFER_H
#define FRAGMENTED_COMMANDS_BUFFER_H

class FragmentedCommandsBuffer
{
public:
    FragmentedCommandsBuffer();

    bool isNewFragmentedCommand(NetworkCommand& networkCommand) const;
    void addCommandFragment(NetworkCommand& networkCommand);

    FragmentedNetworkCommand& operator[](size_t elementIndex);
    const FragmentedNetworkCommand& operator[](size_t elementIndex) const;
    inline void push_back(FragmentedNetworkCommand&& fragmentedNetworkCommand);
    inline void push_back(FragmentedNetworkCommand& fragmentedNetworkCommand);
    inline uint32_t size() const;

private:
    std::vector<FragmentedNetworkCommand> _fragmentedCommandsBuffer;
};

inline void FragmentedCommandsBuffer::push_back(FragmentedNetworkCommand&& fragmentedNetworkCommand)
{
    _fragmentedCommandsBuffer.push_back(std::move(fragmentedNetworkCommand));
}
inline void FragmentedCommandsBuffer::push_back(FragmentedNetworkCommand& fragmentedNetworkCommand)
{
    _fragmentedCommandsBuffer.push_back(fragmentedNetworkCommand);
}
inline uint32_t FragmentedCommandsBuffer::size() const
{
    return _fragmentedCommandsBuffer.size();
}

#endif // !FRAGMENTED_COMMANDS_BUFFER_H