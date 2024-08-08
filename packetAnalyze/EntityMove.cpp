#include "pch.h"

EntityMove::EntityMove(uint32_t id, float_t positionX, float_t positionY)
{
    _id        = id;
    _positionX = positionX;
    _positionY = positionY;
}

EntityMove::EntityMove(NetworkCommand& entityMove)
{
    _id        = 0;
    _positionX = 0;
    _positionY = 0;

    DataLayout dataLayout{};
    dataLayout.findDataLayout(entityMove);

    DataFragment idFragment = dataLayout.findFragment(0);
    uint8_t idSize = idFragment._dataType._size;
    if 
		(idSize == 1) { _id = net::read_uint8 (entityMove, idFragment._offset); }
	else if 
		(idSize == 2) { _id = net::read_uint16(entityMove, idFragment._offset); }
	else if 
		(idSize == 4) { _id = net::read_uint32(entityMove, idFragment._offset); }
	else if 
		(idSize == 8) { _id = net::read_uint32(entityMove, idFragment._offset + 4); }

    _positionX = net::read_float32big(entityMove, dataLayout.findFragment(1)._offset + 9);
    _positionY = net::read_float32big(entityMove, dataLayout.findFragment(1)._offset + 13);

    if (_positionX == 0   || _positionY == 0  ||
        _positionX > 800  || _positionY > 800 ||
        _positionX < -800 || _positionY < -800) {
        std::cout << "entity coords out of bounds: " <<
            _positionX << " " << _positionY << "\n";
        entityMove.printCommandInOneString();
    }
}
