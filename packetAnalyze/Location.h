#ifndef LOCATION_H
#define LOCATION_H

struct Location
{
    std::string     _locationID      = "";
    HarvestableList _harvestableList = {};
    PlayerList      _playerList      = {};
    MobList         _mobList         = {};
    FishNodeList    _fishNodeList    = {};

    Location();
    Location(
        std::string locationID,
        HarvestableList harvestableList,
        PlayerList playerList,
        MobList mobList,
        FishNodeList fishNodeList
    );
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