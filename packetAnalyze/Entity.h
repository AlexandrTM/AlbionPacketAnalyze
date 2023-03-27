#ifndef ENTITY_H
#define ENTITY_H


struct HarvestableFilter
{
    std::vector<uint8_t> _trackingTiers;
    std::vector<uint8_t> _trackingEnchantments;
    std::vector<uint8_t> _trackingCharges;

    HarvestableFilter(std::vector<uint8_t> trackingTiers, 
                      std::vector<uint8_t> trackingEnchantments,
                      std::vector<uint8_t> trackingCharges);
    HarvestableFilter();
};

struct HarvestableListFilter
{
    std::vector<HarvestableFilter> _harvestableListFilter = { 
        HarvestableFilter(/*wood*/
            {4,5},
            {0,1},
            {1,1}),
        HarvestableFilter(/*rock*/
            {1,4,5},
            {0,0,1},
            {1,1,1}),
        HarvestableFilter(/*fiber*/
            {4,5,6,7,8},
            {0,1,1,0,0},
            {1,1,1,0,0}),
        HarvestableFilter(/*hide*/
            {4},
            {0},
            {1}),
        HarvestableFilter(/*ore*/
            {4},
            {0},
            {1}),
        HarvestableFilter(/*other*/) 
    };

    size_t size();
    HarvestableFilter& operator[](size_t elementIndex);
};

struct EntityList
{
    HarvestableList _harvestableList;
    HarvestableListFilter _harvestableListFilter;

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

    bool filterHarvestable(Harvestable harvestable);
    void changeLocation();
    void clear();
};

#endif // !ENTITY_H