#ifndef ENTITY_H
#define ENTITY_H


// **************************************************************************
// ============================== Location ==================================
// **************************************************************************


struct Location
{
    uint16_t _locationID = 0;
    HarvestableList _harvestableList = {};
    PlayerList _playerList = {};

    Location(uint16_t locationID, HarvestableList harvestableList, PlayerList playerList);
    static void update(
        std::vector<Location>& locations,
        HarvestableList& currentHarvestableList, PlayerList& currentPlayerList,
        uint16_t newLocationID
    );
};


// **************************************************************************
// ============================== EntityList ==================================
// **************************************************************************


struct EntityList
{
    HarvestableList _harvestableList;
    HarvestableListFilter _harvestableListFilter;

    PlayerSelf _player;
    PlayerList _playerList;

    std::vector<Location> _locations = {};

    EntityList();

    void drawCharges(Harvestable harvestable, std::vector<float> harvestableCoords, std::vector<float> playerCoords);
    void drawWindowFrame();
    void drawHarvestables();
    void drawPlayers();

    std::vector<GLfloat> returnEnchantmentColor(uint8_t enchantment);
    void colorizeHarvestable(Harvestable harvestable);
    void colorizeHarvestableCharge(Harvestable harvestable, size_t chargeID);

    void draw(GLFWwindow* window);
    void DrawCircle(float_t offsetX, float_t offsetY, float_t radius, size_t num_segments);
    std::vector<GLfloat> convertToMapCoordinates(float_t x, float_t y);
    float_t findDistance(float_t x1, float_t y1, float_t x2, float_t y2);

    bool filterHarvestable(Harvestable harvestable);
    void changeLocation(NetworkCommand& command);
    void clear();
};

#endif // !ENTITY_H