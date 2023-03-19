#include "pch.h"

float_t _pixelsInMeter = 45.5;

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawWindowFrame();
    drawHarvestables();
    drawPlayers();

    glfwSwapBuffers(window);
}

bool EntityList::harvestableFiltered(Harvestable harvestable)
{
    if (std::isElementInVector(this->_harvestableTrackingTiers, harvestable._tier) and 
        std::isElementInVector(this->_harvestableTrackingTypes, harvestable._type)) {
        size_t elementIndex = std::findElementIndex(this->_harvestableTrackingTiers, harvestable._tier);
        if (harvestable._charges >= _harvestableTrackingCharges[elementIndex] and
            harvestable._enchantment >= _harvestableTrackingEnchantments[elementIndex]) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
void EntityList::changeLocation()
{
    _harvestableList = {};
    _playerList = {};

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
    _player = {};
    _playerList = {};
}

EntityList::EntityList()
{
    _harvestableList = {};
    _harvestableTrackingTiers = { 4,5,6 };
    _harvestableTrackingTypes = {resourceType::WOOD,1,2,3,4,5};
    _harvestableTrackingEnchantments = { 1,1,0,0,0 };
    _harvestableTrackingCharges = { 1,1,1,0,0 };

    _player = {};
    _playerList = {};
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

void EntityList::drawHarvestables()
{
    std::vector<GLfloat> _playerCoords = { _player._positionX, _player._positionY };
    std::vector<GLfloat> _playerMapCoords = convertToMapCoordinates(_player._positionX, _player._positionY);
    std::vector<GLfloat> _harvestableCoords;
    std::vector<GLfloat> _harvestableMapCoords;
    for (size_t i = 0; i < _harvestableList.size(); i++) {
        if (harvestableFiltered(_harvestableList[i])) 
        {
            _harvestableCoords = { _harvestableList[i]._positionX, _harvestableList[i]._positionY };
            _harvestableMapCoords = convertToMapCoordinates(_harvestableList[i]._positionX, 
                                                            _harvestableList[i]._positionY);
            
            glPointSize(std::max(pow((float)_harvestableList[i]._tier / 4, 2) * 2.65, 4.0));
            glBegin(GL_POINTS);
            colorizeHarvestable(_harvestableList[i]);
            glVertex3f(_harvestableMapCoords[0], _harvestableMapCoords[1], 0.0f);
            glEnd();

            drawCharges(_harvestableList[i]._charges, _harvestableCoords, _playerCoords);
        }
    }
    glPointSize(4);
    glBegin(GL_POINTS);
    glColor3f(0, 0.65, 0.9);
    glVertex3f(_playerMapCoords[0], _playerMapCoords[1], 0.0f);
    glEnd();

    glPointSize(2);
    glColor3f(0.85, 0.85, 0.85);
    DrawCircle(_playerMapCoords[0], _playerMapCoords[1], (float_t)45 / 400, 25);
}

void EntityList::drawPlayers()
{
    std::vector<GLfloat> _playerSelfCoords = { _player._positionX, _player._positionY };
    for (size_t i = 0; i < _playerList.size(); i++) {
        std::vector<GLfloat> _playerCoords = { _playerList[i]._positionX, _playerList[i]._positionY};
        GLfloat x = _playerCoords[0];
        GLfloat y = _playerCoords[1];
        std::vector<GLfloat> _playerMapCoords = convertToMapCoordinates(x, y);
        if (findDistance(_playerSelfCoords[0], _playerSelfCoords[1], _playerCoords[0], _playerCoords[1]) < 72) 
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3f(0.9, 0.65, 0.65);
            glVertex3f(_playerMapCoords[0], _playerMapCoords[1], 0.0f);
            glEnd();

            x = (x - _playerSelfCoords[0]) * _pixelsInMeter;
            y = (y - _playerSelfCoords[1]) * _pixelsInMeter;
            _playerMapCoords = convertToMapCoordinates(x, y);

            glPointSize(15);
            glBegin(GL_POINTS);
            glColor3f(0.9, 0.9, 0.9);
            //glVertex3f(0.0f, 0.085f, 0.0f);
            glVertex3f(_playerMapCoords[0], _playerMapCoords[1], 0.0f);
            glEnd();

            //std::cout << _playerList[i]._positionX << " " << _playerList[i]._positionY << "\n";
        }
        else {
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3f(0.6, 0.42, 0.42);
            glVertex3f(_playerMapCoords[0], _playerMapCoords[1], 0.0f);
            glEnd();
        }
    }
}

void EntityList::colorizeHarvestable(Harvestable harvestable)
{
    std::vector<GLfloat> _color = {};
    switch (harvestable._enchantment)
    {
    case 0:
        _color = { 0.7f, 0.7f, 0.7f }; break;
    case 1:
        _color = { 0.34f, 0.82f, 0.54f }; break;
    case 2:
        _color = { 0.27f, 0.54f, 0.92f }; break;
    case 3:
        _color = { 0.75f, 0.65f, 0.93f }; break;
    case 4:
        _color = { 0.88f, 0.92f, 0.64f }; break;
    default:
        _color = { 1, 0, 1 }; break;
    }
    
    if (harvestable._charges == 0) {
        for (size_t i = 0; i < _color.size(); i++) {
            _color[i] /= 1.6f;
        }
    }
    else {
        for (size_t i = 0; i < _color.size(); i++) {
            _color[i] += (harvestable._tier - 4) * 0.033f;
        }
    }
    glColor3f(_color[0], _color[1], _color[2]);
}
void EntityList::drawCharges(uint8_t charges, std::vector<float> harvestableCoords, std::vector<float> playerCoords)
{
    float_t chargeSize = 15;
    glPointSize(chargeSize);
    glBegin(GL_POINTS);
    GLfloat x;
    GLfloat y;
    std::vector<GLfloat> mapCoords;
    float_t distance;
    for (size_t j = 0; j < charges; j++) {
        glColor3f(0.55 + 0.04f * j, 0.4 + 0.04f * j, 0.5 + 0.04f * j);
        x = (harvestableCoords[0] - playerCoords[0]) * _pixelsInMeter;
        y = (harvestableCoords[1] - playerCoords[1]) * _pixelsInMeter;
        distance = findDistance(harvestableCoords[0], harvestableCoords[1],
                                        playerCoords[0], playerCoords[1]);
        mapCoords = convertToMapCoordinates(x, y);

        if (distance > 20 and distance < 50) {
            glVertex2f(mapCoords[0] / distance * 8.5f + ((chargeSize * 0.58f) * (j - (float)charges / 2)) / 400,
                mapCoords[1] / distance * 8.5f + 0.2f);
        }
        else {
            glVertex2f(mapCoords[0] + ((chargeSize * 0.82f) * (j - (float)charges / 2)) / 400,
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

