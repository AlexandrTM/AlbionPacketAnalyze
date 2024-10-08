#include "pch.h"

float_t _pixelsInMeter = 45.5;
size_t  _halfMapSize = 400;

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawPlayerSelf();
    drawPlayers();
    //drawHarvestables();
    drawMobs();
    drawWindowFrame();
    
    glfwSwapBuffers(window);
}
void EntityList::changeLocation(NetworkCommand& command)
{
    if (abs(_player._positionX) >= abs(_player._positionY)) {
        _player._positionX = _player._positionX * -1;
    }
    else if (abs(_player._positionY) > abs(_player._positionX)) {
        _player._positionY = _player._positionY * -1;
    }
}
void EntityList::clear()
{
    _currentLocation = {};
    _player          = {};
    _locationList    = {};
}
EntityList::EntityList()
{
    _currentLocation = {};
    _player          = {};
    _locationList    = {};
}
void EntityList::drawWindowFrame()
{
    if (_currentLocation._locationID.find("TNL") == std::string::npos) {
        glPointSize(1);
        glBegin(GL_LINES);
        glColor4f(0.5, 0.5, 0.5, 1);

        glVertex2f(0, -0.821);
        glVertex2f(0.59, -0.085);
        glVertex2f(0.59, -0.085);
        glVertex2f(0, 0.653);

        glVertex2f(0, 0.653);
        glVertex2f(-0.59, -0.085);
        glVertex2f(-0.59, -0.085);
        glVertex2f(0, -0.821);
        glEnd();
    }
}
void EntityList::drawPlayerSelf()
{
    std::vector<GLfloat> playerCoords = { _player._positionX, _player._positionY };
    std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(_player._positionX, _player._positionY);

    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0, 0.65, 0.9);
    glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
    glEnd();

    glPointSize(2);
    glColor3f(0.85, 0.85, 0.85);
    DrawCircle(playerMapCoords[0], playerMapCoords[1], (float_t)45 / _halfMapSize, 25);
}
void EntityList::drawHarvestables()
{
    std::vector<GLfloat> playerCoords = { _player._positionX, _player._positionY };
    std::vector<GLfloat> harvestableCoords;
    std::vector<GLfloat> harvestableMapCoords;
    for (size_t i = 0; i < _currentLocation._harvestableList.size(); i++) {
        harvestableCoords = { _currentLocation._harvestableList[i]._positionX, _currentLocation._harvestableList[i]._positionY };
        harvestableMapCoords = convertToMapCoordinates(
                _currentLocation._harvestableList[i]._positionX,
                _currentLocation._harvestableList[i]._positionY);
        if (isHarvestableFiltered(_currentLocation._harvestableList[i])) {

            glPointSize(std::max(pow((float)_currentLocation._harvestableList[i]._tier / 4, 2) * 2.8, 4.0));
            glBegin(GL_POINTS);
            colorizeHarvestable(_currentLocation._harvestableList[i]);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();

            drawCharges(_currentLocation._harvestableList[i], harvestableCoords, playerCoords);
        }
        else if (_currentLocation._harvestableList[i]._tier >= 4) {
            glPointSize(_currentLocation._harvestableList[i]._tier - 3);
            glBegin(GL_POINTS);
            colorizeHarvestable(_currentLocation._harvestableList[i]);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();
        }
    }
}
void EntityList::drawPlayers()
{
    std::vector<GLfloat> playerSelfCoords = { _player._positionX, _player._positionY };
    for (size_t i = 0; i < _currentLocation._playerList.size(); i++) {
        std::vector<GLfloat> playerCoords = { _currentLocation._playerList[i]._positionX, _currentLocation._playerList[i]._positionY};
        GLfloat x = playerCoords[0];
        GLfloat y = playerCoords[1];
        std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(x, y);

        if (_currentLocation._playerList[i]._isVisible == true) {
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3f(0.9, 0.65, 0.65);
            glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
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

    for (size_t i = 0; i < _currentLocation._mobList.size(); i++) {
        std::vector<GLfloat> mobCoords = {
            _currentLocation._mobList[i]._positionX, 
            _currentLocation._mobList[i]._positionY 
        };
        GLfloat x = mobCoords[0];
        GLfloat y = mobCoords[1];
        std::vector<GLfloat> mobMapCoords = convertToMapCoordinates(x, y);

        if (isMobFiltered(_currentLocation._mobList[i], pointSize, color)) {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(mobMapCoords[0], mobMapCoords[1], 0.0f);
            glEnd();
        }
        else if (_currentLocation._mobList[i]._textType == "MOB_NOT_FOUND") {
            glPointSize(7);
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
    if (harvestable._type >=  resourceType::WOOD  and
        harvestable._type <= (resourceType::ROCK  - 1)) { filterID = 0; }
    if (harvestable._type >=  resourceType::ROCK  and
        harvestable._type <= (resourceType::FIBER - 1)) { filterID = 1; }
    if (harvestable._type >=  resourceType::FIBER and
        harvestable._type <= (resourceType::HIDE  - 1)) { filterID = 2; }
    if ((harvestable._type >=  resourceType::HIDE  and
        harvestable._type <= (resourceType::ORE   - 1)) or 
        harvestable._type == 44)                        { filterID = 3; }
    if (harvestable._type >=  resourceType::ORE   and
        harvestable._type <= (resourceType::OTHER - 1)) { filterID = 4; }

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

    if (mob._category == mobCategory::resource or
        mob._category == mobCategory::resourceElemental) {
        uint8_t filterID = 5;

        if (mob._tier > 3) {
            std::string mobTypePrefix = mob._textType.substr(0, 3);

            if      (mobTypePrefix == "WOO") { filterID = 0; }
            else if (mobTypePrefix == "ROC") { filterID = 1; }
            else if (mobTypePrefix == "FIB") { filterID = 2; }
            else if (mobTypePrefix == "HID") { filterID = 3; }
            else if (mobTypePrefix == "ORE") { filterID = 4; }

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
    if (mob._textType.find("ELEMENTAL") != std::string::npos) {
        pointSize = std::max(pow((float)mob._tier / 4, 2) * 2.8, 4.0);
        color = enchantmentColor;
        return true;
    }
    if (mob._textType.find("ASPECT") != std::string::npos) {
        pointSize = std::max(pow((float)mob._tier / 4, 2) * 2.8, 4.0);
        color = enchantmentColor;
        return true;
    }
    if (mob._textType.rfind("TREASURE", 0) == 0) {
        pointSize = 4;
        color = { 0.811f, 0.709f, 0.231f };
        return true;
    }
    if (mob._textType.rfind("AVALONIAN_TREASURE", 0) == 0) {
        pointSize = 5;
        for (size_t i = 0; i < enchantmentColor.size(); i++) {
            enchantmentColor[i] *= 1.025;
        }
        color = enchantmentColor;
        return true;
    }
    if ((mob._textType.find("WISP") != std::string::npos) and mob._enchantment >= 3) {
        pointSize = 4;
        color = enchantmentColor;
        return true;
    }
    if ((mob._textType.find("MOB_BOSS") != std::string::npos)) {
        pointSize = mob._tier - 1;
        color = { 0.8f, 0.435f, 0.435f };
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
void EntityList::drawCharges(Harvestable harvestable, std::vector<float> harvestableCoords,
    std::vector<float> playerCoords)
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

        if (distance > 10 and distance < 50) {
            glVertex2f(
                mapCoords[0] / distance * 8.5f + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / _halfMapSize,
                mapCoords[1] / distance * 8.5f + 0.24f);
        }
        else {
            glVertex2f(
                mapCoords[0] + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / _halfMapSize,
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
std::vector<GLfloat> EntityList::convertToMapCoordinates(float_t x, float_t y)
{
    x = x / _halfMapSize;
    y = y / _halfMapSize;
    return { (float)((x * _cos + y * _sin) * 0.56 * 0.7366),
             (float)((((-1) * x * _sin) + (y * _cos)) * 0.516 - 0.085)};
}
float_t EntityList::findDistance(float_t x1, float_t y1, float_t x2, float_t y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
