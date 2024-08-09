#include "pch.h"

Location::Location()
{
    std::string _locationID = "";
    HarvestableList _harvestableList = {};
    PlayerList _playerList = {};
    MobList _mobList = {};
}

Location::Location(std::string locationID,
    HarvestableList harvestableList, PlayerList playerList, MobList mobList)
{
    _locationID = locationID;
    _harvestableList = harvestableList;
    _playerList = playerList;
    _mobList = mobList;
}

void Location::changeLocation(
    NetworkCommand& command,
    std::vector<Location>& locations,
    HarvestableList& currentHarvestableList, 
    PlayerList& currentPlayerList, 
    MobList& currentMobList,
    bool printInfo)
{
    DataLayout dataLayout{};
    dataLayout.findDataLayout(command);
    //_dataLayout.printInfo(*this);
    DataFragment locationFromFragment = dataLayout.findFragment(65);
    DataFragment locationToFragment = dataLayout.findFragment(8);
    std::string locationFrom = "";
    std::string locationTo = "";
    for (size_t i = 0; i < locationFromFragment._numOfEntries; i++) {
        locationFrom += (unsigned)command[locationFromFragment._offset + i];
    }
    for (size_t i = 0; i < locationToFragment._numOfEntries; i++) {
        locationTo += (unsigned)command[locationToFragment._offset + i];
    }

    std::sort(currentHarvestableList.begin(), currentHarvestableList.end(),
        [](const Harvestable& a, const Harvestable& b) {
            return a._id < b._id;
        });

    //currentHarvestableList.printInfo();
    if (locations.size() == 0) {
        locations.push_back(Location(locationFrom,
            currentHarvestableList, currentPlayerList, currentMobList));

        locations.push_back(Location(locationTo, {}, {}, {}));
        currentHarvestableList = {};
        currentPlayerList      = {};
        currentMobList         = {};
    }
    else {
        for (size_t i = 0; i < locations.size(); i++) {
            //locations[i].printInfo();
            if (locations[i]._locationID == locationTo) {
                currentHarvestableList = locations[i]._harvestableList;
                currentPlayerList      = {};
                currentMobList         = locations[i]._mobList;
                //currentPlayerList    = locations[i]._playerList;
                break;
            }
            if (locations[i]._locationID == locationFrom) {
                locations[i]._harvestableList = currentHarvestableList;
                locations[i]._playerList      = {};
                locations[i]._mobList         = currentMobList;
                //locations[i]._playerList      = currentPlayerList;
            }
            if (i == locations.size() - 1) {
                locations.push_back(Location(locationTo, {}, {}, {}));
                currentHarvestableList = {};
                currentPlayerList      = {};
                currentMobList         = {};
                break;
            }
        }
        /*for (size_t i = 0; i < locations.size(); i++) {
            if (locations[i]._locationID == locationFrom) {
            }
            else if (i == locations.size() - 1) {
                locations.push_back(Location(locationFrom,
                    currentHarvestableList, currentPlayerList, currentMobList));
            }
        }*/
    }

    if (printInfo) {
        auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeBuffer[30];
        ctime_s(timeBuffer, sizeof(timeBuffer), &timeNow);
        std::cout <<
            "from: " << locationFrom << " -> " <<
            "to: " << locationTo << "\n" <<
            timeBuffer;

        std::cout << "num of locations: " << locations.size() << "\n";
        for (size_t i = 0; i < locations.size(); i++) {
            std::cout << 
                "location id" << i << ": " << locations[i]._locationID          << "\n" << 
                "num of harvestables: " << locations[i]._harvestableList.size() << "\n" << 
                "num of mobs:         " << locations[i]._mobList.size()         << "\n\n";
        } 

        std::cout << 
            "current location:    " << locationTo                    << "\n" <<
            "num of harvestables: " << currentHarvestableList.size() << "\n" << 
            "num of mobs:         " << currentMobList.size()         << "\n\n";
    }
}

void Location::printInfo()
{
    _harvestableList.printInfo();
}
