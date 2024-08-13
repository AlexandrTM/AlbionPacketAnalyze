#ifndef HEALTH_UPDATE
#define HEALTH_UPDATE

struct HealthUpdate
{
	uint64_t _id;
	uint32_t _healthChange;
	uint32_t _health;

	HealthUpdate(NetworkCommand& rawHealthUpdate);
	HealthUpdate(uint64_t id, uint32_t healthChange, uint32_t health);

};

#endif