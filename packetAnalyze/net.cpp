#include "pch.h"
#include "net.h"
#include "entity_list_instance.h"

nlohmann::json net::readJsonFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error: Could not open file: " + filename);
    }
    nlohmann::json jsonData;
    file >> jsonData;
    return jsonData;
}

void net::getMobData(
    const nlohmann::json& data, const uint16_t uniqueValue,
    uint8_t& tier, std::string& uniqueName,
    std::string& category, std::string& typeCategory
)
{
    if (!data.contains("Mobs") || !data["Mobs"].contains("Mob")) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    const auto& mobs = data["Mobs"]["Mob"];

    if (uniqueValue >= mobs.size()) {
        // std::cerr << "Error: mobUniqueValue out of range! " << uniqueValue << std::endl;
        return;
    }

    const auto& mob = mobs[uniqueValue];

    if (mob.contains("@category") && mob["@category"].is_string()) {
        category = mob["@category"].get<std::string>();
    }
    if (mob.contains("@mobtypecategory") && mob["@mobtypecategory"].is_string()) {
        typeCategory = mob["@mobtypecategory"].get<std::string>();
        //std::cout << "typeCategory: " << typeCategory << "\n";
    }
    uniqueName = mob["@uniquename"].get<std::string>();
    tier = std::stoi(mob["@tier"].get<std::string>());
}

void net::getMatchingMobs(
    const nlohmann::json& data,
    float_t moveSpeed, float_t maxEnergy, float_t energyRegeneration
)
{
    if (!data.contains("Mobs") || !data["Mobs"].contains("Mob")) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    for (const auto& mob : data["Mobs"]["Mob"]) {
        if (mob.contains("@movespeed") &&
            mob.contains("@energymax") &&
            mob.contains("@energyregeneration")
            ) {
            float_t mobMoveSpeed = std::stof(mob["@movespeed"].get<std::string>());
            float_t mobMaxEnergy = std::stof(mob["@energymax"].get<std::string>());
            float_t mobEnergyRegeneration = std::stof(mob["@energyregeneration"].get<std::string>());

            if (mobMoveSpeed == moveSpeed &&
                mobMaxEnergy == maxEnergy &&
                mobEnergyRegeneration == energyRegeneration
                ) {
                std::cout <<
                    "uniquename: " << mob["@uniquename"] << " " <<
                    "category: " << mob["@category"] << " " <<
                    "category: " << mob["@category"] << "\n";
            }
        }
    }
    std::cout << "\n";
}

void net::findUniqueValues(
    const nlohmann::json& data,
    const std::string& firstLevel,
    const std::string& secondLevel,
    const std::string& key
)
{
    if (!data.contains(firstLevel) || !data[firstLevel].contains(secondLevel)) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    std::unordered_map<std::string, size_t> uniqueValues;
    std::vector<std::string> order;
    size_t maxKeyLength = 0;
    size_t totalCount = 0;

    for (const auto& entry : data[firstLevel][secondLevel]) {
        if (entry.contains(key) && entry[key].is_string()) {
            std::string entryName = entry[key].get<std::string>();

            if (uniqueValues.find(entryName) == uniqueValues.end()) {
                order.push_back(entryName); // Store order of first appearance
            }

            uniqueValues[entryName]++;
            totalCount++;
            maxKeyLength = std::max(maxKeyLength, entryName.length());
        }
    }

    // Print results
    std::cout <<
        "Unique values for \"" << key << "\" (" <<
        uniqueValues.size() << " found) (" <<
        totalCount << " " << firstLevel << ")" << ":\n";

    size_t index = 0;
    for (const auto& value : order) {
        std::cout <<
            "- " << std::setw(maxKeyLength) << std::left << value <<
            " = " << index <<
            " (" << uniqueValues[value] << " " << firstLevel << ")\n";
        index++;
    }
    /*for (const auto& entryName : order) {
        std::cout <<
            std::setw(maxKeyLength) << std::left << entryName <<
            " = " << index << ",\n";
        index++;
    }*/
    std::cout << "---------------------------------\n";
}

