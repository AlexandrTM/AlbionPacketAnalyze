#ifndef ENTITY_H
#define ENTITY_H

struct EntityList
{
    HarvestableList _harvestableList;
    HarvestableListFilter _harvestableListFilter;

    PlayerSelf _player;
    PlayerList _playerList;

    EntityList();

    void drawCharges(Harvestable harvestable, std::vector<float> harvestableCoords, std::vector<float> playerCoords);
    void drawWindowFrame();
    void drawHarvestables();
    void drawPlayers();

    std::vector<GLfloat> returnEnchantmentColor(uint8_t enchantment);
    void colorizeHarvestableEnchantment(Harvestable harvestable);
    void colorizeHarvestableTier(Harvestable harvestable, size_t chargeID);

    void draw(GLFWwindow* window);
    void DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments);
    std::vector<GLfloat> convertToMapCoordinates(float_t x, float_t y);
    float_t findDistance(float_t x1, float_t y1, float_t x2, float_t y2);

    bool filterHarvestable(Harvestable harvestable);
    void changeLocation();
    void clear();
};

#endif // !ENTITY_H