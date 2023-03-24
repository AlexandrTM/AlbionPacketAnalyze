#ifndef HARVESTABLE_H
#define HARVESTABLE_H


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