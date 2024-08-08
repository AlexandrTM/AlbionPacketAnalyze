#ifndef MOB
#define MOB

struct Mob
{
	uint32_t _id;
	uint8_t  _category;
	uint32_t _health;
	uint8_t  _tier;
	uint8_t  _enchantment;
	float_t  _positionX;
	float_t  _positionY;

	Mob();
	Mob(NetworkCommand& rawMob);
	Mob(uint32_t id, uint8_t category, 
		uint32_t health, uint8_t  tier, uint8_t enchantment,
		float_t  positionX, float_t  positionY);

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
	void update(HealthUpdate healthUpdate);
	void mobChangeState(NetworkCommand& mobChangeState);

	size_t size();
	Mob& operator[](size_t elementIndex);
};

#endif