#include "pch.h"

// **************************************************************************
// ============================== DataType ==================================
// **************************************************************************


uint8_t DataType::getDataTypeSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int8_list:
        return 1;
    case dataType::int8_string:
        return 1;
    case dataType::int16:
        return 2;
    case dataType::float32:
        return 4;
    case dataType::int32:
        return 4;
    case dataType::int64:
        return 8;
    case dataType::dictionary:
        return 0;
    case dataType::listOfType:
        return 0;
    default:
        return 1;
        break;
    }
}
uint8_t DataType::getDataTypeHeaderSize(uint8_t dataType)
{
    switch (dataType)
    {
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int16:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::dictionary:
        return 4;
    case dataType::int8_list:
        return 5;
    case dataType::int8_string:
        return 3;
    case dataType::listOfType:
        return 3;
    default:
        std::cout << "new data type!" << "\n";
        return 1;
        break;
    }
}
uint16_t DataType::getNumOfEntries(NetworkCommand& command, uint8_t dataType, ptrdiff_t offset)
{
    switch (dataType)
    {
    case dataType::int8:
        return 1;
    case dataType::uint8:
        return 1;
    case dataType::int16:
        return 1;
    case dataType::float32:
        return 1;
    case dataType::int32:
        return 1;
    case dataType::int64:
        return 1;
    case dataType::dictionary:
        return (command[offset + 3] << 8) | command[offset + 4];;
    case dataType::int8_list:
        return (command[offset + 3] << 8) | command[offset + 4];
    case dataType::int8_string:
        return (command[offset + 1] << 8) | command[offset + 2];
    case dataType::listOfType:
        return (command[offset + 1] << 8) | command[offset + 2];
    default:
        return 0;
        break;
    }
}

DataType::DataType()
{
    _size = 0;
    _headerSize = 0;
    _dataTypeId = 0;
}
DataType::DataType(uint8_t dataTypeSize, uint8_t dataTypeHeaderSize, uint8_t dataType)
{
    _size = dataTypeSize;
    _headerSize = dataTypeHeaderSize;
    _dataTypeId = dataType;
}


// **************************************************************************
// ============================== DataFragment ==============================
// **************************************************************************


ptrdiff_t DataFragment::findFragmentsNumOffset(NetworkCommand& command)
{
    switch (command.getCommandType())
    {
    case commandType::reliable:
        if (command.getOperationType() == operationType::response ||
            command.getOperationType() == operationType::binary) {
            return 19;
        }
        else {
            return 16;
        }
    case commandType::unreliable:
        return 20;
    case commandType::fragmented:
        return 39;
    default:
        std::cout << "command type not defined: " << command.getCommandType() << "\n";
        command.printCommandInOneString();
        return -1;
    }
}

