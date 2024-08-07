#ifndef ENTITY_MOVE
#define ENTITY_MOVE

struct EntityMove {
    uint32_t _id;
    float_t  _positionX;
    float_t  _positionY;

    EntityMove(NetworkCommand& entityMove);
    EntityMove(uint32_t id, float_t positionX, float_t positionY);
};

#endif