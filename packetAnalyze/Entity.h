#ifndef ENTITY_H
#define ENTITY_H


struct EntityList
{
    std::vector<uint8_t> _harvestableTrackingTiers;
    std::vector<uint8_t> _harvestableTrackingTypes;
    HarvestableList _harvestableList;

    EntityList();
    void colorizeHarvestables();
    void drawWindowFrame();

    void draw(GLFWwindow* window);

    void clear();
};

#endif // !ENTITY_H