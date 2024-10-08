#ifndef HARVESTABLE_H
#define HARVESTABLE_H

// *****************************************************************************
// ============================== HarvestableFilter ============================
// *****************************************************************************

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
    /*everything*/
    //std::vector<HarvestableFilter> _harvestableListFilter = {
    //    HarvestableFilter(/*wood*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*rock*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*fiber*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*hide*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*ore*/
    //        {1,2,3,4,5,6,7,8},
    //        {0,0,0,0,0,0,0,0},
    //        {0,0,0,0,0,0,0,0}),
    //    HarvestableFilter(/*other*/)
    //};

    std::vector<HarvestableFilter> _harvestableListFilter = {
        HarvestableFilter(/*wood*/
            { 4 },
            { 2 },
            { 0 }),
        HarvestableFilter(/*rock*/
            { 4, 5 },
            { 3, 2 },
            { 0, 0 }),
        HarvestableFilter(/*fiber*/
            { 4, 5 },
            { 2, 2 },
            { 0, 0 }),
        HarvestableFilter(/*hide*/
            { 4 },
            { 2 },
            { 0 }),
        HarvestableFilter(/*ore*/
            { 4 },
            { 2 },
            { 0 }),
        HarvestableFilter(/*other*/)
    };

    /*
    { 4, 5, 6, 7, 8 },
    { 2, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
    */
    /*
    { 1, 2, 3, 4, 5, 6, 7, 8 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
    */

    size_t size();
    HarvestableFilter& operator[](size_t elementIndex);
};

// *****************************************************************************
// ============================== Harvestable ==================================
// *****************************************************************************


struct Harvestable
{
	uint64_t _id;
	uint8_t  _type;
	uint8_t  _tier;
	uint8_t  _enchantment;
	float_t  _positionX;
	float_t  _positionY;
	uint8_t  _charges;

	Harvestable();
	Harvestable(uint64_t id, uint8_t type, uint8_t tier,
				float_t positionX, float_t positionY, 
				uint8_t charges, uint8_t enchantment);
	Harvestable(NetworkCommand& rawHarvestable);
    static void harvestStart(NetworkCommand& rawHarvestable);
    static void harvestFinished(NetworkCommand& rawHarvestable);

	void printInfo();
    static std::string getHarvestableTextType(uint8_t type);
};


// *****************************************************************************
// ============================== HarvestableList ==============================
// *****************************************************************************


struct HarvestableList
{
	std::vector<Harvestable> _harvestableList;

    using iterator = std::vector<Harvestable>::iterator;

	HarvestableList();
	HarvestableList(NetworkCommand& harvestableRawList);

	void printInfo();

    void updateState(NetworkCommand& updateState);
	void update(Harvestable harvestable);
	void update(HarvestableList harvestableList);
    static bool isResourceStatic(Harvestable harvestable);

    void clear();
    size_t size();
    iterator begin();
    iterator end();

	Harvestable& operator[](size_t elementIndex);
	HarvestableList& operator+=(HarvestableList harvestableList);

};

#endif