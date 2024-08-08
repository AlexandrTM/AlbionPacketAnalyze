#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

enum operationType: uint8_t
{
    not_defined = 0,
    request = 2,
    response = 3,
    event = 4,
    binary = 7, // not sure what it is
};
enum commandType: uint8_t
{
    something = 1,
    reliable = 6,
    unreliable = 7,
    fragmented = 8,
};
enum operationCode : uint16_t
{
    joinLocation = 2,
    move = 21,
    changeLocation = 35,
    auctionSellOrders = 75,
    auctionBuyOrders = 76,
    auctionFinishedOrders = 84,
    auctionMyOwnAuctions = 88,
    auctionAverageValues = 89,

    getCharacterEquipment = 137,
    getClusterMapInfo = 190,
    getGoldPrices = 242,

};
enum eventCode : uint16_t
{
    none = 0,
    playerLeave = 1,
    playerMove = 2,
    healthUpdate = 6,
    activeSpellEffectsUpdate = 10,
    newPlayer = 29,
    harvestableObjectList = 39,
    harvestableObject = 40,
    newSilverObject = 43,
    newBuilding = 45,
    harvestableChangeState = 46,
    mobChangeState = 47,
    harvestStart = 55,
    harvestFinished = 57,
    equipmentChanged = 85,
    newMob = 123,
    newRandomDungeonExit = 307,

};

enum resourceType : uint8_t
{
    WOOD = 0,
    ROCK = 6,
    FIBER = 12,
    HIDE = 17,
    ORE = 23,
    OTHER = 28,
};

enum mopType : uint16_t
{

};

// header size, type size, num of entries
enum dataType : uint16_t
{
    nested = 68, // actually some nested type
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