DataFragment::DataFragment(uint8_t fragmentID, ptrdiff_t offset, uint16_t numOfEntries, DataType dataType)
{
    _fragmentID = fragmentID;
    _offset = offset;
    _numOfEntries = numOfEntries;
    _dataType = dataType;
}
DataFragment::DataFragment()
{
    _fragmentID = 0;
    _offset = std::numeric_limits<ptrdiff_t>::min();
    _numOfEntries = 0;
    _dataType = {};
}
void DataFragment::printInfo(NetworkCommand& command) const
{
    size_t commandSizeLength = std::to_string(command.size()).length();
    std::cout << "id: " << std::setw(commandSizeLength) << (unsigned)_fragmentID << " "
              << "of: " << std::setw(commandSizeLength) << (unsigned)_offset << " ";
}

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
void DataFragment::printFragmentInfo(NetworkCommand& command, size_t& currentStringPosition,
    bool printPayload) const
{
    // take dicionaty into account
    ptrdiff_t startOffset = _offset - _dataType._headerSize - 1;
    ptrdiff_t endOffset = _offset + _dataType._size * _numOfEntries;
    size_t fragmentLength = endOffset - startOffset + 3;

    /*std::cout << 
        "data type:" << (unsigned)_dataType._dataType << " " <<
        "size: " << (unsigned)_dataType._size << " " <<
        "header size: " << (unsigned)_dataType._headerSize << " " <<
        "offset: " << (unsigned)_offset << " " <<
        "num of entries: " << (unsigned)_numOfEntries << "\n";*/
    
    currentStringPosition += fragmentLength;
    if (currentStringPosition > 50) {
        std::cout << "\n";
        currentStringPosition = 0;
    }

    std::cout.setf(std::ios::hex, std::ios::basefield);
    // fragment id
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    if (_fragmentID < 16) {
        std::cout << "0";
    }
    std::cout << (unsigned)_fragmentID;
    SetConsoleTextAttribute(consoleHandle, 7);
    //std::cout << " ";
    // fragment data type
    SetConsoleTextAttribute(consoleHandle, 6);
    /*if (_dataType._dataType < 16) {
        std::cout << "0";
    }
    std::cout << (unsigned)_dataType._dataType;*/
    command.printCommandInOneString(_offset - _dataType._headerSize, _offset, false);
    SetConsoleTextAttribute(consoleHandle, 7);
    if (printPayload) {
        SetConsoleTextAttribute(consoleHandle, 7 | FOREGROUND_INTENSITY);

        /*if (_dataType._dataType == dataType::int8_string) {
            for (size_t i = _offset; i < endOffset; i++) {
                std::cout << (unsigned char)command[i];
            }
        }
        else {
            command.printCommandInOneString(_offset, endOffset, false);
        }*/
        if (_dataType._dataTypeId == dataType::float32) {
            std::cout << " " << net::read_float32(command, _offset);
        }
        else {
            command.printCommandInOneString(_offset, endOffset, false);
        }
        SetConsoleTextAttribute(consoleHandle, 7);
    }
    std::cout << " ";
    //std::cout << fragmentLength << "\n";
    std::cout.unsetf(std::ios::hex);
}


// **************************************************************************
// ============================== DataLayout ================================
// **************************************************************************


DataFragment& DataLayout::findFragment(uint8_t fragmentID)
{
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        if (_dataLayout[i]._fragmentID == (uint8_t)fragmentID) {
            while (_dataLayout[i]._dataType._dataTypeId == dataType::listOfType) {
                i += 1;
            }
            return _dataLayout[i];
        }
    }
    return _defaultDataFragment;
}

