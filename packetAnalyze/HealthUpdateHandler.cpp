#include "pch.h"
#include "HealthUpdateHandler.h"

HealthUpdateHandler::HealthUpdateHandler(NetworkCommand& rawHealthUpdate)
{
	_id		      = 0;
	_healthChange = 0;
	_health		  = 0;

	DataLayout dataLayout{};
    dataLayout.findDataLayout(rawHealthUpdate);

    _id = net::read_integer(rawHealthUpdate, dataLayout.findFragment(0));

	_healthChange = net::read_float32(rawHealthUpdate, dataLayout.findFragment(2)._offset);
	_health = net::read_float32(rawHealthUpdate, dataLayout.findFragment(3)._offset);
}

HealthUpdateHandler::HealthUpdateHandler(uint64_t id, uint32_t healthChange, uint32_t health)
{
	_id			  = id;
	_healthChange = healthChange;
	_health		  = health;
}
