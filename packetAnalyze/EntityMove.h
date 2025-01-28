#ifndef ENTITY_MOVE
#define ENTITY_MOVE

struct EntityMove {
    uint64_t _id;
    float_t  _positionX;
    float_t  _positionY;

    EntityMove(uint64_t id, float_t positionX, float_t positionY);
    static uint64_t getIdOfEntityMove(NetworkCommand& entityMove, DataLayout& dataLayout);

    static void updateEntityListMove(NetworkCommand& entityMove, MobList& mobList);
    static void updateEntityListMove(NetworkCommand& entityMove, PlayerList& playerList);

    static bool isCoordinateAdequate(float_t coordinate);
};

#endif