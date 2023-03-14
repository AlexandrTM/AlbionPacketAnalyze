#include "pch.h"

void Entities::draw(GLFWwindow* window, HarvestableList entityList)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glPointSize(3);

    glColor3f(1, 1, 0);
    for (size_t i = 0; i < entityList.size(); i++) {
        glBegin(GL_POINTS);

        glVertex3f(entityList[i]._positionX / 420, entityList[i]._positionY / 420, 0.0f);

        glEnd();
    }

    glfwSwapBuffers(window);
}

Entities::Entities()
{

}