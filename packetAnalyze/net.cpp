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
            if (location.contains("UniqueName"))
                //std::cout << location["UniqueName"].get<std::string>() << "\n";
                return location["UniqueName"].get<std::string>();
        }
    }
    std::cout << "Location Name not found: " << id << std::endl;
    return {};
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
void net::parseObjectsFromTemplate(const std::string& templateFilePath)
{
    std::vector<std::pair<float_t, float_t>> objectPositions;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(templateFilePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << templateFilePath << std::endl;
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
        _entityList._currentLocation._fishNodeList.update(
            FishNode::FishNode(i, "fisn_node", 1, 1, 0, fishingZone.first, fishingZone.second, 1)
        );
        //std::cout << fishingZone.first << " " << fishingZone.second << "\n";
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