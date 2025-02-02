#include "pch.h"

EntityMove::EntityMove(uint64_t id, float_t positionX, float_t positionY)
{
    _id        = id;
    _positionX = positionX;
    _positionY = positionY;
}

void EntityMove::updateEntityListMove(NetworkCommand& entityMove, MobList& mobList)
{
    uint64_t mob_id = 0;
    float_t positionX = 0;
    float_t positionY = 0;

    DataLayout dataLayout{};
    dataLayout.findDataLayout(entityMove);

    mob_id = getIdOfEntityMove(entityMove, dataLayout);

    for (size_t i = 0; i < mobList.size(); i++) {
        if (mobList[i]._id == mob_id) {
            mobList[i]._positionX = 
                net::read_float32big(entityMove, dataLayout.findFragment(1)._offset + 9);
            mobList[i]._positionY = 
                net::read_float32big(entityMove, dataLayout.findFragment(1)._offset + 13);
            //dataLayout.printInfo(entityMove);
            return;
        }
    }
}

void EntityMove::updateEntityListMove(NetworkCommand& entityMove, PlayerList& playerList)
{
    uint64_t player_id = 0;
    float_t positionX = 0;
    float_t positionY = 0;

    DataLayout dataLayout{};
    dataLayout.findDataLayout(entityMove);
    //dataLayout.printInfo(entityMove);

    player_id = getIdOfEntityMove(entityMove, dataLayout);

    for (size_t i = 0; i < playerList.size(); i++) {
        if (playerList[i]._id == player_id) {
            //entityMove.printCommandInOneString(46, entityMove.size(), true);
            //dataLayout.printInfo(entityMove);
            for (ptrdiff_t bitOffset = 72; bitOffset < entityMove.size() * 8 - 32 - 37 * 8; bitOffset+=8) {
                float_t valueBig = net::read_float32big_from_bits(entityMove, bitOffset + 37 * 8);

                /*if (EntityMove::isCoordinateAdequate(valueBig)) {
                    std::cout << "bit offset big    " << bitOffset << " value: " << valueBig << "\n";
                }*/
            }
            for (ptrdiff_t bitOffset = 72; bitOffset < entityMove.size() * 8 - 32; bitOffset+=8) {
                float_t valueLittle = net::read_float32_from_bits(entityMove, bitOffset);

                /*if (EntityMove::isCoordinateAdequate(valueLittle)) {
                    std::cout << "bit offset little " << bitOffset << " value: " << valueLittle << "\n";
                }*/
            }

            /*playerList[i]._positionX = positionX;
            playerList[i]._positionY = positionY;*/
            break;
        }
    }
    std::cout << "\n";
}

bool EntityMove::isCoordinateAdequate(float_t coordinate)
{
    if ((coordinate < 2000 and coordinate > -2000) and
        (coordinate > 0.001 or coordinate < -0.001 or coordinate == 0.0)) {
        return true;
    }
    return false;
}

uint64_t EntityMove::getIdOfEntityMove(NetworkCommand& entityMove, DataLayout& dataLayout)
{
    uint64_t id = 0;

    id = net::read_integer(entityMove, dataLayout.findFragment(0));

    return id;
}
