#include "pch.h"

float_t _pixelsInMeter = 45.5;
float_t scaleFactor = 1.145f; // 1.145f // 1.118f

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!(_currentLocation._type == "city" || _currentLocation._type == "island")) {
        drawWindowFrame(scaleFactor);
        drawPlayerSelf();
        drawPlayers();
    }
    //drawHarvestables();
    drawMobs();
    
    glfwSwapBuffers(window);
}

void EntityList::endChangeLocation(NetworkCommand& command, bool printInfo)
{
    DataLayout dataLayout{};
    dataLayout.findDataLayout(command);
    //dataLayout.printInfo(command, true);
    //command.printCommandInOneString();
    DataFragment& locationFromFragment = dataLayout.findFragment(65);
    DataFragment& locationToFragment = dataLayout.findFragment(8);
    // поворот по часовой стрелке от оси Х
    //DataFragment& locationToFragment = dataLayout.findFragment(10);
    std::string locationFromId = "";
    std::string locationToId = "";
    locationFromId = net::readString(command, locationFromFragment);
    locationToId = net::readString(command, locationToFragment);
    
    DataFragment& playerGuildFragment = dataLayout.findFragment(57);
    DataFragment& playerAllianceFragment = dataLayout.findFragment(77);

    _playerSelf._guild = net::readString(command, playerGuildFragment);
    _playerSelf._alliance = net::readString(command, playerAllianceFragment);

    //std::cout << "locationFromId: " << locationFromId << " " << "locationToId: " << locationToId << "\n"; 
    //std::cout << "_playerSelf._guild: " << _playerSelf._guild << " " << "_playerSelf._alliance: " << _playerSelf._alliance << "\n";
    if (locationToId == "") {
        dataLayout.printInfo(command);
    }

    //currentHarvestableList.printInfo();
    bool locationToIsNew = true;
    bool locationFromIsNew = true;
    // order is important
    for (Location& location : _locationList) {
        if (location._id == locationFromId) {
            location             = _currentLocation;
            location._playerList = PlayerList();
            _previousLocation = location;
            locationFromIsNew = false;
            break;
        }
    }
    for (Location& location : _locationList) {
        //location.printInfo();
        if (location._id == locationToId) {
            _currentLocation             = location;
            _currentLocation._playerList = PlayerList();
            //currentPlayerList    = location._playerList;
            locationToIsNew = false;
            break;
        }
    }
    if (locationFromIsNew == true) {
        Location newFromLocation = 
            Location(
                locationFromId, "",
                _currentLocation._harvestableList,
                _currentLocation._playerList,
                _currentLocation._mobList,
                _currentLocation._fishNodeList
            );
        _locationList.push_back(newFromLocation);
        _previousLocation = newFromLocation;
    }
    if (locationToIsNew == true) {
        Location newCurrentLocation = Location(locationToId, "", {}, {}, {}, {});
        _locationList.push_back(newCurrentLocation);
        _currentLocation = newCurrentLocation;
    }

    if (printInfo) { this->printInfo(); }
}
void EntityList::beginChangeLocation(NetworkCommand& command, bool printInfo)
{
    //std::cout << "begin changing cocation\n";
    /*if (abs(_playerSelf._positionX) >= abs(_playerSelf._positionY)) {
        _playerSelf._positionX = _playerSelf._positionX * -1;
    }
    else if (abs(_playerSelf._positionY) > abs(_playerSelf._positionX)) {
        _playerSelf._positionY = _playerSelf._positionY * -1;
    }*/
    if (printInfo) { this->printInfo(); }

    _playerSelf._positionX = 10000;
    _playerSelf._positionY = 10000;
}
void EntityList::clear()
{
    _currentLocation = {};
    _playerSelf      = {};
    _locationList    = {};
}
EntityList::EntityList()
{
    _currentLocation = {};
    _playerSelf      = {};
    _locationList    = {};
}
void EntityList::drawWindowFrame(float scale) const
{
    //if (_currentLocation._type == "avalon") return;

    glPointSize(1);
    glBegin(GL_LINES);
    glColor4f(0.5, 0.5, 0.5, 1);

    glVertex2f(0, -0.821 * scale);
    glVertex2f(0.59 * scale, -0.085 * scale);
    glVertex2f(0.59 * scale, -0.085 * scale);
    glVertex2f(0, 0.653 * scale);
        
    glVertex2f(0, 0.653 * scale);
    glVertex2f(-0.59 * scale, -0.085 * scale);
    glVertex2f(-0.59 * scale, -0.085 * scale);
    glVertex2f(0, -0.821 * scale);

    glEnd();
}
void EntityList::drawPlayerSelf()
{
    std::vector<GLfloat> playerCoords = { _playerSelf._positionX, _playerSelf._positionY };
    std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(_playerSelf._positionX, _playerSelf._positionY);

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0, 0.65, 0.9);
    glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
    glEnd();

    //if (_currentLocation._type == "avalon") return;

    glPointSize(2);
    glColor3f(0.85, 0.85, 0.85);
    DrawCircle(playerMapCoords[0], playerMapCoords[1], (float_t)50 / _currentLocation._halfSize, 25);
}
void EntityList::drawHarvestables()
{
    std::vector<GLfloat> playerCoords = { _playerSelf._positionX, _playerSelf._positionY };
    std::vector<GLfloat> harvestableCoords;
    std::vector<GLfloat> harvestableMapCoords;
    for (const Harvestable& harvestable : _currentLocation._harvestableList) {
        harvestableCoords = { harvestable._positionX, harvestable._positionY };
        harvestableMapCoords = convertToMapCoordinates(
            harvestable._positionX,
            harvestable._positionY
        );

        if (isHarvestableFiltered(harvestable)) {
            glPointSize(std::max(pow((float)harvestable._tier / 4, 2) * 2.8, 4.0));
            glBegin(GL_POINTS);
            colorizeHarvestable(harvestable);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
            
            drawCharges(harvestable, harvestableCoords, playerCoords);
        }
        else if (harvestable._tier >= 4) {
            glPointSize(std::min(harvestable._enchantment + 1, 10));
            glBegin(GL_POINTS);
            colorizeHarvestable(harvestable);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
        }
    }

    for (const FishNode& fishNode : _currentLocation._fishNodeList) {
        harvestableCoords = { fishNode._positionX, fishNode._positionY };
        harvestableMapCoords = convertToMapCoordinates(
            fishNode._positionX,
            fishNode._positionY
        );

        if (fishNode._name.find("Chest") != std::string::npos) {
            glPointSize(4);
            glBegin(GL_POINTS);
            glColor3f(0.92f, 0.92f, 0.231f);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
        }
        else if (fishNode._name != "" and fishNode._charges > 0) {
            glPointSize(2 + fishNode._charges);
            glBegin(GL_POINTS);
            glColor3f(0.85f, 0.5f, 0.85f);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
        }
        else {
            glPointSize(3);
            glBegin(GL_POINTS);
            glColor3f(0.6f, 0.4f, 0.6f);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
        }
    }
}
void EntityList::drawPlayers()
{
    std::vector<GLfloat> playerSelfCoords = { _playerSelf._positionX, _playerSelf._positionY };
    //_playerSelf.printInfo();
    for (const Player& player : _currentLocation._playerList._playerList) {
        bool playerFromSelfGuild = player._guild == _playerSelf._guild && !_playerSelf._guild.empty();
        bool playerFromSelfAlliance = player._alliance == _playerSelf._alliance && !_playerSelf._alliance.empty();

        if (playerFromSelfGuild || playerFromSelfAlliance) {
            //player.printInfo();
            continue;
        }

        std::vector<GLfloat> playerCoords = { player._positionX, player._positionY};
        GLfloat x = playerCoords[0];
        GLfloat y = playerCoords[1];
        std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(x, y);

        if (player._isVisible) {
            glPointSize(5);
            glBegin(GL_POINTS);
            /*if (playerFromSelfGuild) {
                glColor3f(0.5, 0.9, 0.5);
                glVertex3f(0.0f, 0.0f, 0.0f);
            }
            else if (playerFromSelfAlliance) {
                glColor3f(0.9, 0.2, 0.9);
                glVertex3f(20.0f, 0.0f, 0.0f);
            }
            else {*/
                glColor3f(0.9, 0.65, 0.65);
                glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
            //}
            glEnd();

            x = (x - playerSelfCoords[0]) * _pixelsInMeter;
            y = (y - playerSelfCoords[1]) * _pixelsInMeter;
            playerMapCoords = convertToMapCoordinates(x, y);

            //glPointSize(15);
            glBegin(GL_LINES);
            glColor3f(0.9, 0.9, 0.9);
            glVertex3f(0.0f, 0.085f, 0.0f);
            glVertex3f(playerMapCoords[0], playerMapCoords[1] + 0.1f, 0.0f);
            glEnd();

            //std::cout << _playerList[i]._positionX << " " << _playerList[i]._positionY << "\n";
        }
        else {
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3f(0.6, 0.42, 0.42);
            glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
            glEnd();
        }
    }
}
void EntityList::drawMobs()
{
    float_t pointSize = 0;
    std::vector<float_t> color = { 0.7f, 0.7f, 0.7f };

    for (const Mob& mob : _currentLocation._mobList._mobs) {
        std::vector<GLfloat> mobCoords = {
            mob._positionX,
            mob._positionY
        };
        GLfloat x = mobCoords[0];
        GLfloat y = mobCoords[1];
        std::vector<GLfloat> mobMapCoords = convertToMapCoordinates(x, y);

        if (isMobFiltered(mob, pointSize, color)) {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(mobMapCoords[0], mobMapCoords[1], 0.0f);
            glEnd();
        }
        else if (mob._uniqueName == "") {
            glPointSize(6);
            glBegin(GL_POINTS);
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(mobMapCoords[0], mobMapCoords[1], 0.0f);
            glEnd();
        }
    }
}






