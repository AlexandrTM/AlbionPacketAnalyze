#include "pch.h"

float_t _pixelsInMeter = 45.5;
HarvestableFilter& HarvestableListFilter::operator[](size_t elementIndex)
{
    return _harvestableListFilter[elementIndex];
}

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawWindowFrame();
    drawHarvestables();
    //drawPlayers();
    drawMobs();

    glfwSwapBuffers(window);
}

bool EntityList::filterHarvestable(Harvestable harvestable)
{
    uint8_t filterID = 5;
    if (harvestable._type >=  resourceType::WOOD and 
        harvestable._type <= (resourceType::ROCK - 1)) {
        filterID = 0; }
    if (harvestable._type >=  resourceType::ROCK and 
        harvestable._type <= (resourceType::FIBER - 1)) {
        filterID = 1; } 
    if (harvestable._type >=  resourceType::FIBER and 
        harvestable._type <= (resourceType::HIDE - 1)) {
        filterID = 2; }
    if (harvestable._type >=  resourceType::HIDE and 
        harvestable._type <= (resourceType::ORE - 1)) {
        filterID = 3; }
    if (harvestable._type >=  resourceType::ORE and 
        harvestable._type <= (resourceType::OTHER - 1)) {
        filterID = 4; }

    if (std::isElementInVector(this->_harvestableListFilter[filterID]._trackingTiers, harvestable._tier)) {
        size_t elementIndex = std::findElementIndex(this->_harvestableListFilter[filterID]._trackingTiers, harvestable._tier);
        if (harvestable._charges >= _harvestableListFilter[filterID]._trackingCharges[elementIndex] and
            harvestable._enchantment >= _harvestableListFilter[filterID]._trackingEnchantments[elementIndex]) {
            return true;
        }
    }
    return false;
}
uint32_t currentLocationID = 0;
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
    _harvestableList = {};
    _player          = {};
    _playerList      = {};
    _mobList         = {};
    _locationList    = {};
}

EntityList::EntityList()
{
    _harvestableList = {};
    _player          = {};
    _playerList      = {};
    _mobList         = {};
    _locationList    = {};
}

void EntityList::drawWindowFrame()
{
    glPointSize(1);
    glBegin(GL_LINES);
    glColor4f(0.5, 0.5, 0.5, 0);

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
void EntityList::drawHarvestables()
{
    std::vector<GLfloat> playerCoords = { _player._positionX, _player._positionY };
    std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(_player._positionX, _player._positionY);
    std::vector<GLfloat> harvestableCoords;
    std::vector<GLfloat> harvestableMapCoords;
    for (size_t i = 0; i < _harvestableList.size(); i++) {
        if (filterHarvestable(_harvestableList[i]))
        {
            harvestableCoords = { _harvestableList[i]._positionX, _harvestableList[i]._positionY };
            harvestableMapCoords = convertToMapCoordinates(_harvestableList[i]._positionX,
                _harvestableList[i]._positionY);

            glPointSize(std::max(pow((float)_harvestableList[i]._tier / 4, 2) * 2.8, 4.0));
            glBegin(GL_POINTS);
            colorizeHarvestable(_harvestableList[i]);
            glVertex3f(harvestableMapCoords[0], harvestableMapCoords[1], 0.0f);
            glEnd();

            drawCharges(_harvestableList[i], harvestableCoords, playerCoords);
        }
    }
    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0, 0.65, 0.9);
    glVertex3f(playerMapCoords[0], playerMapCoords[1], 0.0f);
    glEnd();

    glPointSize(2);
    glColor3f(0.85, 0.85, 0.85);
    DrawCircle(playerMapCoords[0], playerMapCoords[1], (float_t)45 / 400, 25);
}
void EntityList::drawPlayers()
{
    std::vector<GLfloat> playerSelfCoords = { _player._positionX, _player._positionY };
    for (size_t i = 0; i < _playerList.size(); i++) {
        std::vector<GLfloat> playerCoords = { _playerList[i]._positionX, _playerList[i]._positionY};
        GLfloat x = playerCoords[0];
        GLfloat y = playerCoords[1];
        std::vector<GLfloat> playerMapCoords = convertToMapCoordinates(x, y);
        if (_playerList[i]._isVisible == true)
        {
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
    for (size_t i = 0; i < _mobList.size(); i++) {
        if (!isMobKnown(_mobList[i]._type, _mobList[i]._category)) {

            std::vector<GLfloat> mobCoords = { _mobList[i]._positionX, _mobList[i]._positionY };
            GLfloat x = mobCoords[0];
            GLfloat y = mobCoords[1];
            std::vector<GLfloat> mobMapCoords = convertToMapCoordinates(x, y);

            glPointSize(4);
            glBegin(GL_POINTS);
            glColor3f(0.8, 0.3, 0.3);
            glVertex3f(mobMapCoords[0], mobMapCoords[1], 0.0f);
            glEnd();
        }
    }
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
            glVertex2f(mapCoords[0] / distance * 8.5f + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / 400,
                mapCoords[1] / distance * 8.5f + 0.24f);
        }
        else {
            glVertex2f(mapCoords[0] + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / 400,
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
    x = x / 400;
    y = y / 400;
    return { (float)((x * _cos + y * _sin) * 0.56 * 0.7366),
             (float)((((-1) * x * _sin) + (y * _cos)) * 0.516 - 0.085)};
}
float_t EntityList::findDistance(float_t x1, float_t y1, float_t x2, float_t y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