void net::findUniqueValuesByCriterion(
    const nlohmann::json& data,
    const std::string& firstLevel,
    const std::string& secondLevel,
    const std::string& uniqueKey,
    const std::string& criterion
)
{
    if (!data.contains(firstLevel) || !data[firstLevel].contains(secondLevel)) {
        std::cerr << "Error: Invalid JSON format!" << std::endl;
        return;
    }

    std::unordered_map<std::string, std::vector<std::string>> uniqueValues;
    std::vector<std::string> order;
    std::unordered_map<std::string, size_t> mobIndex;
    size_t maxCriterionLength = 0;
    size_t maxUniqueLength = 0;
    size_t globalIndex = 0;

    for (const auto& entry : data[firstLevel][secondLevel]) {
        if (entry.contains(criterion) && entry[criterion].is_string() &&
            entry.contains(uniqueKey) && entry[uniqueKey].is_string()) {

            std::string criterionName = entry[criterion].get<std::string>();
            std::string uniqueName = entry[uniqueKey].get<std::string>();

            mobIndex[uniqueName] = globalIndex;

            if (uniqueValues.find(criterionName) == uniqueValues.end()) {
                order.push_back(criterionName); // Store order of first appearance
            }

            uniqueValues[criterionName].push_back(uniqueName);
            maxCriterionLength = std::max(maxCriterionLength, criterionName.length());
            maxUniqueLength = std::max(maxUniqueLength, uniqueName.length());

            globalIndex++;
        }
    }

    // Print results
    std::cout << "Mob Unique Names by Faction:\n";
    for (const std::string& criterionName : order) {
        const std::vector<std::string>& uniqueNames = uniqueValues[criterionName];

        std::cout << "- " << criterionName << ":\n";
        size_t index = 0;
        for (const auto& uniqueName : uniqueNames) {
            std::cout << "    " << std::setw(maxUniqueLength) << std::left << uniqueName
                << " = " << index << ",\n";
            index++;
        }
        std::cout << "---------------------------------\n";
    }

    //for (const auto& entry : data[firstLevel][secondLevel]) {
    //    if (entry.contains(uniqueKey) && entry[uniqueKey].is_string()) {
    //        std::string uniqueName = entry[uniqueKey].get<std::string>();

    //        if (mobIndex.find(uniqueName) == mobIndex.end()) {
    //            mobIndex[uniqueName] = globalIndex;
    //            order.push_back(uniqueName); // Maintain order of appearance
    //            maxUniqueLength = std::max(maxUniqueLength, uniqueName.length());
    //            globalIndex++;
    //        }
    //    }
    //}

    //// Print all mobs with their global index
    //std::cout << "Mob Unique Names with Global Index:\n";
    //for (const auto& uniqueName : order) {
    //    std::cout << std::setw(maxUniqueLength) << std::left << uniqueName
    //        << " = " << mobIndex[uniqueName] << ",\n";
    //}
    //std::cout << "---------------------------------\n";
}
std::string net::getLocationNameById(const nlohmann::json& locationNames, const std::string& id)
{
    for (const auto& location : locationNames) {
        if (location.contains("Index") && location["Index"] == id) {
            if (location.contains("UniqueName")) {
                //std::cout << location["UniqueName"].get<std::string>() << "\n";
                return location["UniqueName"].get<std::string>();
            }
        }
    }
    std::cout << "Location Name not found for id: " << id << std::endl;
    return {};
}

