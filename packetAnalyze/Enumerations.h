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
    auctionSellOrders = 76,
    auctionBuyOrders = 77,
    auctionAverageValues = 90,
    getCharacterEquipment = 137,

};
enum eventCode: uint16_t
{
    activeSpellEffectsUpdate = 10,
    harvestableObjectList = 35,
    harvestableObject = 36,
    harvestStart = 55,
    harvestFinished = 57,
    equipmentChanged = 85,
    mobSpawn = 117,
    newRandomDungeonExit = 307,

};
enum resourceType : uint8_t
{
    WOOD = 0,
    ROCK = 6,
    FIBER = 11,
    HIDE = 15,
    ORE = 23,
};

#endif