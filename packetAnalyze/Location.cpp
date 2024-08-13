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
    Location& currentLocation,
    bool printInfo)
{
    DataLayout dataLayout{};
    dataLayout.findDataLayout(command);
    //dataLayout.printInfo(command);
    //command.printCommandInOneString();
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

    /*std::sort(currentHarvestableList.begin(), currentHarvestableList.end(),
        [](const Harvestable& a, const Harvestable& b) {
            return a._id < b._id;
        });*/

    //currentHarvestableList.printInfo();
    bool locationToIsNew = true;
    bool locationFromIsNew = true;
    // order is important
    for (size_t i = 0; i < locations.size(); i++) {
        if (locations[i]._locationID == locationFrom) {
            locations[i]._harvestableList = currentLocation._harvestableList;
            locations[i]._playerList = PlayerList();
            locations[i]._mobList = currentLocation._mobList;
            //locations[i]._playerList      = currentPlayerList;
            locationFromIsNew = false;
            break;
        }
    }
    for (size_t i = 0; i < locations.size(); i++) {
        //locations[i].printInfo();
        if (locations[i]._locationID == locationTo) {
            currentLocation._harvestableList = locations[i]._harvestableList;
            currentLocation._playerList      = PlayerList();
            currentLocation._mobList         = locations[i]._mobList;
            //currentPlayerList    = locations[i]._playerList;
            locationToIsNew = false;
            break;
        }
    }
    if (locationFromIsNew == true) {
        locations.push_back(Location(locationFrom,
            currentLocation._harvestableList, currentLocation._playerList, currentLocation._mobList));
    }
    if (locationToIsNew == true) {
        locations.push_back(Location(locationTo, {}, {}, {}));
        currentLocation._harvestableList = {};
        currentLocation._playerList      = {};
        currentLocation._mobList         = {};
    }

    if (printInfo) {
        auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeBuffer[30];
        ctime_s(timeBuffer, sizeof(timeBuffer), &timeNow);

        std::cout << "num of locations: " << locations.size() << "\n";
        for (size_t i = 0; i < locations.size(); i++) {
            std::cout << 
                "location id" << i << ": " << locations[i]._locationID          << "\n" << 
                "num of harvestables: "    << locations[i]._harvestableList.size() << "\n" << 
                "num of mobs:         "    << locations[i]._mobList.size()         << "\n\n";
        }

        std::cout <<
            "from: " << locationFrom << " -> " <<
            "to: " << locationTo << "\n" <<
            timeBuffer;

        std::cout << 
            "current location:    " << locationTo                    << "\n" <<
            "num of harvestables: " << currentLocation._harvestableList.size() << "\n" <<
            "num of mobs:         " << currentLocation._mobList.size()         << "\n\n";
    }
}

void Location::printInfo()
{
    _harvestableList.printInfo();
}
