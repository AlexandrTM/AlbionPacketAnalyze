#include "pch.h"

Location::Location()
{
    _id      = "";
    _harvestableList = {};
    _playerList      = {};
    _mobList         = {};
    _fishNodeList    = {};
}

Location::Location(
    std::string locationID,
    std::string locationName,
    HarvestableList harvestableList, 
    PlayerList playerList, 
    MobList mobList, 
    FishNodeList fishNodeList
)
{
    _id              = locationID;
    _name            = locationName;
    _harvestableList = harvestableList;
    _playerList      = playerList;
    _mobList         = mobList;
    _fishNodeList    = fishNodeList;
}

const std::regex tierRegex("_T(\\d+)_");
const std::regex biomeRegex("_(SW|FR|HL|ST|MN)_");
// starts with, for regular map four numbers + _WRL
//const std::regex typeRegex("(TNL|_WRL|DNG|PSG|_CTY|BLACKBANK)");

void Location::findLocationData(Location& location)
{
    location._name = net::getLocationNameById(net::locationNames, location._id);
    if (location._name == "") return;

    std::string locationId = location._id;
    bool isAvalonRoad = locationId.starts_with("TNL-");
    if (isAvalonRoad) {
        adjustAvalonNumber(locationId);
    }

    // Find file with matching prefix
    for (const auto& entry : std::filesystem::directory_iterator("cluster")) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.starts_with(locationId) && filename.ends_with(".xml")) {
                // Extract tier from filename
                std::smatch tierMatch;
                std::smatch biomeMatch;

                if (std::regex_search(filename, tierMatch, tierRegex)) {
                    location._tier = std::stoi(tierMatch[1].str());
                }
                else {
                    location._tier = 0; // fallback if parsing fails
                }
                if (std::regex_search(filename, biomeMatch, biomeRegex)) {
                    std::string biomeCode = biomeMatch[1].str();
                    auto it = biomeMap.find(biomeCode);
                    location._biome = (it != biomeMap.end()) ? it->second : "other";
                }
                else {
                    location._biome = "other";
                }
                location._type = findLocationType(filename);

                if (isAvalonRoad) {
                    parseLocationXML(entry.path().string(), location);
                    return;
                }
            }
        }
    }

    location._size = { 930, 930 };
    location._halfSize = 465;
}

std::string Location::findLocationType(const std::string& fileName)
{
    std::string locationType = {};

    if (std::regex_search(fileName, std::regex(R"(^\d{4}_WRL)"))) {
        locationType = "world";
    }
    else if (std::regex_search(fileName, std::regex(R"(^\d{4}_CTY)"))) {
        locationType = "city";
    }
    else if (std::regex_search(fileName, std::regex(R"(^\d{4}_HBS)"))) {
        locationType = "world_boss";
    }
    else if (fileName.starts_with("BLACKBANK")) {
        locationType = "smuggler";
    }
    else if (fileName.starts_with("DNG")) {
        locationType = "static_dungeon";
    }
    else if (fileName.starts_with("PSG")) {
        locationType = "passage";
    }
    else if (fileName.starts_with("TNL")) {
        locationType = "avalon";
    }
    else {
        locationType = "other";
    }

    return locationType;
}
void Location::adjustAvalonNumber(std::string& locationId)
{
    std::string numberPart = locationId.substr(4); // after "TNL-"
    try {
        int32_t number = std::stoi(numberPart);
        if (number > 200) {
            number -= 200;

            std::stringstream ss;
            ss << "TNL-" << std::setw(3) << std::setfill('0') << number;
            locationId = ss.str(); // e.g. "TNL-013"
        }
    }
    catch (...) {
        // Ignore malformed number, fallback to original locationId
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

    //std::cout << "location file name: " << filePath << "\n";
    if (origin) {
        int x = 0, y = 0;
        if (sscanf_s(origin, "%d %d", &x, &y) == 2) {
            location._origin = { x, y };
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
        }
        else {
            std::cerr << "Malformed size attribute in: " << filePath << " -> " << size << "\n";
        }
    }
    else {
        std::cerr << "No 'size' attribute in: " << filePath << "\n";
    }
}
