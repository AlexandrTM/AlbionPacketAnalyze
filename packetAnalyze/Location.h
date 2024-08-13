#ifndef LOCATION_H
#define LOCATION_H

struct Location
{
    std::string     _locationID      = "";
    HarvestableList _harvestableList = {};
    PlayerList      _playerList      = {};
    MobList         _mobList         = {};

    Location();
    Location(std::string locationID,
        HarvestableList harvestableList, PlayerList playerList, MobList mobList);
    static void changeLocation(
        NetworkCommand& command,
        std::vector<Location>& locations,
        Location& currentLocation,
        bool printInfo
    );

    void printInfo();
};

#endif