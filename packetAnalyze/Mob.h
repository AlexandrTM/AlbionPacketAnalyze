#ifndef MOB
#define MOB

struct Mob
{
    uint64_t    _id;
    uint16_t    _uniqueValue;
    std::string _uniqueName;
    std::string _category;
    std::string _typeCategory;
    int8_t      _harvestableType;

	uint8_t     _tier;
	uint8_t     _enchantment;
	uint8_t     _charges;

	float_t     _positionX;
	float_t     _positionY;

	uint32_t    _health;
    float_t     _moveSpeed;
    float_t     _maxEnergy;
    float_t     _energyRegeneration;

    //  "WOOD_" "_ROCK" "_ROCK" !SUMMON "_FIBER" "_ORE"
    //  "WOOD_", "_ROCK", "_ROCK", "_FIBER", "_ORE"

	Mob();
	Mob(NetworkCommand& rawMob);
	Mob(
        uint64_t id, uint16_t uniqueValue,
        std::string uniqueName, std::string category, std::string typeCategory,
        uint32_t health,
        uint8_t  tier, uint8_t enchantment, uint8_t charges,
        float_t  positionX, float_t  positionY
    );

	void printInfo() const;
    int8_t findHarvestableType() const;
    bool isHarvestable() const;

};

struct MobList
{
	std::vector<Mob> _mobList;

	MobList();

	void newMob(Mob mob);
	void update(HealthUpdateHandler healthUpdate);
	void mobChangeState(NetworkCommand& mobChangeState);

	size_t size() const;
	Mob& operator[](size_t elementIndex);
};

struct MobFilter
{
    std::vector<uint8_t> _trackingTiers;
    std::vector<uint8_t> _trackingEnchantments;
    std::vector<uint8_t> _trackingCharges;

    MobFilter(std::vector<uint8_t> trackingTiers,
        std::vector<uint8_t> trackingEnchantments);
    MobFilter();
};

struct MobListFilter
{
    /*everything*/
    //std::vector<HarvestableFilter> _harvestableListFilter = {
    //    HarvestableFilter(/*wood*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*rock*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*fiber*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*hide*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*ore*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*other*/)
    //};

    std::vector<MobFilter> _mobListFilter = {
        MobFilter(/*wood*/
            { 4 },
            { 1 }),
        MobFilter(/*rock*/
            { 4 },
            { 1 }),
        MobFilter(/*fiber*/
            { 4, 5 },
            { 2, 1 }),
        MobFilter(/*hide*/
            { 4 },
            { 1 }),
        MobFilter(/*ore*/
            { 4 },
            { 1 }),
        MobFilter(/*other*/)
    };

    size_t size() const;
    MobFilter& operator[](size_t elementIndex);
};

#endif