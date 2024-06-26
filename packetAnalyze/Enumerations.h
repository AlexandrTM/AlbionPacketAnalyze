#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

enum operationType: uint8_t
{
    operationRequest = 2,
    operationResponse = 3,
    event = 4
};
enum commandType: uint8_t
{
    reliable = 6,
    unreliable = 7,
    fragmented = 8
};
enum operationCode: uint16_t
{
    move = 21,
    changeLocation = 35,
    auctionSellOrders = 76,
    auctionBuyOrders = 77,
    auctionGetFinishedOrders = 84,
    auctionAverageValues = 90,

    getCharacterEquipment = 137,

};
enum eventCode : uint16_t
{
    playerLeave = 1,
    playerMove = 2,
    healthUpdate = 6,
    activeSpellEffectsUpdate = 10,
    newPlayer = 27,
    harvestableObjectList = 36,
    harvestableObject = 37,
    newSilverObject = 40,
    harvestableChangeState = 43,
    harvestStart = 55,
    harvestFinished = 57,
    equipmentChanged = 85,
    newMob = 117,
    newRandomDungeonExit = 307,

};

enum resourceType : uint8_t
{
    WOOD = 0,
    ROCK = 6,
    FIBER = 11,
    HIDE = 15,
    ORE = 23,
    OTHER = 28,
};

// header size, type size, num of entries
enum dataType : uint16_t
{
    int8 = 98,
    float32 = 102,
    int32 = 105,
    int16 = 107,
    int64 = 108,
    uint8 = 111,
    int8_string = 115,
    int8_list = 120,
    dictionary = 121,

};

enum mapState : uint8_t
{
    miniMap = 0,
    fullscreenMap = 1,
};

#endif