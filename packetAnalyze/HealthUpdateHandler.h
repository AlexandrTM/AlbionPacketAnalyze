#ifndef HEALTH_UPDATE
#define HEALTH_UPDATE

struct HealthUpdateHandler
{
	uint64_t _id;
	uint32_t _healthChange;
	uint32_t _health;

	HealthUpdateHandler(NetworkCommand& rawHealthUpdate);
	HealthUpdateHandler(uint64_t id, uint32_t healthChange, uint32_t health);

};

#endif