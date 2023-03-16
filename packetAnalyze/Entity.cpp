#include "pch.h"

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawWindowFrame();
    drawHarvestables();

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
    }
    else {
        return false;
    }
}
void EntityList::changeLocation()
{
    _harvestableList.clear();

    if (abs(_player._positionX) >= abs(_player._positionY)) {
        _player._positionX = _player._positionX * -1;
    }
    else if (abs(_player._positionY) > abs(_player._positionX)) {
        _player._positionY = _player._positionY * -1;
    }
}

void EntityList::clear()
{
    _harvestableList.clear();
    _player = {};
}

EntityList::EntityList()
{
    _harvestableList = {};
    _harvestableTrackingTiers = { 4,5,6,7,8 };
    _harvestableTrackingTypes = {resourceType::FIBER,12,13,14,15,16};
    _harvestableTrackingEnchantments = { 1,1,1,0,0 };
    _harvestableTrackingCharges = { 1,1,1,0,0 };

    _player = {};
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
    std::vector<GLfloat> _playerMapCoords = convertToMapCoordinates(_player._positionX, _player._positionY);
    for (size_t i = 0; i < _harvestableList.size(); i++) {
        //if (harvestableFiltered(_harvestableList[i])) 
        {
            std::vector<GLfloat> _harvestableMapCoords = convertToMapCoordinates(_harvestableList[i]._positionX, 
                                                                                 _harvestableList[i]._positionY);
            if (_harvestableList[i]._enchantment > 0) {
                glPointSize(_harvestableList[i]._enchantment * 1.5 + _harvestableList[i]._tier * 0.5);
                glBegin(GL_POINTS);
                colorizeEnchantment(_harvestableList[i]._enchantment, _harvestableList[i]._charges);
                glVertex3f(_harvestableMapCoords[0], _harvestableMapCoords[1], 0.0f);
                glEnd();
            }

            drawCharges(_harvestableList[i]._charges, _harvestableMapCoords, _playerMapCoords);

            /*glPointSize(2);
            glBegin(GL_POINTS);
            glColor3f(1,1,1);
            glVertex3f(_harvestableMapCoords[0], _harvestableMapCoords[1], 0.0f);
            glEnd();*/


            /*float_t _distance = sqrt(pow(_harvestableList[i]._positionX - _player._positionX, 2));
            if (_distance > 42) {
                std::cout << _distance << "\n";
            }*/

            /*glPointSize(2);
            glBegin(GL_POINTS);
            if (_harvestableList[i]._type < resourceType::ROCK) {
                glColor3f(1, 1, 0);
            }
            else if (_harvestableList[i]._type < resourceType::FIBER) {
                glColor3f(0.8, 0.8, 0.8);
            }
            else if (_harvestableList[i]._type < resourceType::HIDE) {
                glColor3f(1, 1, 0);
            }
            else if (_harvestableList[i]._type < resourceType::ORE) {
                glColor3f(0.5, 0.5, 0);
            }
            else if (_harvestableList[i]._type >= resourceType::ORE) {
                glColor3f(1, 1, 1);
            }
            else {
                glColor3f(1, 0, 1);
                std::cout << (unsigned)_harvestableList[i]._type << "\n";
            }
            glVertex3f(_harvestableList[i]._positionX / 400 * 0.4921 + _harvestableList[i]._positionY / 400 * 0.4921,
                (-1 * (_harvestableList[i]._positionX / 400 * 0.4921) + _harvestableList[i]._positionY / 400 * 0.4921) * 0.715, 0.0f);
            glEnd();*/
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

void EntityList::colorizeEnchantment(uint8_t enchantment, uint8_t charges)
{
    std::vector<GLfloat> _color = {};
    switch (enchantment)
    {
    case 0:
        _color = { 0, 0, 0 }; 
        break;
    case 1:
        _color = { 0.38f, 0.9f, 0.57f }; 
        break;
    case 2:
        _color = { 0.27f, 0.54f, 0.97f }; 
        break;
    case 3:
        _color = { 0.73f, 0.55f, 0.9f }; 
        break;
    case 4:
        _color = { 0.89f, 0.91f, 0.65f }; 
        break;
    default:
        break;
    }
    if (charges == 0) {
        for (size_t i = 0; i < _color.size(); i++) {
            _color[i] /= 2;
        }
    }
    glColor3f(_color[0], _color[1], _color[2]);
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

float_t _pixelsInMeter = 61.1;
void EntityList::drawCharges(uint8_t charges, std::vector<float> harvestableCoords, std::vector<float> playerCoords)
{
    for (size_t j = 0; j < charges; j++) {
        glPointSize(15);
        glColor3f(0.45 + 0.05f * j, 0.45 + 0.05f * j, 0.75 + 0.05f * j);
        glBegin(GL_POINTS);
        glVertex2f((harvestableCoords[0] - playerCoords[0]) * _pixelsInMeter + (16.0f * j - 39.0f) / 400,
            ((harvestableCoords[1] - playerCoords[1]) * _pixelsInMeter) + (90.0f / 400));
        glEnd();
    }
}