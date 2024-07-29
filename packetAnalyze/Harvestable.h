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
    std::vector<HarvestableFilter> _harvestableListFilter = {
        HarvestableFilter(/*wood*/
            {4,5,6,7,8},
            {1,1,1,0,0},
            {1,1,1,0,0}),
        HarvestableFilter(/*rock*/
            {4,5},
            {1,1},
            {1,1}),
        HarvestableFilter(/*fiber*/
            {4,5,6,7,8},
            {2,1,1,0,0},
            {1,1,1,0,0}),
        HarvestableFilter(/*hide*/
            /*{4},
            {1},
            {1}*/),
        HarvestableFilter(/*ore*/
            {4},
            {1},
            {1}),
        HarvestableFilter(/*other*/)
    };

    size_t size();
    HarvestableFilter& operator[](size_t elementIndex);
};

// *****************************************************************************
// ============================== Harvestable ==================================
// *****************************************************************************


struct Harvestable
{
	uint32_t _id;
	uint8_t _type;
	uint8_t _tier;
	uint8_t _enchantment;
	float_t _positionX;
	float_t _positionY;
	uint8_t _charges;

	Harvestable();
	Harvestable(uint32_t id, uint8_t type, uint8_t tier,
				float_t positionX, float_t positionY, 
				uint8_t charges, uint8_t enchantment);
	Harvestable(NetworkCommand& harvestableRaw);

	void printInfo();
};


// *****************************************************************************
// ============================== HarvestableList ==============================
// *****************************************************************************


class HarvestableList
{
private:
	std::vector<Harvestable> _harvestableList;

public:
	HarvestableList();
	HarvestableList(NetworkCommand& harvestableRawList);

	void updateState(NetworkCommand& updateState);

	void printInfo();

	void clear();
	size_t size();
	void update(Harvestable harvestable);
	void update(HarvestableList harvestableList);

	Harvestable& operator[](size_t elementIndex);
	HarvestableList& operator+=(HarvestableList harvestableList);

};

#endif