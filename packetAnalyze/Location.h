#ifndef LOCATION_H
#define LOCATION_H

struct Location
{
    std::string     _id              = "";
    std::string     _name            = "";
    std::string     _biome           = "";
    std::string     _type            = "";
    int32_t         _tier            = 0;

    glm::ivec2      _fromCenterOffset    = glm::ivec2(0, 0);
    glm::ivec2      _size            = glm::ivec2(830, 830);
    size_t          _halfSize        = 415;

    HarvestableList _harvestableList = {};
    PlayerList      _playerList = {};
    MobList         _mobList = {};
    FishNodeList    _fishNodeList = {};

    Location();
    Location(
        std::string locationId,
        std::string locationName,
        HarvestableList harvestableList,
        PlayerList playerList,
        MobList mobList,
        FishNodeList fishNodeList
    );
    static void findLocationData(Location& location);
    static std::string findLocationType(const std::string& fileName);
    static void parseLocationXML(const std::string& filePath, Location& location);
    static void adjustAvalonNumber(std::string& locationId);
};

#endif