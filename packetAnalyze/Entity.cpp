#include "pch.h"

void EntityList::draw(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawWindowFrame();
    colorizeHarvestables();

    glfwSwapBuffers(window);
}

void EntityList::clear()
{
    _harvestableList.clear();
}

EntityList::EntityList()
{
    _harvestableList = {};
    _harvestableTrackingTiers = { 4,5,6,7,8 };
    _harvestableTrackingTypes = {resourceType::FIBER,12,13,14,15,16};
    _harvestableTrackingEnchantments = { 1,1,1,0,0 };
}

void EntityList::drawWindowFrame()
{
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);

    glVertex2f(0, -0.715);
    glVertex2f(1, 0);
    glVertex2f(1, 0);
    glVertex2f(0, 0.715);

    glVertex2f(0, 0.715);
    glVertex2f(-1, 0);
    glVertex2f(-1, 0);
    glVertex2f(0, -0.715);
    glEnd();
}

void EntityList::colorizeHarvestables()
{
    for (size_t i = 0; i < _harvestableList.size(); i++) {
        if (std::isElementInVector(this->_harvestableTrackingTiers, _harvestableList[i]._tier)
            and _harvestableList[i]._charges > 0
            and _harvestableList[i]._enchantment >= _harvestableTrackingEnchantments[std::findElementIndex(this->_harvestableTrackingTiers, _harvestableList[i]._tier)]
            and std::isElementInVector(this->_harvestableTrackingTypes, _harvestableList[i]._type)) {

            if (_harvestableList[i]._enchantment > 0) {
                glPointSize(_harvestableList[i]._enchantment * 1.2 + _harvestableList[i]._tier / 2);
                glBegin(GL_POINTS);
                colorizeEnchantment(_harvestableList[i]._enchantment);
                glVertex3f(_harvestableList[i]._positionX / 400 * 0.707 * 0.707 + _harvestableList[i]._positionY / 400 * 0.707 * 0.707 * 0.715,
                    -1 * (_harvestableList[i]._positionX / 400 * 0.707 * 0.707) + _harvestableList[i]._positionY / 400 * 0.707 * 0.707 * 0.715, 0.0f);
                glEnd();
            }

            glPointSize(2);
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
            glVertex3f(_harvestableList[i]._positionX / 400 * 0.707 * 0.707 + _harvestableList[i]._positionY / 400 * 0.707 * 0.707 * 0.715,
                -1 * (_harvestableList[i]._positionX / 400 * 0.707 * 0.707) + _harvestableList[i]._positionY / 400 * 0.707 * 0.707 * 0.715, 0.0f); 
            glEnd();
        }
    }
}

void EntityList::colorizeEnchantment(uint8_t enchantment)
{
    if (enchantment == 1) {
        glColor3f(0.38, 0.9, 0.57);
    }
    else if (enchantment == 2) {
        glColor3f(0.2106, 0.4627, 0.9);
    }
    else if (enchantment == 3) {
        glColor3f(0.65, 0.43, 0.79);
    }
    else if (enchantment == 4) {
        glColor3f(0.87, 0.89, 0.64);
    }
}

