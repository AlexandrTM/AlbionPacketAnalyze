#ifndef LOCATION_H
#define LOCATION_H

struct Location
{
    std::string     _locationID      = "";
    HarvestableList _harvestableList = {};
    PlayerList      _playerList      = {};
    MobList         _mobList         = {};
    FishNodeList    _fishNodeList    = {};
    glm::ivec2      _origin          = glm::ivec2(0, 0);
    glm::ivec2      _size            = glm::ivec2(800, 800);

    Location();
    Location(
        std::string locationID,
        HarvestableList harvestableList,
        PlayerList playerList,
        MobList mobList,
        FishNodeList fishNodeList
    );
    static void findLocationData(
        const std::regex& locationRegex, Location& location
    );
    static void parseLocationXML(const std::string& filePath, Location& location);

    static void changeLocation(
        NetworkCommand& command,
        std::vector<Location>& locations,
        Location& currentLocation,
        bool printInfo
    );

    static void printInfo(
        std::vector<Location>& locations, Location& currentLocation,
        std::string locationFrom, std::string locationTo
    );
};

#endif