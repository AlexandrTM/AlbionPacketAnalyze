#ifndef ENTITY_H
#define ENTITY_H

struct EntityList
{
    std::vector<uint8_t> _harvestableTrackingTiers;
    std::vector<uint8_t> _harvestableTrackingTypes;
    std::vector<uint8_t> _harvestableTrackingEnchantments;
    std::vector<uint8_t> _harvestableTrackingCharges;
    HarvestableList _harvestableList;

    Player _player;

    EntityList();

    void drawCharges(uint8_t charges, std::vector<float> harvestableCoords, std::vector<float> playerCoords);
    void drawWindowFrame();
    void drawHarvestables();
    void colorizeEnchantment(uint8_t enchantment, uint8_t charges);

    void draw(GLFWwindow* window);
    void DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments);
    std::vector<GLfloat> convertToMapCoordinates(float_t x, float_t y);

    bool harvestableFiltered(Harvestable harvestable);
    void changeLocation();
    void clear();
};

#endif // !ENTITY_H