void net::removeOutdatedTemporaryConnections(const std::string filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << '\n';
        return;
    }

    nlohmann::ordered_json rootJson;
    try {
        inFile >> rootJson;
    }
    catch (...) {
        std::cerr << "Failed to parse JSON from: " << filePath << '\n';
        return;
    }
    inFile.close();

    if (!rootJson.contains("connections") || !rootJson["connections"].is_array()) {
        std::cerr << "No 'connections' array found in JSON.\n";
        return;
    }

    nlohmann::ordered_json& connections = rootJson["connections"];
    nlohmann::ordered_json filteredConnections = nlohmann::json::array();

    auto now = std::chrono::system_clock::now();

    for (const auto& connection : connections) {
        if (!connection.contains("from") || !connection.contains("to")) continue;
        std::string fromId = connection["from"].get<std::string>();
        std::string toId = connection["to"].get<std::string>();

        bool isTemporary =
            fromId.starts_with("TNL") || toId.starts_with("TNL") ||
            fromId.starts_with("@MISTS") || toId.starts_with("@MISTS") ||
            fromId.starts_with("@RANDOMDUNGEON") || toId.starts_with("@RANDOMDUNGEON");

        if (isTemporary) {
            if (!connection.contains("timestamp")) continue; // skip if no timestamp

            try {
                const auto connectionTime = parse_utc_time_string(connection["timestamp"]);
                const auto age_hours = std::chrono::duration_cast<std::chrono::hours>(now - connectionTime).count();

                if (age_hours > 8) {
                    std::cout << "removed connection: " << connection << "\n";
                    std::cout << "age hours: " << age_hours << "\n";
                    continue;  // Skip outdated connection
                }
            }
            catch (const std::exception& ex) {
                std::cerr << ex.what() << '\n';
                continue;
            }
        }

        filteredConnections.push_back(connection);
    }

    // Replace old connections with filtered ones
    rootJson["connections"] = filteredConnections;

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << '\n';
        return;
    }

    outFile << rootJson.dump(4);
    outFile.close();
}

