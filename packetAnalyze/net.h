#pragma once

namespace net
{
    static inline uint8_t read_uint8(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            return command[offset];
        }
        return 0;
    }
    static inline uint16_t read_uint16(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint16_t dataEntry = 0;
            for (size_t i = 0; i < 2; i++) {
                dataEntry += (command[offset + i] << (8 * (1 - i)));
            }
            return dataEntry;
        }
        return 0;
    }
    static inline uint32_t read_uint32(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint32_t dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                dataEntry += (command[offset + i] << (8 * (3 - i)));
            }
            return dataEntry;
        }
        return 0;
    }
    static inline uint32_t read_uint32(const std::vector<uint8_t> rawCommand, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint32_t dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                dataEntry += (rawCommand[offset + i] << (8 * (3 - i)));
            }
            return dataEntry;
        }
        return 0;
    }
    static inline uint64_t read_uint64(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint64_t dataEntry = 0;
            for (size_t i = 0; i < 8; i++) {
                dataEntry += (static_cast<unsigned __int64>(command[offset + i]) << (8 * (7 - i)));
            }
            return dataEntry;
        }
        return 0;
    }
    static inline float_t read_float32(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint32_t dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                dataEntry += (command[offset + i] << (8 * (3 - i)));
            }
            return std::binToFloat(dataEntry);
        }
        return 0;
    }
    static inline float_t read_float32big(const NetworkCommand& command, ptrdiff_t offset)
    {
        if (offset != std::numeric_limits<ptrdiff_t>::min()) {
            uint32_t dataEntry = 0;
            for (size_t i = 0; i < 4; i++) {
                dataEntry += (command[offset + 3 - i] << (8 * (3 - i)));
            }
            return std::binToFloat(dataEntry);
        }
        return 0;
    }
    static inline float_t read_float32big_from_bits(const NetworkCommand& command, ptrdiff_t bitOffset) {
        // Calculate byte offset and bit shift within the first byte
        ptrdiff_t byteOffset = bitOffset / 8;
        uint8_t bitShift = bitOffset % 8;

        if (byteOffset + 4 > command.size()) {
            return NAN; // Return NaN if out of bounds
        }

        // Extract 32 bits (4 bytes) across multiple bytes, considering bitShift
        uint32_t dataEntry = 0;
        for (size_t i = 0; i < 4; i++) {
            dataEntry += (command[byteOffset + 3 - i] << (8 * (3 - i)));
        }

        // Shift to align bits correctly if the float starts in the middle of a byte
        dataEntry = dataEntry >> bitShift;

        // Mask the bits to ensure only the first 32 bits are considered
        dataEntry &= 0xFFFFFFFF;  // Ensure we only use the lowest 32 bits

        // Convert the 32-bit integer to a float using your existing converter
        return std::binToFloat(dataEntry);
    }
    static inline float_t read_float32_from_bits(const NetworkCommand& command, ptrdiff_t bitOffset) {
        ptrdiff_t byteOffset = bitOffset / 8;
        uint8_t bitShift = bitOffset % 8;

        if (byteOffset + 4 > command.size()) {
            return NAN;
        }

        uint32_t dataEntry = 0;
        for (size_t i = 0; i < 4; i++) {
            dataEntry += (command[byteOffset + i] << (8 * (3 - i)));
        }

        dataEntry = dataEntry >> bitShift;
        dataEntry &= 0xFFFFFFFF;

        return std::binToFloat(dataEntry);
    }
    static inline std::string readString(const NetworkCommand& command, ptrdiff_t offset, size_t length) {
        return std::string(command.begin() + offset, command.begin() + offset + length);
    }
    static inline std::string readString(const NetworkCommand& command, DataFragment& dataFragment) {
        return readString(command, dataFragment._offset, dataFragment._numOfEntries);
    }
    
    static uint64_t read_integer(NetworkCommand& command, DataFragment& dataFragment) {
        uint64_t integer = 0;
        uint8_t dataTypeSize = dataFragment._dataType._size;
        ptrdiff_t fragmentOffset = dataFragment._offset;

        if
            (dataTypeSize == 1) {
            integer = net::read_uint8(command, fragmentOffset);
        }
        else if
            (dataTypeSize == 2) {
            integer = net::read_uint16(command, fragmentOffset);
        }
        else if
            (dataTypeSize == 4) {
            integer = net::read_uint32(command, fragmentOffset);
        }
        else if
            (dataTypeSize == 8) {
            integer = net::read_uint64(command, fragmentOffset);
        }

        return integer;
    }

    nlohmann::json readJsonFile(const std::string& filename);
    
    void getMobData(
        const nlohmann::json& data, const uint16_t uniqueValue,
        uint8_t& tier, std::string& uniqueName,
        std::string& category, std::string& typeCategory
    );
    void getMatchingMobs(
        const nlohmann::json& data,
        float_t moveSpeed, float_t maxEnergy, float_t energyRegeneration
    );
    void findUniqueValues(
        const nlohmann::json& data,
        const std::string& firstLevel,
        const std::string& secondLevel,
        const std::string& key
    );
    void findUniqueValuesByCriterion(
        const nlohmann::json& data,
        const std::string& firstLevel,
        const std::string& secondLevel,
        const std::string& uniqueKey,
        const std::string& criterion
    );
    void parseObjectsFromTemplate(const std::string& filePath);

    void removeOutdatedTemporaryConnections(const std::string filePath);
    void makeLocationsConnection(
        Location& locationFrom, Location& locationTo, const bool& isChangingLocation
    );
    void addLocationIfMissing(const Location& location);
    void updatePlayerData(Location& currentLocation, const std::string& filePath);
    void parseLocationsAndConnections(const std::string& xmlPath);
    std::string getLocationNameById(const nlohmann::json& locationNames, const std::string& id);
    void searchLocationsTemplates(int32_t x, int32_t y);
    void findLocationsStatistics(const std::string& filePath);

    void formatItemsData();
    std::string get_utc_time();
    std::chrono::system_clock::time_point parse_utc_time_string(const std::string& utcString);

    const nlohmann::json locationNames = net::readJsonFile("world.json");
    const nlohmann::json mobsData = net::readJsonFile("mobs.json");
    const nlohmann::json harvestablesData = net::readJsonFile("harvestables.json");
};

