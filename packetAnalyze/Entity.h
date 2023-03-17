#ifndef ENTITY_H
#define ENTITY_H

struct EntityList
{
    std::vector<uint8_t> _harvestableTrackingTiers;
    std::vector<uint8_t> _harvestableTrackingTypes;
    std::vector<uint8_t> _harvestableTrackingEnchantments;
    std::vector<uint8_t> _harvestableTrackingCharges;
    HarvestableList _harvestableList;

    PlayerSelf _player;
    PlayerList _playerList;

    EntityList();

    void drawCharges(uint8_t charges, std::vector<float> harvestableCoords, std::vector<float> playerCoords);
    void drawWindowFrame();
    void drawHarvestables();
    void drawPlayers();
    void colorizeHarvestable(Harvestable harvestable);

    void draw(GLFWwindow* window);
    void DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments);
    std::vector<GLfloat> convertToMapCoordinates(float_t x, float_t y);
    float_t findDistance(float_t x1, float_t y1, float_t x2, float_t y2);

    bool harvestableFiltered(Harvestable harvestable);
    void changeLocation();
    void clear();
};

#endif // !ENTITY_H