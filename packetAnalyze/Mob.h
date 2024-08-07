#ifndef MOB
#define MOB

struct Mob
{
	uint32_t _id;
	float_t  _positionX;
	float_t  _positionY;
	uint32_t _health;
	uint8_t  _tier;
	uint8_t  _enchantment;

	Mob();
	Mob(NetworkCommand& rawMob);

	void printInfo();

};

class MobList
{
private:
	std::vector<Mob> _mobList;

public:
	MobList();

	void newMob(Mob mob);
	void update(EntityMove mobMove);

	size_t size();
	Mob& operator[](size_t elementIndex);
};

#endif