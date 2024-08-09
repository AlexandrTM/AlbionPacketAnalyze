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

enum mobCategory : uint8_t
{
    resource = 1,
    resourceElemental = 2,
    basicMob = 5,
    magicMob = 6,
    mobBlackZone = 7,
    guard = 8,
    portal = 90,
};

struct MobDescription 
{
    uint8_t _mobTypeID;
    uint8_t _tier;
    uint8_t _category;
    std::string _mobType;

    MobDescription(uint8_t mobTypeID, uint8_t category, uint8_t tier, std::string mobType) :
        _mobTypeID(mobTypeID), _category(category), _tier(tier), _mobType(mobType) {};
};

static std::vector<MobDescription> mobDescriptions
{
    MobDescription(2  , 87 , 1, "MISTS_WISP"),
    MobDescription(2  , 88 , 1, "MISTS_WISP"),
    MobDescription(2  , 90 , 1, "MISTS_WISP"),
    MobDescription(2  , 93 , 1, "MISTS_WISP"),
    MobDescription(2  , 94 , 1, "MISTS_WISP"),

    MobDescription(1  , 0 , 5, "CASTLE_GUARD"),
    MobDescription(3  , 0 , 4, "CITY_GUARD"),
    MobDescription(4  , 0 , 5, "CASTLE_CHAMPION"),
    MobDescription(18 , 0 , 5, "CASTLE_GUARD"),
    MobDescription(21 , 0 , 5, "CASTLE_CHAMPION"),
    MobDescription(23 , 0 , 5, "CASTLE_GUARD"),
    MobDescription(25 , 8 , 4, "CITY_GUARD"),
    MobDescription(56 , 8 , 4, "CITY_GUARD_ARCHER"),
    MobDescription(26 , 0 , 5, "CASTLE_CHAMPION"),

    MobDescription(30 , 0 , 4, "BASIC_MOB"),
    MobDescription(118, 7 , 5, "BASIC_MOB"),
    MobDescription(119, 7 , 5, "BASIC_MOB"),
    MobDescription(123, 7 , 5, "BASIC_MOB"),
    MobDescription(124, 7 , 5, "BASIC_MOB"),
    MobDescription(131, 7 , 5, "BASIC_MOB"),
    MobDescription(187, 0 , 4, "BASIC_MOB"),
    MobDescription(196, 0 , 4, "BASIC_MOB"),
    MobDescription(205, 0 , 4, "BASIC_MOB"),

    MobDescription(130, 4 , 6, "BASIC_MOB"),
    MobDescription(138, 4 , 6, "BASIC_MOB"),
    MobDescription(146, 4 , 6, "BASIC_MOB"),
    MobDescription(178, 4 , 6, "BASIC_MOB"),
    MobDescription(186, 4 , 6, "BASIC_MOB"),
    MobDescription(194, 4 , 6, "BASIC_MOB"),
    MobDescription(210, 4 , 6, "BASIC_MOB"),
    MobDescription(218, 4 , 6, "BASIC_MOB"),
    MobDescription(226, 4 , 6, "BASIC_MOB"),
    MobDescription(234, 4 , 6, "BASIC_MOB"),
    MobDescription(250, 4 , 6, "BASIC_MOB"),

    MobDescription(183, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(197, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(204, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(211, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(225, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(232, 10, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(246, 10, 5, "BASIC_MOB_DUNGEON"),

    MobDescription(11 , 11, 5, "BASIC_MOB_DUNGEON"),
    MobDescription(4  , 11, 5, "BASIC_MOB_DUNGEON"),

    MobDescription(214, 0 , 4, "BASIC_MOB"),
    MobDescription(241, 0 , 4, "BASIC_MOB"),
    MobDescription(120, 7 , 5, "BASIC_MOB_BOSS"),
    MobDescription(121, 7 , 5, "BASIC_MOB_BOSS"),
    MobDescription(125, 7 , 5, "BASIC_MOB_BOSS"),
    MobDescription(126, 0 , 5, "BASIC_MOB_BOSS"),
    MobDescription(128, 7 , 5, "BASIC_MOB_BOSS"),
    MobDescription(132, 7 , 5, "BASIC_MOB_BOSS"),

    MobDescription(141, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(144, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(148, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(161, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(176, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(188, 7 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(201, 7 , 5, "STATIC_DUNGEON_MOB"),

    MobDescription(148, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(149, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(152, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(162, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(177, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(190, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(203, 0 , 5, "STATIC_DUNGEON_MOB"),
    MobDescription(208, 0 , 4, "STATIC_DUNGEON_MOB"),
    MobDescription(209, 0 , 4, "STATIC_DUNGEON_MOB"),
    MobDescription(230, 0 , 4, "STATIC_DUNGEON_MOB"),
    MobDescription(251, 0 , 4, "STATIC_DUNGEON_MOB"),

    MobDescription(102, 0 , 1, "HIDE"),
    MobDescription(103, 0 , 3, "HIDE"),
    MobDescription(105, 0 , 3, "HIDE"),
    MobDescription(106, 0 , 4, "HIDE"),
    MobDescription(107, 1 , 5, "HIDE"),
    MobDescription(108, 1 , 6, "HIDE"),
    MobDescription(119, 1 , 1, "HIDE"),
    MobDescription(120, 1 , 2, "HIDE"),
    MobDescription(121, 1 , 3, "HIDE"),
    MobDescription(122, 1 , 4, "HIDE"),
    MobDescription(123, 1 , 5, "HIDE"),
    MobDescription(124, 1 , 6, "HIDE"),
    MobDescription(130, 0 , 1, "HIDE"),
    MobDescription(132, 0 , 3, "HIDE"),
    MobDescription(133, 0 , 4, "HIDE"),
    MobDescription(145, 1 , 1, "HIDE"),
    MobDescription(134, 0 , 5, "HIDE"),
    MobDescription(219, 0 , 3, "HIDE"),
    MobDescription(220, 1 , 5, "HIDE"),
    MobDescription(237, 1 , 4, "HIDE"),
    MobDescription(239, 1 , 6, "HIDE"),

    MobDescription(41 , 2 , 4, "FIBER"),
    MobDescription(43 , 2 , 6, "FIBER"),
    MobDescription(48 , 2 , 6, "FIBER_BOSS"),
    MobDescription(49 , 2 , 7, "FIBER_BOSS"),
    MobDescription(216, 1 , 3, "FIBER"),
    MobDescription(217, 1 , 5, "FIBER"),

    MobDescription(2  , 2 , 5, "WOOD"),
    MobDescription(227, 0 , 3, "WOOD"),
    MobDescription(228, 0 , 3, "WOOD"),
    MobDescription(229, 0 , 5, "WOOD"),
    MobDescription(252, 1 , 4, "WOOD"),
    MobDescription(254, 1 , 6, "WOOD"),

    MobDescription(11 , 2 , 4, "ROCK"),
    MobDescription(13 , 2 , 6, "ROCK"),
    MobDescription(16 , 2 , 4, "ROCK_BOSS"),

    MobDescription(223, 1 , 3, "ORE"),
    MobDescription(224, 1 , 5, "ORE"),
    MobDescription(26 , 2 , 4, "ORE"),
    MobDescription(28 , 2 , 6, "ORE"),
    MobDescription(31 , 2 , 4, "ORE_BOSS"),

    MobDescription(205, 2 , 6, "TREASURE"),
    MobDescription(203, 2 , 4, "TREASURE"),
    MobDescription(210, 2 , 6, "TREASURE"),
    MobDescription(215, 0 , 5, "TREASURE"),
    
    MobDescription(85 , 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(86 , 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(88 , 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(89 , 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(91 , 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(92 , 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(94 , 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(95 , 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(100, 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(101, 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(106, 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(118, 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(119, 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(130, 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(131, 3 , 6, "AVALONIAN_ROAD"),
    MobDescription(136, 3 , 4, "AVALONIAN_ROAD"),
    MobDescription(137, 3 , 6, "AVALONIAN_ROAD"),
    
    MobDescription(65 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(66 , 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(68 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(69 , 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(71 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(72 , 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(74 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(75 , 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(77 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(84 , 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(86 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(92 , 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(116, 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(122, 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(131, 15, 4, "AVALONIAN_ROAD_CHEST"),
    MobDescription(135, 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(148, 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(154, 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(163, 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(184, 15, 6, "AVALONIAN_ROAD_CHEST"),
    MobDescription(193, 15, 6, "AVALONIAN_ROAD_CHEST"),
};

inline bool isMobKnown(uint8_t mobTypeID, uint8_t category) {
    auto it = std::find_if(mobDescriptions.begin(), mobDescriptions.end(),
        [mobTypeID, category](const MobDescription& mob) {
            return (mob._mobTypeID == mobTypeID && mob._category == category);
        });
    return it != mobDescriptions.end(); // Return true if found
}

// header size, type size, num of entries
enum dataType : uint16_t
{
    dictionary = 68, // actually some nested type
    int8 = 98,
    float32 = 102,
    int32 = 105,
    int16 = 107,
    int64 = 108,
    uint8 = 111,
    int8_string = 115,
    int8_list = 120,
    listOfType = 121,

};

enum mapState : uint8_t
{
    miniMap = 0,
    fullscreenMap = 1,
};

#endif