uint8_t DataLayout::findNumOfFragments(NetworkCommand& command)
{
    ptrdiff_t offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t numOfFragments = command[offset];

    return numOfFragments;
}
void DataLayout::findDataLayout(NetworkCommand& command)
{
    ptrdiff_t offset = DataFragment::findFragmentsNumOffset(command);
    uint8_t numOfFragments = command[offset];
    /*std::cout << 
        "event code: " << command.getEventCode() << " " <<
        "size:" << (unsigned)command.size() << " " <<
        "num of fragments: " << (unsigned)numOfFragments << " " << "\n";*/
    //command.printCommandInOneString(offset, command.size());
    offset += 1;

    uint8_t fragmentID = 0;

    uint8_t dataType = 0;
    uint8_t dataTypeSize = 0;
    uint8_t dataTypeHeaderSize = 0;
    uint16_t numOfEntries = 0;
    uint16_t sizeOfData = 0;
    uint16_t commandSize = command.size();

    DataFragment dataFragment;

    for (size_t i = 0; i < numOfFragments; i++) {
        if (offset > commandSize) {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << 
                "offset bigger than command size\n" << 
                "offset (hex dec): " << std::hex << offset << std::dec << " " << offset << "\n";
            command.printCommandInOneString();
            SetConsoleTextAttribute(consoleHandle, 7);
            return;
        }
        //std::cout << "start offset: " << offset << " ";
        if (offset < commandSize) {
            fragmentID = command[offset];
            //std::cout << "fragmentID: " << (unsigned)command[offset] << " ";
            offset += 1;
            dataType = command[offset];

            numOfEntries = DataType::getNumOfEntries(command, dataType, offset);
            /*if (dataType == dataType::int16) {
                std::cout << "numOfEntries: " << numOfEntries << " ";
            }*/
            
            if (dataType == dataType::listOfType) {
                offset += 3;
                dataTypeHeaderSize = 3;
                dataTypeSize = DataType::getDataTypeSize(dataType);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                );
                _dataLayout.push_back(dataFragment);

                dataType = command[offset];
                //processDictionary(command, fragmentID, offset, numOfEntries, dataTypeHeaderSize, 0);
                if (numOfEntries == 0) {
                    offset += 1;
                    dataTypeHeaderSize = 1;
                    sizeOfData = 0;

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        sizeOfData,
                        DataType(1, dataTypeHeaderSize, dataType)
                    );

                    _dataLayout.push_back(dataFragment);
                }
                else if (dataType == dataType::listOfType) {
                    uint16_t nestedNumOfEntries = DataType::getNumOfEntries(
                        command, dataType, offset);
                    offset += 3;
                    dataTypeHeaderSize = 3;

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        numOfEntries,
                        DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                    );
                    _dataLayout.push_back(dataFragment);

                    dataType = command[offset];
                    if (dataType == dataType::float32) {
                        dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                        dataTypeSize = DataType::getDataTypeSize(dataType);
                        offset += DataType::getDataTypeHeaderSize(dataType);
                        for (size_t i = 0; i < numOfEntries; i++) {
                            if (i > 0) {
                                offset += 3;
                            }

                            dataFragment = DataFragment(
                                fragmentID,
                                offset,
                                nestedNumOfEntries,
                                DataType(dataTypeSize,
                                    i == 0 ? dataTypeHeaderSize : 3, dataType::int8_string)
                            );

                            _dataLayout.push_back(dataFragment);

                            sizeOfData = dataTypeSize * nestedNumOfEntries;
                            offset += sizeOfData;
                        }
                    }
                }
                else if (dataType == dataType::int8_list || dataType == dataType::int8_string) {
                    uint16_t nestedNumOfEntries = DataType::getNumOfEntries(command, dataType, offset);
                    uint8_t nestedDataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                    dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);

                    offset += 1;
                    for (size_t i = 0; i < numOfEntries; i++) {
                        offset += nestedDataTypeHeaderSize - 1;
                        sizeOfData = DataType::getNumOfEntries(command, dataType, offset - nestedDataTypeHeaderSize);

                        dataFragment = DataFragment(
                            fragmentID,
                            offset,
                            sizeOfData,
                            DataType(1, i == 0 ? dataTypeHeaderSize : nestedDataTypeHeaderSize - 1, dataType)
                        );

                        _dataLayout.push_back(dataFragment);
                        offset += sizeOfData;

                        /*std::cout <<
                            "offset: "         << (unsigned)dataFragment._offset << " " <<
                            std::hex <<
                            "fragment id: "    << (unsigned)fragmentID           << " " <<
                            "data type: "      << (unsigned)dataType             << " " <<
                            std::dec <<
                            "size: "           << (unsigned)dataTypeSize         << " " <<
                            "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                            "num of entries: " << (unsigned)numOfEntries         << " " <<
                            "size of data: "   << (unsigned)sizeOfData           << "\n";*/
                    }
                }
                else {
                    dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                    dataTypeSize = DataType::getDataTypeSize(dataType);
                    offset += DataType::getDataTypeHeaderSize(dataType);

                    dataFragment = DataFragment(
                        fragmentID,
                        offset,
                        numOfEntries,
                        DataType(dataTypeSize, dataTypeHeaderSize, dataType::int8_string)
                    );

                    _dataLayout.push_back(dataFragment);

                    //std::cout << "size of data " << sizeOfData << "\n";
                    sizeOfData = dataTypeSize * numOfEntries;
                    offset += sizeOfData;
                }
            }
            else if (dataType == dataType::dictionary) {
                numOfEntries = DataType::getNumOfEntries(command, dataType, offset);

                offset += 1;
                              dataType     = command[offset];
                              dataTypeSize = DataType::getDataTypeSize(dataType);
                uint8_t nestedDataType     = command[offset + 1];
                uint8_t nestedDataTypeSize = DataType::getDataTypeSize(nestedDataType);

                sizeOfData = 4 + numOfEntries * (dataTypeSize + nestedDataTypeSize);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize + nestedDataTypeSize, 4, dataType::dictionary)
                );

                _dataLayout.push_back(dataFragment);
                /*std::cout <<
                    "offset: "         << (unsigned)dataFragment._offset << " " <<
                    std::hex <<
                    "fragment id: "    << (unsigned)fragmentID           << " " <<
                    "data type: "      << (unsigned)dataType             << " " <<
                    std::dec <<
                    "size: "           << (unsigned)dataTypeSize         << " " <<
                    "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                    "num of entries: " << (unsigned)numOfEntries         << " " <<
                    "size of data: "   << (unsigned)sizeOfData           << "\n";*/

                offset += sizeOfData;
            }
            else {
                dataTypeSize = DataType::getDataTypeSize(dataType);
                dataTypeHeaderSize = DataType::getDataTypeHeaderSize(dataType);
                offset += DataType::getDataTypeHeaderSize(dataType);

                dataFragment = DataFragment(
                    fragmentID,
                    offset,
                    numOfEntries,
                    DataType(dataTypeSize, dataTypeHeaderSize, dataType)
                );

                sizeOfData = dataTypeSize * numOfEntries;
                offset += sizeOfData;
                _dataLayout.push_back(dataFragment);

                /*std::cout <<
                    "offset: "         << (unsigned)dataFragment._offset << " " <<
                    std::hex <<
                    "fragment id: "    << (unsigned)fragmentID           << " " <<
                    "data type: "      << (unsigned)dataType             << " " <<
                    std::dec <<
                    "size: "           << (unsigned)dataTypeSize         << " " <<
                    "header size: "    << (unsigned)dataTypeHeaderSize   << " " <<
                    "num of entries: " << (unsigned)numOfEntries         << " " <<
                    "size of data: "   << (unsigned)sizeOfData           << "\n";*/
            }
        }
    }
}
DataLayout::DataLayout(std::vector<DataFragment> dataFragments)
{
    _dataLayout = dataFragments;
}
DataLayout::DataLayout()
{
    _dataLayout = {};
}

