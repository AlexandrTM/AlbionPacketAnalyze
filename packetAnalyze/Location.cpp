#include "pch.h"

Location::Location()
{
    _locationID      = "";
    _harvestableList = {};
    _playerList      = {};
    _mobList         = {};
    _fishNodeList    = {};
}

Location::Location(
    std::string locationID,
    HarvestableList harvestableList, 
    PlayerList playerList, 
    MobList mobList, 
    FishNodeList fishNodeList
)
{
    _locationID      = locationID;
    _harvestableList = harvestableList;
    _playerList      = playerList;
    _mobList         = mobList;
    _fishNodeList    = fishNodeList;
}

void Location::findLocationData(
    const std::regex& locationRegex, Location& location
)
{
    std::smatch match;
    if (std::regex_search(location._locationID, match, locationRegex)) {
        std::string locationCode = match.str();
        std::string xmlFileName = locationCode + "_.*.cluster.xml";

        // Find file with matching prefix
        for (const auto& entry : std::filesystem::directory_iterator("cluster")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.rfind(locationCode, 0) == 0 && filename.ends_with(".xml")) {
                    parseLocationXML(entry.path().string(), location);
                    break;
                }
            }
        }
    }
    else {
        location._size = { 930, 930 };
        location._halfSize = 465;
    }
}
void Location::parseLocationXML(const std::string& filePath, Location& location) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << filePath << "\n";
        return;
    }

    tinyxml2::XMLElement* cluster = doc.FirstChildElement("cluster");
    if (!cluster) {
        std::cerr << "No <cluster> tag found in: " << filePath << "\n";
        return;
    }

    const char* origin = cluster->Attribute("origin");
    const char* size = cluster->Attribute("size");

    if (origin) {
        int x = 0, y = 0;
        if (sscanf_s(origin, "%d %d", &x, &y) == 2) {
            location._origin = { x, y };
            //std::cout << "Origin for " << filePath << " = (" << x << ", " << y << ")\n";
        }
        else {
            std::cerr << "Malformed origin attribute in: " << filePath << " -> " << origin << "\n";
        }
    }
    else {
        std::cerr << "No 'origin' attribute in: " << filePath << "\n";
    }

    if (size) {
        int width = 0, height = 0;
        if (sscanf_s(size, "%d %d", &width, &height) == 2) {
            location._size = { width, height };
            location._halfSize = std::max(width, height) / 2;
            //location._halfSize = width / 2;
            std::cout << "Size for " << filePath << " = (" << width << ", " << height << ")\n";
            std::cout << "half size: " << location._halfSize << "\n";
        }
        else {
            std::cerr << "Malformed size attribute in: " << filePath << " -> " << size << "\n";
        }
    }
    else {
        std::cerr << "No 'size' attribute in: " << filePath << "\n";
    }
}

void Location::printInfo(
    std::vector<Location>& locations, Location& currentLocation,
    std::string locationFrom, std::string locationTo
)
{
    auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timeBuffer[30];
    ctime_s(timeBuffer, sizeof(timeBuffer), &timeNow);

    std::cout << "num of locations: " << locations.size() << "\n";
    /*for (size_t i = 0; i < locations.size(); i++) {
        std::cout <<
            "location id" << i << ": " << locations[i]._locationID          << "\n" <<
            "num of harvestables: "    << locations[i]._harvestableList.size() << "\n" <<
            "num of mobs:         "    << locations[i]._mobs.size()         << "\n\n";
    }*/

    std::cout <<
        "from: " << locationFrom << " -> " <<
        "to: " << locationTo << "\n" <<
        timeBuffer;

    std::cout <<
        "current location:    " << locationTo << "\n" <<
        "num of harvestables: " << currentLocation._harvestableList.size() << "\n" <<
        "num of mobs:         " << currentLocation._mobList.size() << "\n\n";

    // Print information about the location you're coming from (locationFrom)
    std::cout << "From location: " << locationFrom << "\n";
    for (Location& location: locations) {
        if (location._locationID == locationFrom) {
            std::cout << "Number of harvestables: " << location._harvestableList.size() << "\n";
            std::cout << "Number of mobs: " << location._mobList.size() << "\n";
            location._harvestableList.printInfo(); // Print detailed info of the harvestables
            break;
        }
    }
}
