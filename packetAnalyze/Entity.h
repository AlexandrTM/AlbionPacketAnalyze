#ifndef ENTITY_H
#define ENTITY_H


struct EntityList
{
    std::vector<uint8_t> _harvestableTrackingTiers;
    std::vector<uint8_t> _harvestableTrackingTypes;
    std::vector<uint8_t> _harvestableTrackingEnchantments;

    HarvestableList _harvestableList;

    EntityList();

    void drawWindowFrame();
    void colorizeHarvestables();
    void colorizeEnchantment(uint8_t enchantment);

    void draw(GLFWwindow* window);

    void clear();
};

#endif // !ENTITY_H