void DataLayout::printInfo(NetworkCommand& command, bool printPayload) const
{
    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout << 
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    for (size_t i = 0; i < _dataLayout.size(); i++) {
        _dataLayout[i].printFragmentInfo(command, currentStringPosition, printPayload);

        /*if (i < _dataLayout.size() - 1) {
            command.printCommandInOneString(_dataLayout[i]._offset, _dataLayout[i + 1]._offset, false);
        }
        else {
            command.printCommandInOneString(_dataLayout[i]._offset, command.size(), false);
        }
        std::cout << " ";*/
    }
    std::cout << "\n\n";
}
void DataLayout::printInfo(NetworkCommand& command, size_t beginFragment, bool printPayload) const
{
    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout <<
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    for (const DataFragment& dataFragment : _dataLayout) {
        if (dataFragment._fragmentID >= beginFragment) {
            dataFragment.printFragmentInfo(command, currentStringPosition, printPayload);
        }
    }
    std::cout << "\n\n";
}
void DataLayout::printInfo(NetworkCommand& command, size_t beginFragment, size_t endFragment, bool printPayload) const
{
    uint16_t numOfFragmentsOffset = _dataLayout[0]._offset - _dataLayout[0]._dataType._headerSize - 2;
    std::cout <<
        "num of fragments: " << (unsigned)command[numOfFragmentsOffset] << " " <<
        "event code: " << command.getEventCode() << " " <<
        "size: " << command.size() << "\n";

    size_t currentStringPosition = 0;
    for (const DataFragment& dataFragment : _dataLayout) {
        //std::vector<uint8_t> nonWantedElements = { 13, 14, 17, 19 };
        if (dataFragment._fragmentID >= beginFragment && dataFragment._fragmentID <= endFragment/* &&
            !std::isElementInVector(nonWantedElements, dataFragment._fragmentID)*/) {
            dataFragment.printFragmentInfo(command, currentStringPosition, printPayload);
        }
    }
    std::cout << "\n\n";
}

size_t DataLayout::size() const
{
    return _dataLayout.size();
}
DataFragment DataLayout::operator[](size_t elementIndex)
{
    return _dataLayout[elementIndex];
}

nlohmann::json net::readJsonFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: Could not open file: " + filename);
    }
    nlohmann::json jsonData;
    file >> jsonData;
    return jsonData;
}

