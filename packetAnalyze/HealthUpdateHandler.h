#ifndef HEALTH_UPDATE
#define HEALTH_UPDATE

struct HealthUpdateHandler
{
	uint64_t _id;
	int32_t _healthChange;
	int32_t _health;

	HealthUpdateHandler(NetworkCommand& rawHealthUpdate);
	HealthUpdateHandler(uint64_t id, int32_t healthChange, int32_t health);

};

#endif