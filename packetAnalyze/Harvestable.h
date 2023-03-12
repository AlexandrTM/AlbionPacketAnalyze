#ifndef HARVESTABLE_H
#define HARVESTABLE_H

class Harvestable
{
private:
	uint16_t _id;
	uint8_t _type;
	uint8_t _tier;
	float_t _positionX;
	float_t _positionY;
	uint8_t _charges;

public:
	Harvestable();
	Harvestable(uint16_t id, uint8_t type, uint8_t tier,
				float_t positionX, float_t positionY, uint8_t charges);

	void printInfo();
};

class HarvestableList
{
private:
	std::vector<Harvestable> _harvestableList;

public:
	HarvestableList();
	HarvestableList(NetworkCommand& harvestableList);

	void clear();
	size_t size();
	Harvestable& operator[](size_t elementIndex);
	HarvestableList& operator+=(Harvestable harvestable);

};

#endif