bool EntityList::isHarvestableFiltered(Harvestable harvestable)
{
    uint8_t filterID = 5;
    for (size_t i = 0; i < staticHarvestableRanges.size(); ++i) {
        const auto& range1 = staticHarvestableRanges[i].first;
        const auto& range2 = staticHarvestableRanges[i].second;

        // Check if the resource falls within either of the two valid ranges
        if ((harvestable._type >= range1.start && harvestable._type <= range1.end) ||
            (harvestable._type >= range2.start && harvestable._type <= range2.end)) {
            filterID = i;
            break;
        }
    }

    HarvestableFilter harvestableFilter = this->_harvestableListFilter[filterID];

    if (std::isElementInVector(harvestableFilter._trackingTiers, harvestable._tier)) {
        size_t elementIndex = std::findElementIndex(harvestableFilter._trackingTiers, harvestable._tier);
        if (harvestable._charges >= harvestableFilter._trackingCharges[elementIndex] and
            harvestable._enchantment >= harvestableFilter._trackingEnchantments[elementIndex]) {
            return true;
        }
    }
    return false;
}
bool EntityList::isMobFiltered(Mob mob, float_t& pointSize, std::vector<float_t>& color)
{
    std::vector<GLfloat> enchantmentColor = returnEnchantmentColor(mob._enchantment);

    for (size_t i = 0; i < color.size(); i++) {
        enchantmentColor[i] += (mob._tier - 4) * 0.033f;
    }

    if (mob.isHarvestable()) {
        //std::cout << "_uniqueName: " << mob._uniqueName << "\n";
        uint8_t filterID = static_cast<uint8_t>(mob._harvestableType);

        // Check if _uniqueName contains any of the special names
        if (std::any_of(harvestableSpecialNames.begin(), harvestableSpecialNames.end(),
            [&](const std::string& keyword) { return mob._uniqueName.find(keyword) != std::string::npos; })) {
            pointSize = std::max(pow((float)mob._tier / 4, 2) * 2.8, 4.0);
            color = enchantmentColor;
            //std::cout << "special mob!\n";
            return true;
        }

        if (mob._tier > 3) {
            HarvestableFilter mobFilter = this->_harvestableListFilter[filterID];

            pointSize = std::max(pow((float)mob._tier / 4, 2) * 2.8, 4.0);
            color = enchantmentColor;

            if (std::isElementInVector(mobFilter._trackingTiers, mob._tier)) {
                size_t elementIndex = std::findElementIndex(mobFilter._trackingTiers, mob._tier);
                if (mob._enchantment >= mobFilter._trackingEnchantments[elementIndex]) {
                    return true;
                }
            }
        }
    }
    /*if (mob._typeCategory == "champion") {
        pointSize = 3;
        color = { 0.8f, 0.4f, 0.4f };
        return true;
    }
    if (mob._typeCategory == "boss") {
        pointSize = 3;
        color = { 0.6f, 0.3f, 0.3f };
        return true;
    }*/
    if (mob._uniqueName.find("_CRYSTAL") != std::string::npos) {
        pointSize = 5;
        color = { 0.38, 0.733, 0.937 };
        return true;
    }
    if (mob._category == "hiddentreasures") {
        pointSize = 4;
        color = { 0.811f, 0.709f, 0.231f };
        return true;
    }
    if (mob._category == "treasuredrones") {
        pointSize = 5;
        for (size_t i = 0; i < enchantmentColor.size(); i++) {
            enchantmentColor[i] *= 1.025;
        }
        color = enchantmentColor;
        return true;
    }
    if (mob._uniqueName.find("WISP_RESOURCE") != std::string::npos) {
        pointSize = 4;
        for (size_t i = 0; i < enchantmentColor.size(); i++) {
            enchantmentColor[i] *= 1.1;
        }
        color = enchantmentColor;
        return true;
    }
    else if ((mob._uniqueName.find("WISP") != std::string::npos) and mob._enchantment >= 2) {
        pointSize = 7;
        color = enchantmentColor;
        return true;
    }
    return false;
}
std::vector<GLfloat> EntityList::returnEnchantmentColor(uint8_t enchantment)
{
    switch (enchantment)
    {
    case 0:
        return { 0.7f, 0.7f, 0.7f }; break;
    case 1:
        return { 0.34f, 0.82f, 0.54f }; break;
    case 2:
        return { 0.27f, 0.54f, 0.92f }; break;
    case 3:
        return { 0.75f, 0.65f, 0.93f }; break;
    case 4:
        return { 0.88f, 0.92f, 0.64f }; break;
    default:
        return { 1, 0, 1 }; break;
    }
}
void EntityList::colorizeHarvestable(Harvestable harvestable)
{
    std::vector<GLfloat> color = returnEnchantmentColor(harvestable._enchantment);

    if (harvestable._charges == 0) {
        for (size_t i = 0; i < color.size(); i++) {
            color[i] /= 1.6f;
        }
    }
    else {
        for (size_t i = 0; i < color.size(); i++) {
            color[i] += (harvestable._tier - 4) * 0.033f;
        }
    }
    glColor3f(color[0], color[1], color[2]);
}
void EntityList::colorizeMob(Mob mob)
{
    std::vector<GLfloat> color = returnEnchantmentColor(mob._enchantment);

    for (size_t i = 0; i < color.size(); i++) {
        color[i] += (mob._tier - 4) * 0.033f;
    }

    glColor3f(color[0], color[1], color[2]);
}
void EntityList::colorizeHarvestableCharge(Harvestable harvestable, size_t chargeID)
{
    glColor3f(0.55 + 0.04f * chargeID, 0.4 + 0.04f * chargeID, 0.5 + 0.04f * chargeID);
}
void EntityList::drawCharges(
    Harvestable harvestable, std::vector<float> harvestableCoords,
    std::vector<float> playerCoords
)
{
    float_t chargeSize = 12;
    glPointSize(chargeSize);
    glBegin(GL_POINTS);

    GLfloat x;
    GLfloat y;
    std::vector<GLfloat> mapCoords;
    float_t distance;

    uint8_t charges = harvestable._charges;

    for (size_t j = 0; j < charges; j++) {
        colorizeHarvestableCharge(harvestable, j);

        x = (harvestableCoords[0] - playerCoords[0]) * _pixelsInMeter;
        y = (harvestableCoords[1] - playerCoords[1]) * _pixelsInMeter;
        distance = findDistance(harvestableCoords[0], harvestableCoords[1],
                                        playerCoords[0], playerCoords[1]);
        mapCoords = convertToMapCoordinates(x, y);

        if (distance > 10 and distance < 60) {
            glVertex2f(
                mapCoords[0] / distance * 8.5f + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / _currentLocation._halfSize,
                mapCoords[1] / distance * 8.5f + 0.24f);
        }
        else {
            glVertex2f(
                mapCoords[0] + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / _currentLocation._halfSize,
                mapCoords[1] + 0.32f);
        }
    }
    glEnd();
}

