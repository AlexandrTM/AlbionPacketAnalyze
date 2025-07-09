#ifndef LOCATION_H
#define LOCATION_H

struct Location
{
    std::string     _id              = "";
    std::string     _name            = "";

    HarvestableList _harvestableList = {};
    PlayerList      _playerList      = {};
    MobList         _mobList         = {};
    FishNodeList    _fishNodeList    = {};

    glm::ivec2      _origin          = glm::ivec2(0, 0);
    glm::ivec2      _size            = glm::ivec2(930, 930);
    size_t          _halfSize        = 465;

    Location();
    Location(
        std::string locationId,
        std::string locationName,
        HarvestableList harvestableList,
        PlayerList playerList,
        MobList mobList,
        FishNodeList fishNodeList
    );
    static void findLocationData(
        const std::regex& locationRegex, Location& location
    );
    static void parseLocationXML(const std::string& filePath, Location& location);

    static void printInfo(
        std::vector<Location>& locations,
        Location& currentLocation, Location& previousLocation
    );
};

#endif