void net::updatePlayerData(Location& currentLocation, const std::string& filePath)
{
    nlohmann::ordered_json playerData;

    playerData["player"]["location"]["id"] = currentLocation._id;

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << '\n';
        return;
    }

    outFile << playerData.dump(4); // Pretty-print with 4 spaces
    outFile.close();
}
void net::parseObjectsFromTemplate(const std::string& filePath)
{
    std::vector<std::pair<float_t, float_t>> objectPositions;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << filePath << std::endl;
        return;
    }

    std::regex fishingZoneRegex(R"(.*_NODE)");

    auto* root = doc.FirstChildElement();
    if (!root) return;

    std::function<void(tinyxml2::XMLElement*)> recursiveSearch = [&](tinyxml2::XMLElement* element) {
        while (element) {
            std::string tag = element->Name();
            if (tag == "tile"/* || tag == "compoundtile"*/) {
                const char* nameAttr = element->Attribute("name");
                const char* posAttr = element->Attribute("pos");
                if (nameAttr && posAttr && std::regex_match(nameAttr, fishingZoneRegex)) {
                    float_t px = 0, py = 0, pz = 0;
                    if (sscanf_s(posAttr, "%f %f %f", &px, &py, &pz) == 3) {
                        objectPositions.emplace_back(px, pz);
                    }
                }
            }
            recursiveSearch(element->FirstChildElement());
            element = element->NextSiblingElement();
        }
        };

    recursiveSearch(root);

    //tinyxml2::XMLElement* root = doc.FirstChildElement("template");
    //if (!root) return;

    //// Navigate to <tiles><layergroup><layer name="T8">
    //tinyxml2::XMLElement* tiles = root->FirstChildElement("tiles");
    //if (!tiles) return;

    //tinyxml2::XMLElement* layerGroup = tiles->FirstChildElement("layergroup");
    //while (layerGroup) {
    //    tinyxml2::XMLElement* layer = layerGroup->FirstChildElement("layer");
    //    while (layer) {
    //        const char* layerName = layer->Attribute("name");
    //        const char* layerId = layer->Attribute("id");
    //        if (layerName && layerId && std::string(layerName) == "T8" &&
    //            std::string(layerId) == "f4cd37b2-8abd-46f4-8135-d172356d12a5") {

    //            tinyxml2::XMLElement* tile = layer->FirstChildElement("tile");
    //            while (tile) {
    //                const char* tileName = tile->Attribute("name");
    //                const char* posAttr = tile->Attribute("pos");

    //                if (tileName && posAttr && std::regex_match(tileName, fishingZoneRegex)) {

    //                    float_t x = 0.0f, y = 0.0f, z = 0.0f;
    //                    if (sscanf_s(posAttr, "%f %f %f", &x, &y, &z) == 3) {
    //                        objectPositions.emplace_back(x, z);  // only X and Z are used
    //                    }
    //                }

    //                tile = tile->NextSiblingElement("tile");
    //            }
    //        }

    //        layer = layer->NextSiblingElement("layer");
    //    }
    //    layerGroup = layerGroup->NextSiblingElement("layergroup");
    //}

    for (size_t i = 0; i < objectPositions.size(); i++) {
        const auto& fishingZone = objectPositions[i];
        entityList._currentLocation._fishNodeList.update(
            FishNode::FishNode(i, "fisn_node", 1, 1, 0, fishingZone.first, fishingZone.second, 1)
        );
        //std::cout << fishingZone.first << " " << fishingZone.second << "\n";
    }
}
void net::makeLocationsConnection(
    Location& locationFrom, Location& locationTo, const bool& isChangingLocation
)
{
    /*if (_currentLocation._id.find("TNL") == std::string::npos and
        _previousLocation._id.find("TNL") == std::string::npos) {
        return;
    }*/

    // temporary locations
    if (locationFrom._name.empty() || locationTo._name.empty()) return;

    const std::string filePath = "location_connections.json";
    nlohmann::ordered_json rootJson;

    // Load existing file
    {
        std::ifstream inputFile(filePath);
        if (inputFile.is_open()) {
            try {
                inputFile >> rootJson;
                if (!rootJson.contains("locations") || !rootJson["locations"].is_array())
                    rootJson["locations"] = nlohmann::json::array();
                if (!rootJson.contains("connections") || !rootJson["connections"].is_array())
                    rootJson["connections"] = nlohmann::json::array();
            }
            catch (...) {
                rootJson["locations"] = nlohmann::json::array();
                rootJson["connections"] = nlohmann::json::array();
            }
            inputFile.close();
        }
        else {
            rootJson["locations"] = nlohmann::json::array();
            rootJson["connections"] = nlohmann::json::array();
        }
    }

    auto& locationsArray = rootJson["locations"];
    auto& connectionsArray = rootJson["connections"];

    // Helper to add location only once by id
    auto addUniqueLocation = [&](const Location& location) {
        for (const auto& existingLoc : locationsArray) {
            if (existingLoc["id"] == location._id) return; // already exists
        }

        locationsArray.push_back({
            { "id", location._id },
            { "name", location._name },
            { "type", location._type },
            { "tier", location._tier },
            { "biome", location._biome }
            });
        };

    // Ensure both locations are added
    addUniqueLocation(locationFrom);
    addUniqueLocation(locationTo);

    // Construct new connection entry
    nlohmann::ordered_json newConnection = {
        { "timestamp", get_utc_time() },
        { "type", isChangingLocation ? "walk" : "teleportation" },
        { "from", locationFrom._id },
        { "to", locationTo._id }
    };

    // Check if this connection already exists (bidirectionally and same type)
    bool alreadyExists = false;
    for (auto& connection : connectionsArray) {
        bool same = connection["from"] == newConnection["from"] && connection["to"] == newConnection["to"];
        bool mirrored = connection["from"] == newConnection["to"] && connection["to"] == newConnection["from"];
        bool sameType = connection["type"] == newConnection["type"];

        if ((same || mirrored) && sameType) {
            connection["timestamp"] = newConnection["timestamp"]; // update timestamp
            alreadyExists = true;
            break;
        }
    }

    if (!alreadyExists) {
        connectionsArray.push_back(newConnection);
        std::cout << "made connection: " << locationFrom._name << " -> " << locationTo._name << "\n";
    }

    // Save updated JSON
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << rootJson.dump(4);
        outFile.close();
    }
    else {
        std::cerr << "Failed to open " << filePath << " for writing\n";
    }
}