void net::extractMatchingMobs(
    const nlohmann::json& data, 
    float_t moveSpeed, float_t maxEnergy, float_t energyRegeneration
)
{
    if (!data.contains("Mobs") || !data["Mobs"].contains("Mob")) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    for (const auto& mob : data["Mobs"]["Mob"]) {
        if (mob.contains("@movespeed") &&
            mob.contains("@energymax") &&
            mob.contains("@energyregeneration")
            ) {
            float_t mobMoveSpeed = std::stof(mob["@movespeed"].get<std::string>());
            float_t mobMaxEnergy = std::stof(mob["@energymax"].get<std::string>());
            float_t mobEnergyRegeneration = std::stof(mob["@energyregeneration"].get<std::string>());

            if (mobMoveSpeed == moveSpeed && 
                mobMaxEnergy == maxEnergy &&
                mobEnergyRegeneration == energyRegeneration
                ) {
                std::cout << 
                    "uniquename: " << mob["@uniquename"] << " " <<
                    "category: "   << mob["@category"] << " " <<
                    "faction: "    << mob["@faction"] << "\n";
            }
        }
    }
    std::cout << "\n";
}

void net::findUniqueValues(
    const nlohmann::json& data, 
    const std::string& firstLevel, 
    const std::string& secondLevel,
    const std::string& key
)
{
    if (!data.contains(firstLevel) || !data[firstLevel].contains(secondLevel)) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    std::unordered_map<std::string, size_t> uniqueValues;
    std::vector<std::string> order;
    size_t maxKeyLength = 0;
    size_t totalCount = 0;

    for (const auto& entry : data[firstLevel][secondLevel]) {
        if (entry.contains(key) && entry[key].is_string()) {
            std::string entryName = entry[key].get<std::string>();

            if (uniqueValues.find(entryName) == uniqueValues.end()) {
                order.push_back(entryName); // Store order of first appearance
            }

            uniqueValues[entryName]++;
            totalCount++;
            maxKeyLength = std::max(maxKeyLength, entryName.length());
        }
    }

    // Print results
    std::cout << 
        "Unique values for \"" << key << "\" (" << 
        uniqueValues.size() << " found) (" << 
        totalCount << " " << firstLevel << ")" << ":\n";

    size_t index = 0;
    for (const auto& value : order) {
        std::cout << 
            "- " << std::setw(maxKeyLength) << std::left << value <<
            " = " << index << 
            " (" << uniqueValues[value] << " " << firstLevel << ")\n";
        index++;
    }
    /*for (const auto& entryName : order) {
        std::cout <<
            std::setw(maxKeyLength) << std::left << entryName <<
            " = " << index << ",\n";
        index++;
    }*/
    std::cout << "---------------------------------\n";
}

void net::findUniqueValuesByCriterion(
    const nlohmann::json& data,
    const std::string& firstLevel,
    const std::string& secondLevel,
    const std::string& uniqueKey,
    const std::string& criterion
)
{
    if (!data.contains(firstLevel) || !data[firstLevel].contains(secondLevel)) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    std::unordered_map<std::string, std::vector<std::string>> uniqueValues;
    std::vector<std::string> order;
    size_t maxCriterionLength = 0;
    size_t maxUniqueLength = 0;

    for (const auto& entry : data[firstLevel][secondLevel]) {
        if (entry.contains(criterion) && entry[criterion].is_string() &&
            entry.contains(uniqueKey) && entry[uniqueKey].is_string()) {

            std::string criterionName = entry[criterion].get<std::string>();
            std::string uniqueName = entry[uniqueKey].get<std::string>();

            if (uniqueValues.find(criterionName) == uniqueValues.end()) {
                order.push_back(criterionName); // Store order of first appearance
            }

            uniqueValues[criterionName].push_back(uniqueName);
            maxCriterionLength = std::max(maxCriterionLength, criterionName.length());
            maxUniqueLength = std::max(maxUniqueLength, uniqueName.length());
        }
    }

    // Print results
    std::cout << "Mob Unique Names by Faction:\n";
    for (const std::string& criterionName : order) {
        const std::vector<std::string>& uniqueNames = uniqueValues[criterionName];

        std::cout << "- " << criterionName << ":\n";
        size_t index = 0;
        for (const auto& uniqueName : uniqueNames) {
            std::cout << "    " << std::setw(maxUniqueLength) << std::left << uniqueName
                << " = " << index << ",\n";
            index++;
        }
        std::cout << "---------------------------------\n";
    }
}

