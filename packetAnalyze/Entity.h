#ifndef ENTITY_H
#define ENTITY_H

// **************************************************************************
// ============================== EntityList ================================
// **************************************************************************

struct EntityList
{
    HarvestableListFilter _harvestableListFilter;
    PlayerSelf            _playerSelf;
    MobListFilter         _mobListFilter;

    Location              _currentLocation;
    Location              _previousLocation;
    std::vector<Location> _locationList;
    bool                  _isChangingLocation = false;

    size_t                _visiblePlayers = 0;

    EntityList();

    void drawPlayerSelf();
    void drawCharges(
        Harvestable harvestable, std::vector<float> harvestableCoords, 
        std::vector<float> playerCoords
    );
    void drawWindowFrame(float scale) const;
    void drawHarvestables();
    void drawPlayers();
    void drawMobs();

    bool isMobFiltered(Mob mob, float_t& pointSize, std::vector<float_t>& color);
    bool isHarvestableFiltered(Harvestable harvestable);
    std::vector<GLfloat> returnEnchantmentColor(uint8_t enchantment);
    void colorizeHarvestable(Harvestable harvestable);
    void colorizeMob(Mob mob);
    void colorizeHarvestableCharge(Harvestable harvestable, size_t chargeID);

    void draw(GLFWwindow* window);
    void DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments);
    std::vector<GLfloat> convertToMapCoordinates(float_t x, float_t y) const;
    float_t findDistance(float_t x1, float_t y1, float_t x2, float_t y2);

    void beginChangeLocation(NetworkCommand& command, bool printInfo);
    void endChangeLocation(NetworkCommand& command, bool printInfo);
    void clear();

    void printInfo();
    void printVisiblePlayers();
};

#endif // !ENTITY_H