void net::parseLocationsAndConnections(const std::string& xmlPath)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlPath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << xmlPath << '\n';
        return;
    }

    std::regex idRegex("^(\\d{4}|DNG.*|PSG.|TNL-\\d{3}.*)$");

    std::unordered_map<std::string, tinyxml2::XMLElement*> validClusters;

    // Collect all clusters with 4-digit numeric ids
    auto* clusters = doc.FirstChildElement("world")->FirstChildElement("clusters");
    if (!clusters) {
        std::cerr << "No <clusters> element found.\n";
        return;
    }

    for (auto* cluster = clusters->FirstChildElement("cluster"); cluster; cluster = cluster->NextSiblingElement("cluster")) {
        std::string clusterId = cluster->Attribute("id");
        if (std::regex_match(clusterId, idRegex)) {
            //std::cout << clusterId << "\n";
            validClusters[clusterId] = cluster;
        }
    }
    
    std::regex targetIdRegex("@(.*)$");

    Location locationFrom, locationTo;

    // For each valid cluster, parse exits
    for (const auto& [clusterId, clusterElem] : validClusters) {
        auto* exits = clusterElem->FirstChildElement("exits");
        if (!exits) continue;

        for (auto* exit = exits->FirstChildElement("exit"); exit; exit = exit->NextSiblingElement("exit")) {
            const char* targetIdAttr = exit->Attribute("targetid");
            if (!targetIdAttr) continue;

            std::smatch match;
            std::string targetIdStr = targetIdAttr;
            if (!std::regex_search(targetIdStr, match, targetIdRegex)) continue;

            std::string targetClusterId = match[1];
            //std::cout << "targetClusterId: " << targetClusterId << "\n";
            //if (validClusters.find(targetClusterId) == validClusters.end()) continue;

            // Prepare both location structures
            locationFrom._id = clusterId;
            locationTo._id = targetClusterId;

            Location::findLocationData(locationFrom);
            Location::findLocationData(locationTo);

            net::makeLocationsConnection(locationFrom, locationTo, true);
        }
    }

    // add locations data even if no connections present
    for (const auto& [clusterId, clusterElem] : validClusters) {
        Location location;
        location._id = clusterId;
        Location::findLocationData(location);

        // Add location only if it's valid
        if (!location._name.empty()) {
            addLocationIfMissing(location);
        }
    }
}
void net::addLocationIfMissing(const Location& location)
{
    const std::string filePath = "location_connections.json";
    nlohmann::ordered_json rootJson;

    // Load JSON
    {
        std::ifstream inputFile(filePath);
        if (inputFile.is_open()) {
            try {
                inputFile >> rootJson;
                if (!rootJson.contains("locations") || !rootJson["locations"].is_array())
                    rootJson["locations"] = nlohmann::json::array();
            }
            catch (...) {
                rootJson["locations"] = nlohmann::json::array();
            }
            inputFile.close();
        }
        else {
            rootJson["locations"] = nlohmann::json::array();
        }
    }

    auto& locationsArray = rootJson["locations"];

    // Check if this location already exists
    for (const auto& existingLoc : locationsArray) {
        if (existingLoc["id"] == location._id) return; // already added
    }

    locationsArray.push_back({
        { "id", location._id },
        { "name", location._name },
        { "type", location._type },
        { "tier", location._tier },
        { "biome", location._biome }
    });


    // Save
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << rootJson.dump(4);
        outFile.close();
        std::cout << "Added data for location: " << location._name << "\n";
    }
    else {
        std::cerr << "Failed to open " << filePath << " for writing\n";
    }
}
void net::searchLocationsTemplates(int32_t x, int32_t y)
{
    const std::filesystem::path rootFolder = "templates/";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootFolder)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".xml")
            continue;

        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(entry.path().string().c_str()) != tinyxml2::XML_SUCCESS)
            continue;

        auto checkPosition = [&](const char* posAttr) -> bool {
            if (!posAttr) return false;
            int px = 0, py = 0, pz = 0;
            if (sscanf_s(posAttr, "%d %d %d", &px, &py, &pz) == 3) {
                return px == x && pz == y;
            }
            return false;
            };

        bool matchFound = false;

        // Recursive check of <tile> and <compoundtile> inside document
        std::function<void(tinyxml2::XMLElement*)> recursiveSearch = [&](tinyxml2::XMLElement* element) {
            while (element) {
                std::string tag = element->Name();
                if ((/*tag == "tile" || */tag == "compoundtile") && checkPosition(element->Attribute("pos"))) {
                    std::cout << "Match in: " << entry.path().string() << std::endl;
                    matchFound = true;
                    return;
                }
                recursiveSearch(element->FirstChildElement());
                element = element->NextSiblingElement();
            }
            };

        tinyxml2::XMLElement* root = doc.FirstChildElement();
        if (root) recursiveSearch(root);
    }


    std::cout << "--------------------------\nSearch finished\n";
}
void net::findLocationsStatistics(const std::string& filePath)
{
    //std::regex locationRegex("^(\\d{4})_(?:WRL|HBS)");
    std::regex locationRegex("^TNL-(\\d{3})");

    std::map<int, int> tierDistribution;
    std::map<std::string, int> biomeDistribution;
    std::unordered_map<std::string, std::vector<std::string>> tierBiomeLocations;

    for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::smatch match;
        if (!std::regex_search(filename, match, locationRegex)) continue;

        std::string locationId = "TNL-" + match.str(1);
        Location location;
        location._id = locationId;

        Location::findLocationData(location);

        Location secondAvalon;

        std::smatch numberMatch;
        if (std::regex_search(locationId, numberMatch, locationRegex)) {
            int avalonNum = std::stoi(numberMatch[1]);
            if (avalonNum <= 200) {
                std::ostringstream secondAvalonId;
                secondAvalonId << "TNL-" << std::setw(3) << std::setfill('0') << (avalonNum + 200);
                secondAvalon._id = secondAvalonId.str();
                Location::findLocationData(secondAvalon);
            }
        }

        if (location._tier != 0 && !location._biome.empty()) {
            tierDistribution[location._tier]++;
            biomeDistribution[location._biome]++;

            std::string tierBiomeKey = "T" + std::to_string(location._tier) + "_" + location._biome;
            tierBiomeLocations[tierBiomeKey].push_back(location._name);
        }

        if (secondAvalon._tier != 0 && !secondAvalon._biome.empty()) {
            tierDistribution[secondAvalon._tier]++;
            biomeDistribution[secondAvalon._biome]++;

            std::string tierBiomeKey = "T" + std::to_string(secondAvalon._tier) + "_" + secondAvalon._biome;
            tierBiomeLocations[tierBiomeKey].push_back(secondAvalon._name);
        }
    }

    size_t totalLocations = 0;
    for (const auto& [tier, count] : tierDistribution) {
        totalLocations += count;
    }
    std::cout << "Total locations: " << totalLocations << "\n\n";

    std::cout << "Tier Distribution:\n";
    for (const auto& [tier, count] : tierDistribution) {
        std::cout << "  Tier " << tier << ": " << count << "\n";
    }

    std::cout << "\nBiome Distribution:\n";
    for (const auto& [biome, count] : biomeDistribution) {
        std::cout << "  " << biome << ": " << count << "\n";
    }

    std::cout << "\nCombined Tier + Biome Distribution:\n";
    std::vector<std::pair<std::string, std::vector<std::string>>> combined(tierBiomeLocations.begin(), tierBiomeLocations.end());

    std::sort(combined.begin(), combined.end(), [](const auto& a, const auto& b) {
        auto extract = [](const std::string& key) {
            size_t underscore = key.find('_');
            int tier = std::stoi(key.substr(1, underscore - 1));
            std::string biome = key.substr(underscore + 1);
            return std::pair{ tier, biome };
            };
        return extract(a.first) < extract(b.first);
    });

    for (const auto& [key, names] : combined) {
        std::vector<std::string> sortedNames = names;

        //std::sort(sortedNames.begin(), sortedNames.end(), [](const std::string& a, const std::string& b) {
        //    auto extractLastPart = [](const std::string& name) -> std::string {
        //        size_t lastDash = name.rfind('-');
        //        return (lastDash != std::string::npos) ? name.substr(lastDash + 1) : name;
        //        };
        //
        //    // First compare by first character
        //    char firstCharA = !a.empty() ? a[0] : '\0';
        //    char firstCharB = !b.empty() ? b[0] : '\0';
        //    if (firstCharA != firstCharB)
        //        return firstCharA < firstCharB;
        //
        //    // If first characters are equal, compare by last part
        //    return extractLastPart(a) < extractLastPart(b);
        //});

        std::cout << key << ": " << sortedNames.size() << " locations\n";
        for (const std::string& name : sortedNames) {
            std::cout << " - " << name << "\n";
        }
    }
}

