#include "pch.h"
#include "HealthUpdate.h"

HealthUpdate::HealthUpdate(NetworkCommand& rawHealthUpdate)
{
	_id		      = 0;
	_healthChange = 0;
	_health		  = 0;

	DataLayout dataLayout{};
    dataLayout.findDataLayout(rawHealthUpdate);

    DataFragment idFragment = dataLayout.findFragment(0);
    uint8_t idSize = idFragment._dataType._size;
    if 
		(idSize == 1) { _id = net::read_uint8 (rawHealthUpdate, idFragment._offset); }
	else if 
		(idSize == 2) { _id = net::read_uint16(rawHealthUpdate, idFragment._offset); }
	else if 
		(idSize == 4) { _id = net::read_uint32(rawHealthUpdate, idFragment._offset); }
	else if 
		(idSize == 8) { _id = net::read_uint64(rawHealthUpdate, idFragment._offset); }

	_healthChange = net::read_float32(rawHealthUpdate, dataLayout.findFragment(2)._offset);
	_health		  = net::read_float32(rawHealthUpdate, dataLayout.findFragment(3)._offset);
}

HealthUpdate::HealthUpdate(uint64_t id, uint32_t healthChange, uint32_t health)
{
	_id			  = id;
	_healthChange = healthChange;
	_health		  = health;
}