void EntityList::DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments)
{
    glBegin(GL_LINE_LOOP);
    for (size_t i = 0; i < num_segments; i++) {
        float_t theta = 2.0f * 3.1415926f * i / num_segments;

        glVertex2f(offsetX + radius * cosf(theta) * 0.5647, offsetY + radius * sinf(theta) * 0.7085);
    }
    glEnd();
}

float_t _cos = 0.7071/*0.8159*/;
float_t _sin = 0.7071/*0.5781*/;
std::vector<GLfloat> EntityList::convertToMapCoordinates(float_t x, float_t y) const
{
    x = (x - _currentLocation._fromCenterOffset.x) / _currentLocation._halfSize;
    y = (y - _currentLocation._fromCenterOffset.y) / _currentLocation._halfSize;
    //x = x / _currentLocation._halfSize;
    //y = y / _currentLocation._halfSize;
    return { (float)((x * _cos + y * _sin)            * 0.560 * 0.7366 * 83 / 80) * scaleFactor,
             (float)((((-1) * x * _sin) + (y * _cos)) * 0.516 * 83 / 80 - 0.085) * scaleFactor };
}
float_t EntityList::findDistance(float_t x1, float_t y1, float_t x2, float_t y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void EntityList::printInfo()
{
    auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timeBuffer[30];
    ctime_s(timeBuffer, sizeof(timeBuffer), &timeNow);

    std::cout << timeBuffer << "\n" << "num of locations: " << _locationList.size() << "\n";
    /*for (size_t i = 0; i < locations.size(); i++) {
        std::cout <<
            "location id" << i << ": " << locations[i]._id          << "\n" <<
            "num of harvestables: "    << locations[i]._harvestableList.size() << "\n" <<
            "num of mobs:         "    << locations[i]._mobList.size()         << "\n\n";
    }*/

    std::cout <<
        "from: " << _previousLocation._name << " -> " <<
        "to: " << _currentLocation._name << "\n";

    std::cout <<
        "current location:    " << _currentLocation._id << "\n" <<
        "num of harvestables: " << _currentLocation._harvestableList.size() << "\n" <<
        "num of mobs:         " << _currentLocation._mobList.size() << "\n\n";

    // Print information about the location you're coming from (locationFrom)
    std::cout << "From location: " << _previousLocation._id << "\n";
    for (Location& location : _locationList) {
        if (location._id == _previousLocation._id) {
            std::cout << "Number of harvestables: " << location._harvestableList.size() << "\n";
            std::cout << "Number of mobs: " << location._mobList.size() << "\n";
            location._harvestableList.printInfo(); // Print detailed info of the harvestables
            break;
        }
    }
}

void EntityList::printVisiblePlayers()
{
    _visiblePlayers = 0;
    for (auto& player : _currentLocation._playerList._playerList) {
        if (player._isVisible) {
            _visiblePlayers += 1;
        }
    }
    if (_visiblePlayers > 0) {
        std::cout << net::get_utc_time() << " visible players: " << _visiblePlayers << "\n";
    }
}