void net::formatItemsData()
{
    std::ifstream inputFile("items.txt");  // Replace with your file name
    std::ofstream outputFile("formatted_items.txt");

    if (!inputFile || !outputFile) {
        std::cerr << "Error opening file!" << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string itemId, itemTechName;

        // Read item ID (before ':')
        std::getline(ss, itemId, ':');
        std::getline(ss, itemTechName, ':');

        itemId.erase(std::remove_if(itemId.begin(), itemId.end(),
            [](unsigned char c) { return std::isspace(c); }), itemId.end());
        itemTechName.erase(std::remove_if(itemTechName.begin(), itemTechName.end(),
            [](unsigned char c) { return std::isspace(c); }), itemTechName.end());

        // Output in desired format
        outputFile << itemTechName << ", " << itemId << std::endl;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Formatting complete. Output saved to formatted_items.txt" << std::endl;
}

std::chrono::system_clock::time_point net::parse_utc_time_string(const std::string& utcString) {
    std::string trimmed = utcString;
    //std::cout << trimmed << "\n";
    // Remove fractional seconds if present (e.g., .705Z -> Z)
    auto dotPos = trimmed.find(".");
    if (dotPos != std::string::npos) {
        auto zPos = trimmed.find("Z", dotPos);
        if (zPos == std::string::npos) {
            trimmed.append("Z");
        }
        trimmed.erase(dotPos, zPos - dotPos); // remove .xxx part
    }

    std::tm tm = {};
    std::istringstream ss(trimmed);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp: " + trimmed);
    }
    return std::chrono::system_clock::from_time_t(_mkgmtime(&tm)); // UTC to time_point
}
std::string net::get_utc_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm utc_tm = {};
    gmtime_s(&utc_tm, &now_c);

    std::stringstream ss;
    ss << std::put_time(&utc_tm, "%FT%TZ");  // ISO 8601 with 'Z' for UTC
    return ss.str();
}
std::string net::sanitize_filename(const std::string& input) {
    std::string output = input;
    for (char& ch : output) {
        if (ch == ':' || ch == '/' || ch == '\\' || ch == '?' || 
            ch == '*' || ch == '"' || ch == '<' || ch == '>' || ch == '|')
            ch = '_';
    }
    return output;
}
