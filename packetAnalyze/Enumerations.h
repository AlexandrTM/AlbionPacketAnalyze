#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

typedef std::vector<uint8_t> RawNetworkPacket;

struct NetworkPacketInfo
{
    Tins::IPv4Address src_ip;
    Tins::IPv4Address dst_ip;

    uint16_t src_port;
    uint16_t dst_port;

    void to_string() const;
};

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

// header size, type size, num of entries
enum dataType : uint16_t
{
    /*"Unknown": 0,
  "Null": 42,
  "Dictionary": 68,
  "StringArray": 97,
  "Byte": 98,
  "Double": 100,
  "EventData": 101,
  "Float": 102,
  "Integer": 105,
  "Hashtable": 104,
  "Short": 107,
  "Long": 108,
  "IntegerArray": 110,
  "Boolean": 111,
  "OperationResponse": 112,
  "OperationRequest": 113,
  "String": 115,
  "ByteArray": 120,
  "Array": 121,
  "ObjectArray": 122*/

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

enum operationCode : uint16_t
{
    Join = 2, // join location
    Move = 21,
    ChangeCluster = 35, // change location
    AuctionSellOrders = 75,
    AuctionBuyOrders = 76,
    AuctionFinishedOrders = 84,
    AuctionGetMyOpenAuctions = 88,
    AuctionGetItemAverageStats = 89,

    GetCharacterEquipment = 136,
    GetClusterMapInfo = 191,
    GoldMarketGetAverageInfo = 243,

};
enum eventCode : uint16_t
{
    none = 0,
    playerLeave = 1,
    playerMove = 3,
    healthUpdate = 6,
    activeSpellEffectsUpdate = 11,
    newPlayer = 29,
    NewSimpleHarvestableObjectList = 39,
    NewHarvestableObject = 40,
    newSilverObject = 44,
    newBuilding = 45,
    harvestableChangeState = 46,
    mobChangeState = 47,
    craftBuildingInfo = 49,
    harvestStart = 59,
    harvestFinished = 61,
    craftItemFinished = 71,
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
    OTHER = 30,
    // 34 wood
    // 43 fiber
    // 44 fiber
    // 53 ore
};

enum itemId : uint32_t
{
    T1_LOG = 907,
    T1_ROCK = 935,
    T2_ORE = 958,
    T1_HIDE = 985,
    T2_FIBER = 1013,
    T2_PLANK = 1040,
    T2_STONEBLOCK = 1067,
    T2_METALBAR = 1074,
    T2_LEATHER = 1101,
    T2_CLOTH = 1128,
    T8_4_CLOTH = 1154,
};

enum locationId : uint32_t
{
    Thetford = 0007,
    Fort_Sterling = 4002,
    Lymhurst = 1002,
    Bridgewatch = 2004,
    Martlock = 3008,
    Caerleon = 3005,
    Black_market = 3003,
    Fort_Sterling_portal = 4301,
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
    uint8_t _typeID;
    uint8_t _tier;
    uint8_t _category;
    std::string _textType;

    MobDescription(uint8_t typeID, uint8_t category, uint8_t tier, std::string mobType) :
        _typeID(typeID), _category(category), _tier(tier), _textType(mobType) {};
    MobDescription() :
        _typeID(0), _category(0), _tier(0), _textType("EMPTY_MOB") {};
};

// basic mobs
/*MobDescription(40 , 0  , 0, "PLAYER_DUEL"),
MobDescription(122, 0  , 0, "PLAYER_DUEL"),
MobDescription(183, 0  , 0, "PLAYER_DUEL"),

MobDescription(2  , 87 , 1, "MISTS_WISP_COMMON"),
MobDescription(2  , 88 , 1, "MISTS_WISP_UNCOMMON"),
MobDescription(2  , 89 , 1, "MISTS_WISP_RARE"),
MobDescription(2  , 90 , 1, "MISTS_WISP_EPIC"),
MobDescription(2  , 91 , 1, "MISTS_WISP_LEGENDARY"),
MobDescription(2  , 92 , 1, "MISTS_WISP_COMMON_GROUP"),
MobDescription(2  , 93 , 1, "MISTS_WISP_UNCOMMON_GROUP"),
MobDescription(2  , 94 , 1, "MISTS_WISP_RARE_GROUP"),
MobDescription(2  , 95 , 1, "MISTS_WISP_EPIC_GROUP"),
MobDescription(2  , 96 , 1, "MISTS_WISP_LEGENDARY_GROUP"),

MobDescription(2  , 119, 1, "HIDEOUT_ORB_RARE"),
MobDescription(2  , 120, 1, "HIDEOUT_ORB_EPIC"),

MobDescription(2  , 102, 1, "VORTEX_RARE"),
MobDescription(2  , 107, 1, "VORTEX_EPIC"),

MobDescription(1  , 8  , 5, "CASTLE_GUARD"),
MobDescription(2  , 8  , 5, "CASTLE_GUARD_LYMHURST"),
MobDescription(3  , 8  , 4, "CITY_GUARD"),
MobDescription(4  , 8  , 5, "CASTLE_CHAMPION"),
MobDescription(5  , 8  , 5, "CASTLE_CHAMPION_LYMHURST"),
MobDescription(8  , 8  , 4, "CITY_GUARD"),
MobDescription(10 , 8  , 4, "CITY_GUARD"),
MobDescription(11 , 8  , 5, "CASTLE_CHAMPION"),
MobDescription(13 , 8  , 5, "CASTLE_GUARD"),
MobDescription(15 , 8  , 4, "CITY_GUARD"),
MobDescription(16 , 8  , 5, "CASTLE_CHAMPION"),
MobDescription(18 , 8  , 5, "CASTLE_GUARD"),
MobDescription(20 , 8  , 5, "CITY_GUARD"),
MobDescription(21 , 8  , 5, "CASTLE_CHAMPION"),
MobDescription(23 , 8  , 5, "CASTLE_GUARD"),
MobDescription(25 , 8  , 4, "CITY_GUARD"),
MobDescription(56 , 8  , 4, "CITY_GUARD_ARCHER"),
MobDescription(26 , 8  , 5, "CASTLE_CHAMPION"),
MobDescription(248, 7  , 6, "CASTLE_CHAMPION_CARLEON"),
MobDescription(250, 7  , 6, "CASTLE_GUARD_CARLEON"),
MobDescription(254, 7  , 6, "CASTLE_GUARD_CARLEON"),
MobDescription(246, 7  , 6, "CAERLEON_CITY_GUARD"),

MobDescription(2  , 135, 2, "TEST_MANNEQUIN"),
MobDescription(2  , 136, 3, "TEST_MANNEQUIN"),

MobDescription(187, 5  , 4, "BASIC_MOB"),
MobDescription(196, 5  , 4, "BASIC_MOB"),
MobDescription(205, 5  , 4, "BASIC_MOB"),
MobDescription(214, 5  , 4, "BASIC_MOB"),
MobDescription(232, 5  , 4, "BASIC_MOB"),
MobDescription(241, 5  , 4, "BASIC_MOB"),

MobDescription(118, 7  , 5, "BASIC_MOB"),
MobDescription(119, 7  , 5, "BASIC_MOB"),
MobDescription(123, 7  , 5, "BASIC_MOB"),
MobDescription(123, 7  , 5, "BASIC_MOB"),
MobDescription(124, 7  , 5, "BASIC_MOB"),
MobDescription(130, 7  , 5, "BASIC_MOB"),
MobDescription(131, 7  , 5, "BASIC_MOB"),

MobDescription(3  , 4  , 6, "BASIC_MOB"),

MobDescription(129, 4  , 5, "BASIC_MOB"),
MobDescription(133, 4  , 5, "BASIC_MOB"),
MobDescription(137, 4  , 5, "BASIC_MOB"),
MobDescription(145, 4  , 5, "BASIC_MOB"),
MobDescription(169, 4  , 5, "BASIC_MOB"),
MobDescription(173, 4  , 5, "BASIC_MOB"),
MobDescription(177, 4  , 5, "BASIC_MOB"),
MobDescription(181, 4  , 5, "BASIC_MOB"),
MobDescription(185, 4  , 5, "BASIC_MOB"),
MobDescription(189, 4  , 5, "BASIC_MOB"),
MobDescription(209, 4  , 5, "BASIC_MOB"),
MobDescription(213, 4  , 5, "BASIC_MOB"),
MobDescription(217, 4  , 5, "BASIC_MOB"),
MobDescription(225, 4  , 5, "BASIC_MOB"),
MobDescription(253, 4  , 5, "BASIC_MOB"),

MobDescription(130, 4  , 6, "BASIC_MOB"),
MobDescription(138, 4  , 6, "BASIC_MOB"),
MobDescription(142, 4  , 6, "BASIC_MOB"),
MobDescription(146, 4  , 6, "BASIC_MOB"),
MobDescription(154, 4  , 6, "BASIC_MOB"),
MobDescription(170, 4  , 6, "BASIC_MOB"),
MobDescription(174, 4  , 6, "BASIC_MOB"),
MobDescription(178, 4  , 6, "BASIC_MOB"),
MobDescription(182, 4  , 6, "BASIC_MOB"),
MobDescription(186, 4  , 6, "BASIC_MOB"),
MobDescription(190, 4  , 6, "BASIC_MOB"),
MobDescription(194, 4  , 6, "BASIC_MOB"),
MobDescription(202, 4  , 6, "BASIC_MOB"),
MobDescription(210, 4  , 6, "BASIC_MOB"),
MobDescription(214, 4  , 6, "BASIC_MOB"),
MobDescription(218, 4  , 6, "BASIC_MOB"),
MobDescription(222, 4  , 6, "BASIC_MOB"),
MobDescription(226, 4  , 6, "BASIC_MOB"),
MobDescription(230, 4  , 6, "BASIC_MOB"),
MobDescription(234, 4  , 6, "BASIC_MOB"),
MobDescription(242, 4  , 6, "BASIC_MOB_BOSS"),
MobDescription(250, 4  , 6, "BASIC_MOB"),
MobDescription(254, 4  , 6, "BASIC_MOB"),

MobDescription(126, 4  , 8, "BASIC_MOB"),
MobDescription(132, 4  , 8, "BASIC_MOB"),
MobDescription(136, 4  , 8, "BASIC_MOB"),
MobDescription(140, 4  , 8, "BASIC_MOB"),
MobDescription(144, 4  , 8, "BASIC_MOB"),
MobDescription(148, 4  , 8, "BASIC_MOB"),
MobDescription(156, 4  , 8, "BASIC_MOB_BOSS"),
MobDescription(160, 4  , 8, "BASIC_MOB"),
MobDescription(172, 4  , 8, "BASIC_MOB"),
MobDescription(180, 4  , 8, "BASIC_MOB"),
MobDescription(184, 4  , 8, "BASIC_MOB"),
MobDescription(188, 4  , 8, "BASIC_MOB"),
MobDescription(192, 4  , 8, "BASIC_MOB"),
MobDescription(196, 4  , 8, "BASIC_MOB"),
MobDescription(212, 4  , 8, "BASIC_MOB"),
MobDescription(216, 4  , 8, "BASIC_MOB"),
MobDescription(220, 4  , 8, "BASIC_MOB"),
MobDescription(224, 4  , 8, "BASIC_MOB"),
MobDescription(228, 4  , 8, "BASIC_MOB"),
MobDescription(236, 4  , 8, "BASIC_MOB"),
MobDescription(252, 4  , 8, "BASIC_MOB"),

MobDescription(5  , 7  , 6, "BASIC_MOB_BOSS"),

MobDescription(120, 7  , 5, "BASIC_MOB"),
MobDescription(121, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(122, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(125, 7  , 5, "BASIC_MOB"),
MobDescription(126, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(127, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(128, 7  , 5, "BASIC_MOB_MINI_BOSS"),
MobDescription(129, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(132, 7  , 5, "BASIC_MOB"),
MobDescription(133, 7  , 5, "BASIC_MOB_BOSS"),
MobDescription(217, 3  , 6, "BASIC_MOB_BOSS"),

MobDescription(6  , 6  , 7, "BASIC_MOB"),
MobDescription(15 , 6  , 7, "BASIC_MOB"),
MobDescription(24 , 6  , 7, "BASIC_MOB"),
MobDescription(33 , 6  , 7, "BASIC_MOB"),
MobDescription(42 , 6  , 7, "BASIC_MOB"),
MobDescription(51 , 6  , 7, "BASIC_MOB"),
MobDescription(69 , 6  , 7, "BASIC_MOB_BOSS"),
MobDescription(78 , 6  , 7, "BASIC_MOB_BOSS"),
MobDescription(3  , 6  , 4, "BASIC_MOB_BOSS"),
MobDescription(12 , 6  , 4, "BASIC_MOB_BOSS"),
MobDescription(66 , 6  , 4, "BASIC_MOB_BOSS"),

MobDescription(190, 5  , 7, "BASIC_MOB"),
MobDescription(194, 5  , 7, "BASIC_MOB"),
MobDescription(199, 5  , 7, "BASIC_MOB"),
MobDescription(203, 5  , 7, "BASIC_MOB"),
MobDescription(208, 5  , 7, "BASIC_MOB"),
MobDescription(212, 5  , 7, "BASIC_MOB"),
MobDescription(217, 5  , 7, "BASIC_MOB"),
MobDescription(221, 5  , 7, "BASIC_MOB"),
MobDescription(226, 5  , 7, "BASIC_MOB"),
MobDescription(235, 5  , 7, "BASIC_MOB"),
MobDescription(244, 5  , 7, "BASIC_MOB"),
MobDescription(248, 5  , 7, "BASIC_MOB"),
MobDescription(253, 5  , 7, "BASIC_MOB"),

MobDescription(21 , 6  , 4, "BASIC_MOB"),
MobDescription(48 , 6  , 4, "BASIC_MOB"),

MobDescription(23 , 6  , 6, "BASIC_MOB"),
MobDescription(27 , 6  , 6, "BASIC_MOB"),
MobDescription(32 , 6  , 6, "BASIC_MOB"),
MobDescription(36 , 6  , 6, "BASIC_MOB"),
MobDescription(50 , 6  , 6, "BASIC_MOB"),

MobDescription(29 , 7  , 6, "BASIC_MOB"),

MobDescription(30 , 6  , 4, "BASIC_MOB"),
MobDescription(39 , 6  , 4, "BASIC_MOB_BOSS"),
MobDescription(60 , 6  , 7, "BASIC_MOB_BOSS"),
MobDescription(232, 1  , 4, "BASIC_MOB"),
MobDescription(223, 5  , 4, "BASIC_MOB"),
MobDescription(250, 5  , 4, "BASIC_MOB"),
MobDescription(193, 3  , 3, "BASIC_MOB"),
MobDescription(2  , 5  , 6, "BASIC_MOB_BOSS"),
MobDescription(15 , 5  , 6, "BASIC_MOB"),
MobDescription(169, 5  , 6, "BASIC_MOB"),
MobDescription(189, 5  , 6, "BASIC_MOB"),
MobDescription(193, 5  , 6, "BASIC_MOB"),
MobDescription(198, 5  , 6, "BASIC_MOB"),
MobDescription(202, 5  , 6, "BASIC_MOB"),
MobDescription(207, 5  , 6, "BASIC_MOB"),
MobDescription(211, 5  , 6, "BASIC_MOB"),
MobDescription(216, 5  , 6, "BASIC_MOB"),
MobDescription(220, 5  , 6, "BASIC_MOB"),
MobDescription(225, 5  , 6, "BASIC_MOB"),
MobDescription(243, 5  , 6, "BASIC_MOB"),
MobDescription(247, 5  , 6, "BASIC_MOB"),
MobDescription(252, 5  , 6, "BASIC_MOB"),

MobDescription(2  , 14 , 4, "BASIC_MOB_TRACK"),
MobDescription(2  , 15 , 4, "BASIC_MOB_BOSS_TRACK"),
MobDescription(2  , 26 , 4, "BASIC_MOB_BOSS_TRACK"),
MobDescription(2  , 63 , 4, "BASIC_MOB_BOSS_TRACK"),
MobDescription(2  , 65 , 4, "BASIC_MOB_BOSS_TRACK"),

MobDescription(9  , 4  , 8, "BASIC_MOB_BOSS"),
MobDescription(152, 3  , 8, "BASIC_MOB"),
MobDescription(156, 3  , 8, "BASIC_MOB"),
MobDescription(161, 3  , 8, "BASIC_MOB"),
MobDescription(165, 3  , 8, "BASIC_MOB"),
MobDescription(170, 3  , 8, "BASIC_MOB"),
MobDescription(174, 3  , 8, "BASIC_MOB"),
MobDescription(179, 3  , 8, "BASIC_MOB_BOSS"),
MobDescription(197, 3  , 8, "BASIC_MOB"),
MobDescription(201, 3  , 8, "BASIC_MOB"),
MobDescription(206, 3  , 8, "BASIC_MOB"),
MobDescription(224, 3  , 8, "BASIC_MOB"),
MobDescription(228, 3  , 8, "BASIC_MOB"),
MobDescription(233, 3  , 8, "BASIC_MOB_BOSS"),
MobDescription(242, 3  , 8, "BASIC_MOB_BOSS"),
MobDescription(131, 4  , 7, "BASIC_MOB"),
MobDescription(135, 4  , 7, "BASIC_MOB"),
MobDescription(139, 4  , 7, "BASIC_MOB"),
MobDescription(143, 4  , 7, "BASIC_MOB"),
MobDescription(147, 4  , 7, "BASIC_MOB"),
MobDescription(151, 4  , 7, "BASIC_MOB_BOSS"),
MobDescription(171, 4  , 7, "BASIC_MOB"),
MobDescription(175, 4  , 7, "BASIC_MOB"),
MobDescription(179, 4  , 7, "BASIC_MOB"),
MobDescription(183, 4  , 7, "BASIC_MOB"),
MobDescription(187, 4  , 7, "BASIC_MOB"),
MobDescription(191, 4  , 7, "BASIC_MOB"),
MobDescription(211, 4  , 7, "BASIC_MOB"),
MobDescription(215, 4  , 7, "BASIC_MOB"),
MobDescription(219, 4  , 7, "BASIC_MOB"),
MobDescription(223, 4  , 7, "BASIC_MOB"),
MobDescription(227, 4  , 7, "BASIC_MOB"),
MobDescription(255, 4  , 7, "BASIC_MOB_BOSS"),
// static dungeon mobs
MobDescription(239, 4  , 7, "BASIC_MOB_BOSS"),
MobDescription(4  , 4  , 7, "BASIC_MOB_BOSS"),
MobDescription(8  , 4  , 7, "BASIC_MOB_BOSS"),
//MobDescription(4  , 5  , 8, "BASIC_MOB_BOSS"),
MobDescription(18 , 5  , 8, "BASIC_MOB"),
MobDescription(20 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(23 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(41 , 3  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(46 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(47 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(70 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(71 , 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(100, 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(128, 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(166, 5  , 8, "STATIC_DUNGEON_MOB"),
MobDescription(152, 5  , 8, "STATIC_DUNGEON_MOB"),
//MobDescription(204, 4  , 8, "BASIC_MOB_BOSS"),
MobDescription(172, 5  , 8, "BASIC_MOB"),

MobDescription(37 , 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(228, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(231, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(233, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(236, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(237, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(239, 3  , 8, "BASIC_MOB_WORLD_BOSS"),
MobDescription(242, 3  , 8, "BASIC_MOB_WORLD_BOSS"),

MobDescription(150, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(154, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(159, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(163, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(168, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(172, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(181, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(190, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(195, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(199, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(222, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(226, 3  , 6, "BASIC_MOB_MISTS"),
MobDescription(231, 3  , 6, "BASIC_MOB_MISTS"),

MobDescription(151, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(155, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(160, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(164, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(169, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(173, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(178, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(182, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(196, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(200, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(205, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(209, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(223, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(227, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(232, 3  , 7, "BASIC_MOB_MISTS"),
MobDescription(236, 3  , 7, "BASIC_MOB_MISTS"),

MobDescription(183, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(197, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(204, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(211, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(225, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(232, 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(246, 10 , 5, "BASIC_MOB_DUNGEON"),

MobDescription(50 , 10 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(72 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(76 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(80 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(84 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(88 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(92 , 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(100, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(104, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(108, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(112, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(116, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(120, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(124, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(128, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(132, 10 , 6, "BASIC_MOB_DUNGEON_BOSS"),
MobDescription(136, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(140, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(144, 10 , 6, "BASIC_MOB_DUNGEON_BOSS"),
MobDescription(148, 10 , 6, "BASIC_MOB_DUNGEON_BOSS"),
MobDescription(152, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(160, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(164, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(168, 10 , 6, "BASIC_MOB_DUNGEON"),
MobDescription(237, 12 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(242, 12 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(11 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(16 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(21 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(26 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(36 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(49 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(59 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(64 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(74 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(79 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(89 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(94 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(99 , 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(104, 13 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(119, 13 , 5, "BASIC_MOB_DUNGEON_BOSS"),

MobDescription(77 , 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(81 , 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(85 , 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(89 , 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(109, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(121, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(117, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(113, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(137, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(153, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(141, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(105, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(161, 10 , 7, "BASIC_MOB_DUNGEON"),
MobDescription(165, 10 , 7, "BASIC_MOB_DUNGEON"),

MobDescription(11 , 11 , 5, "BASIC_MOB_DUNGEON"),
MobDescription(4  , 11 , 5, "BASIC_MOB_DUNGEON"),

MobDescription(19 , 7  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(20 , 7  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(141, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(159, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(168, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(169, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(208, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(209, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(229, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(230, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(251, 6  , 4, "STATIC_DUNGEON_MOB"),
MobDescription(252, 6  , 4, "STATIC_DUNGEON_MOB"),

MobDescription(6  , 7  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(7  , 7  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(9  , 7  , 7, "STATIC_DUNGEON_MOB_BOSS"),
MobDescription(42 , 7  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(144, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(174, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(176, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(195, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(196, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(201, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(202, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(217, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(218, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(221, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(241, 6  , 7, "STATIC_DUNGEON_MOB"),
MobDescription(242, 6  , 7, "STATIC_DUNGEON_MOB"),

MobDescription(43 , 7  , 7, "STATIC_DUNGEON_MOB"),

MobDescription(141, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(144, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(148, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(149, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(152, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(161, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(162, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(176, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(177, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(188, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(190, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(201, 7  , 5, "STATIC_DUNGEON_MOB"),
MobDescription(203, 7  , 5, "STATIC_DUNGEON_MOB"),

MobDescription(28 , 8  , 4, "CASTLE_CLAIM"), // tier not defined
MobDescription(29 , 8  , 4, "CASTLE_OUTPOST"),
MobDescription(30 , 8  , 4, "CASTLE_OUTPOST"),

MobDescription(49, 1, 5, "CASTLE_CLAIM_MAGE"),
MobDescription(50, 1, 5, "CASTLE_CLAIM_MAGE"),
MobDescription(38, 8, 4, "CASTLE_CLAIM"),
MobDescription(39, 8, 5, "CASTLE_CLAIM"),
MobDescription(40, 8, 6, "CASTLE_CLAIM"),
MobDescription(41, 8, 7, "CASTLE_CLAIM"),
MobDescription(42, 8, 8, "CASTLE_CLAIM"),
MobDescription(43, 8, 4, "CASTLE_CLAIM"),
MobDescription(44, 8, 5, "CASTLE_CLAIM"),
MobDescription(45, 8, 6, "CASTLE_CLAIM"),
MobDescription(46, 8, 7, "CASTLE_CLAIM"),
MobDescription(47, 8, 8, "CASTLE_CLAIM"),
MobDescription(48, 8, 4, "CASTLE_CLAIM"),
MobDescription(49, 8, 5, "CASTLE_CLAIM"),
MobDescription(50, 8, 6, "CASTLE_CLAIM"),
MobDescription(51, 8, 7, "CASTLE_CLAIM"),
MobDescription(52, 8, 8, "CASTLE_CLAIM"),*/

// resource mobs
/*MobDescription(148, 1, 5, "ORE_5_4"),
MobDescription(201, 1  , 4, "HIDE_4_4"),
MobDescription(205, 1  , 8, "HIDE_8_4"),
MobDescription(266, 1  , 5, "HIDE_5_4"), // strange that bigger than 255

MobDescription(157, 1  , 4, "ORE_7_4"),

MobDescription(174, 2  , 6, "HIDE_ASPECT_MAMMOTH"),

MobDescription(41 , 2  , 4, "FIBER"),
MobDescription(43 , 2  , 6, "FIBER"),
MobDescription(46 , 2  , 4, "FIBER_ELEMENTAL"),
MobDescription(47 , 2  , 5, "FIBER_ELEMENTAL"),
MobDescription(48 , 2  , 6, "FIBER_ELEMENTAL"),
MobDescription(49 , 2  , 7, "FIBER_ELEMENTAL"),
MobDescription(54 , 2  , 7, "FIBER_ELEMENTAL_ELITE"),
MobDescription(55 , 2  , 8, "FIBER_ELEMENTAL_ELITE"),
MobDescription(53 , 2  , 6, "FIBER_ELEMENTAL_ANCIENT"),
MobDescription(75 , 2  , 4, "FIBER"),
MobDescription(76 , 2  , 5, "FIBER"),
MobDescription(77 , 2  , 6, "FIBER"),
MobDescription(100, 2  , 5, "FIBER"),
MobDescription(101, 2  , 6, "FIBER"),
MobDescription(102, 2  , 7, "FIBER"),
MobDescription(123, 2  , 4, "FIBER_MISTS"),
MobDescription(124, 2  , 5, "FIBER_MISTS"),
MobDescription(126, 2  , 7, "FIBER"),
MobDescription(127, 2  , 8, "FIBER"),
MobDescription(216, 1  , 3, "FIBER"),
MobDescription(217, 1  , 5, "FIBER"),
MobDescription(218, 1  , 7, "FIBER"),

MobDescription(153, 2  , 6, "FIBER_ASPECT_ENORMOUS_DRYAD"),
MobDescription(166, 2  , 6, "FIBER_ASPECT_TITAN"),
MobDescription(168, 2  , 8, "FIBER_ASPECT_TITAN"),*/

// wood
/*MobDescription(1  , 2  , 4, "WOOD_ELEMENTAL"),
MobDescription(2  , 2  , 5, "WOOD_ELEMENTAL"),
MobDescription(3  , 2  , 6, "WOOD_ELEMENTAL"),
MobDescription(4  , 2  , 7, "WOOD_ELEMENTAL"),
MobDescription(10 , 2  , 8, "WOOD_ELEMENTAL_ELITE"),
MobDescription(56 , 2  , 3, "WOOD_MISTS"),
MobDescription(57 , 2  , 4, "WOOD"),
MobDescription(58 , 2  , 5, "WOOD"),
MobDescription(59 , 2  , 6, "WOOD"),
MobDescription(82 , 2  , 5, "WOOD"),
MobDescription(83 , 2  , 6, "WOOD"),
MobDescription(104, 2  , 3, "WOOD_MISTS"),
MobDescription(105, 2  , 3, "WOOD_MISTS"),
MobDescription(106, 2  , 5, "WOOD_MISTS"),
MobDescription(107, 2  , 6, "WOOD"),
MobDescription(108, 2  , 7, "WOOD"),
MobDescription(109, 2  , 8, "WOOD"),
MobDescription(131, 2  , 6, "WOOD_ASPECT_ANCIENT_ENT"),
MobDescription(177, 2  , 6, "WOOD_ASPECT_ANCIENT_ENT"),
MobDescription(178, 2  , 6, "WOOD_ASPECT_ANCIENT_ENT_ELITE"),
MobDescription(181, 2  , 6, "WOOD_ASPECT"),
MobDescription(227, 1  , 3, "WOOD"),
MobDescription(228, 1  , 3, "WOOD"),
MobDescription(229, 1  , 5, "WOOD"),
MobDescription(230, 1  , 6, "WOOD"),
MobDescription(231, 1  , 7, "WOOD"),
MobDescription(252, 1  , 4, "WOOD"),
MobDescription(254, 1  , 6, "WOOD"),*/

// hide
/*MobDescription(74 , 1  , 1, "HIDE"),
MobDescription(75 , 1  , 2, "HIDE_MISTS"),
MobDescription(76 , 1  , 3, "HIDE_MISTS"),
MobDescription(77 , 1  , 4, "HIDE"),
MobDescription(78 , 1  , 5, "HIDE"),
MobDescription(79 , 1  , 6, "HIDE"),
MobDescription(80 , 1  , 7, "HIDE_MISTS"),
MobDescription(81 , 1  , 8, "HIDE_MISTS"),
MobDescription(102, 1  , 1, "HIDE"),
MobDescription(103, 1  , 3, "HIDE"),
MobDescription(105, 1  , 3, "HIDE"),
MobDescription(106, 1  , 4, "HIDE"),
MobDescription(107, 1  , 5, "HIDE"),
MobDescription(108, 1  , 6, "HIDE"),
MobDescription(109, 1  , 6, "HIDE"),
MobDescription(110, 1  , 7, "HIDE"),
MobDescription(112, 1  , 8, "HIDE"),
MobDescription(119, 1  , 1, "HIDE"),
MobDescription(120, 1  , 2, "HIDE"),
MobDescription(121, 1  , 3, "HIDE"),
MobDescription(122, 1  , 4, "HIDE"),
MobDescription(123, 1  , 5, "HIDE"),
MobDescription(124, 1  , 6, "HIDE"),
MobDescription(130, 1  , 1, "HIDE"),
MobDescription(131, 1  , 2, "HIDE"),
MobDescription(132, 1  , 3, "HIDE"),
MobDescription(133, 1  , 4, "HIDE"),
MobDescription(134, 1  , 5, "HIDE"),
MobDescription(135, 1  , 6, "HIDE"),
MobDescription(136, 1  , 7, "HIDE"),
MobDescription(145, 1  , 1, "HIDE"),
MobDescription(146, 1  , 1, "HIDE"),
MobDescription(219, 1  , 3, "HIDE"),
MobDescription(220, 1  , 5, "HIDE"),
MobDescription(221, 1  , 7, "HIDE"),
MobDescription(237, 1  , 4, "HIDE"),
MobDescription(238, 1  , 5, "HIDE"),
MobDescription(239, 1  , 6, "HIDE"),
MobDescription(242, 1  , 4, "HIDE_ELEMENTAL"),
MobDescription(243, 1  , 5, "HIDE_ELEMENTAL"),
MobDescription(244, 1  , 6, "HIDE_ELEMENTAL"),
MobDescription(251, 1  , 8, "HIDE_ASPECT"),*/

// avalonian road mobs
/*MobDescription(85 , 3  , 4, "AVALONIAN_ROAD"),
MobDescription(86 , 3  , 6, "AVALONIAN_ROAD"),
MobDescription(87 , 3  , 8, "AVALONIAN_ROAD"),
MobDescription(88 , 3  , 4, "AVALONIAN_ROAD"),
MobDescription(89 , 3  , 6, "AVALONIAN_ROAD"),
MobDescription(90 , 3  , 8, "AVALONIAN_ROAD"),
MobDescription(91 , 3  , 4, "AVALONIAN_ROAD"),
MobDescription(92 , 3  , 6, "AVALONIAN_ROAD"),
MobDescription(93 , 3  , 8, "AVALONIAN_ROAD"),
MobDescription(94 , 3  , 4, "AVALONIAN_ROAD"),
MobDescription(95 , 3  , 6, "AVALONIAN_ROAD"),
MobDescription(96 , 3  , 8, "AVALONIAN_ROAD"),
MobDescription(100, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(102, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(101, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(103, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(104, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(105, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(106, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(107, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(110, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(111, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(118, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(119, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(120, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(121, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(122, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(123, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(125, 3  , 4, "AVALONIAN_ROAD_BOSS"),
MobDescription(130, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(131, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(132, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(133, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(134, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(135, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(136, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(137, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(138, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(139, 3  , 4, "AVALONIAN_ROAD"),
MobDescription(140, 3  , 6, "AVALONIAN_ROAD"),
MobDescription(141, 3  , 8, "AVALONIAN_ROAD"),
MobDescription(144, 3  , 8, "AVALONIAN_ROAD"),

MobDescription(65 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(66 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(67 , 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(68 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(69 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(70 , 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(71 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(72 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(73 , 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(74 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(75 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(76 , 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(77 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(78 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(81 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(83 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(84 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(85 , 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(86 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(87 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(90 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(92 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(93 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(95 , 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(96 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(99 , 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(116, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(117, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(122, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(129, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(131, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(132, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(134, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(135, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(137, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(138, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(139, 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(141, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(142, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(144, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(145, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(148, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(149, 15 , 8, "AVALONIAN_ROAD_CHEST"),
MobDescription(150, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(151, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(154, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(159, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(160, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(163, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(166, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(169, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(183, 15 , 4, "AVALONIAN_ROAD_CHEST"),
MobDescription(184, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(193, 15 , 6, "AVALONIAN_ROAD_CHEST"),
MobDescription(198, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(199, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(200, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(201, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(202, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(203, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(204, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(205, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(206, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(210, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(212, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(215, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(221, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(225, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(226, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(229, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(230, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(231, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(241, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(242, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(247, 15 , 8, "AVALONIAN_ROAD_CHEST_ELITE"),
MobDescription(248, 15 , 6, "AVALONIAN_ROAD_CHEST_ELITE"),*/

static std::vector<MobDescription> mobDescriptions
{
    //MobDescription(59 , 1  , 6, "CRYTSAL_SPIDERLING"),
    MobDescription(63 , 1  , 5, "CRYTSAL_SPIDER"), // health: 24825
    MobDescription(64 , 1  , 6, "CRYTSAL_SPIDER"),
    MobDescription(65 , 1  , 7, "CRYTSAL_SPIDER"),
    MobDescription(66 , 1  , 8, "CRYTSAL_SPIDER"),
    MobDescription(67 , 1  , 5, "CRYTSAL_COBRA"), // health: 24825
    MobDescription(68 , 1  , 6, "CRYTSAL_COBRA"),
    MobDescription(69 , 1  , 7, "CRYTSAL_COBRA"),
    MobDescription(70 , 1  , 8, "CRYTSAL_COBRA"), // health: 43588
    MobDescription(71 , 1  , 5, "CRYTSAL_BETTLE"),
    MobDescription(72 , 1  , 6, "CRYTSAL_BETTLE"),
    MobDescription(73 , 1  , 7, "CRYTSAL_BETTLE"), // health: 34803
    MobDescription(74 , 1  , 8, "CRYTSAL_BETTLE"),


    MobDescription(4  , 2  , 4, "HIDE_MISTS"), // health: 1154
    MobDescription(5  , 2  , 5, "HIDE_MISTS"), // health: 1094
    MobDescription(6  , 2  , 6, "HIDE"), // health: 1180
    MobDescription(82 , 1  , 1, "HIDE_MISTS_NON_LETHAL"), // health: 20
    MobDescription(83 , 1  , 2, "HIDE_MISTS_NON_LETHAL"), // health: 515
    MobDescription(84 , 1  , 3, "HIDE_MISTS_NON_LETHAL"), // health: 685
    MobDescription(85 , 1  , 4, "HIDE_MISTS_NON_LETHAL"), // health: 1143
    MobDescription(86 , 1  , 5, "HIDE_MISTS_NON_LETHAL"), // health: 1094
    MobDescription(110, 1  , 1, "HIDE"), // health: 20
    MobDescription(111, 1  , 2, "HIDE"),
    MobDescription(113, 1  , 3, "HIDE"),
    MobDescription(115, 1  , 4, "HIDE"),
    MobDescription(116, 1  , 4, "HIDE"), // health: 1323
    MobDescription(117, 1  , 5, "HIDE"),
    MobDescription(119, 1  , 6, "HIDE"), // health: 1180
    MobDescription(121, 1  , 6, "HIDE"), // health: 1401,
    MobDescription(123, 1  , 7, "HIDE"), // health: 1052
    MobDescription(134, 1  , 1, "HIDE"), // health: 20
    MobDescription(135, 1  , 2, "HIDE"), // health: 400
    MobDescription(136, 1  , 3, "HIDE"), // health: 856
    MobDescription(138, 1  , 4, "HIDE"), // health: 1203
    MobDescription(139, 1  , 4, "HIDE"), // health: 1203
    MobDescription(140, 1  , 5, "HIDE"), // health: 1162
    MobDescription(142, 1  , 6, "HIDE"), // health: 1623
    MobDescription(168, 1  , 1, "HIDE"),
    MobDescription(169, 1  , 1, "HIDE"), // health: 20
    MobDescription(242, 1  , 3, "HIDE_MISTS"), // health: 822


    MobDescription(66 , 2  , 6, "FIBER"), // health: 1475
    MobDescription(70 , 2  , 5, "FIBER_OLD"), // health: 4257
    MobDescription(71 , 2  , 6, "FIBER_OLD"), // health: 4592
    MobDescription(98 , 2  , 4, "FIBER_MISTS_NON_LETHAL"), // health: 1203
    MobDescription(99 , 2  , 5, "FIBER_MISTS_NON_LETHAL"), // health: 1367
    MobDescription(124, 2  , 6, "FIBER_MISTS"), // health: 1770
    MobDescription(133, 2  , 4, "FIBER_MISTS"), // health: 1444
    MobDescription(146, 2  , 4, "FIBER_MISTS"), // health: 1444
    MobDescription(147, 2  , 5, "FIBER_MISTS"), // health: 1640
    MobDescription(239, 1  , 3, "FIBER"), // health: 856
    MobDescription(240, 1  , 5, "FIBER"), // health: 1367


    MobDescription(21 , 2  , 6, "WOOD"), // health: 1770
    MobDescription(25 , 2  , 5, "WOOD_OLD"), // health: 5108
    MobDescription(79 , 2  , 3, "WOOD_MISTS_NON_LETHAL"), // health: 1443
    MobDescription(80 , 2  , 4, "WOOD_MISTS_NON_LETHAL"), // health: 1443
    MobDescription(81 , 2  , 5, "WOOD_MISTS_NON_LETHAL"), // health: 1641
    MobDescription(105, 2  , 5, "WOOD_MISTS"), // health: 1969
    MobDescription(106, 2  , 6, "WOOD_MISTS"), // health: 2124
    MobDescription(127, 2  , 3, "WOOD_MISTS"), // health: 1732
    MobDescription(128, 2  , 4, "WOOD_MISTS"), // health: 1732
    MobDescription(129, 2  , 5, "WOOD_MISTS"), // health: 1969
    MobDescription(251, 1  , 3, "WOOD"), // health: 1028
    MobDescription(252, 1  , 5, "WOOD"), // health: 1641
    MobDescription(253, 1  , 5, "WOOD"), // health: 1641


    /*MobDescription(11 , 2  , 4, "ROCK"),
    MobDescription(13 , 2  , 6, "ROCK"),
    MobDescription(16 , 2  , 4, "ROCK_ELEMENTAL"),
    MobDescription(17 , 2  , 5, "ROCK_ELEMENTAL"),
    MobDescription(18 , 2  , 6, "ROCK_ELEMENTAL"),
    MobDescription(19 , 2  , 7, "ROCK_ELEMENTAL"),
    MobDescription(20 , 2  , 8, "ROCK_ELEMENTAL"),
    MobDescription(24 , 2  , 7, "ROCK_ELEMENTAL_ELITE"),
    MobDescription(25 , 2  , 8, "ROCK_ELEMENTAL_ELITE"),
    MobDescription(62 , 2  , 3, "ROCK_MISTS"),
    MobDescription(63 , 2  , 4, "ROCK_MISTS"),
    MobDescription(64 , 2  , 5, "ROCK_MISTS"),
    MobDescription(65 , 2  , 6, "ROCK_MISTS"),
    MobDescription(88 , 2  , 5, "ROCK"),
    MobDescription(110, 2  , 3, "ROCK_MISTS"),
    MobDescription(111, 2  , 4, "ROCK_MISTS"),
    MobDescription(112, 2  , 5, "ROCK_MISTS"),
    MobDescription(113, 2  , 6, "ROCK"),
    MobDescription(114, 2  , 7, "ROCK"),
    MobDescription(121, 2  , 8, "ROCK"),
    MobDescription(125, 2  , 6, "ROCK"),
    MobDescription(184, 2  , 6, "ROCK_ASPECT"),
    MobDescription(233, 1  , 3, "ROCK"),
    MobDescription(234, 1  , 5, "ROCK"),
    MobDescription(235, 1  , 5, "ROCK"),*/
    
    MobDescription(0  , 2  , 3, "ROCK"), // health: 856
    MobDescription(1  , 2  , 5, "ROCK"), // health: 1367
    MobDescription(36 , 2  , 6, "ROCK"), // health: 1475
    MobDescription(85 , 2  , 3, "ROCK_MISTS_NON_LETHAL"), // health: 1203
    MobDescription(86 , 2  , 4, "ROCK_MISTS_NON_LETHAL"), // health: 1203
    MobDescription(87 , 2  , 5, "ROCK_MISTS_NON_LETHAL"), // health: 1367
    MobDescription(88 , 2  , 6, "ROCK_MISTS"), // health: 1770
    MobDescription(111, 2  , 5, "ROCK_MISTS"), // health: 1640
    MobDescription(113, 2  , 7, "ROCK_MISTS"), // health: 2196
    MobDescription(134, 2  , 4, "ROCK_MISTS"), // health: 1444
    MobDescription(135, 2  , 5, "ROCK_MISTS"), // health: 1640


    /*MobDescription(223, 1, 3, "ORE"),
    MobDescription(224, 1  , 5, "ORE"),
    MobDescription(225, 1  , 5, "ORE"),
    MobDescription(26 , 2  , 4, "ORE"),
    MobDescription(28 , 2  , 6, "ORE"),
    MobDescription(31 , 2  , 4, "ORE_ELEMENTAL"),
    MobDescription(32 , 2  , 5, "ORE_ELEMENTAL"),
    MobDescription(33 , 2  , 6, "ORE_ELEMENTAL"),
    MobDescription(35 , 2  , 8, "ORE_ELEMENTAL"),
    MobDescription(68 , 2  , 3, "ORE_MISTS"),
    MobDescription(69 , 2  , 4, "ORE"),
    MobDescription(70 , 2  , 5, "ORE"),
    MobDescription(71 , 2  , 6, "ORE"),
    MobDescription(94 , 2  , 5, "ORE_MISTS"),
    MobDescription(95 , 2  , 6, "ORE_MISTS"),
    MobDescription(116, 2  , 3, "ORE_MISTS"),
    MobDescription(117, 2  , 4, "ORE_MISTS"),
    MobDescription(118, 2  , 5, "ORE_MISTS"),
    MobDescription(119, 2  , 6, "ORE_MISTS"),
    MobDescription(189, 2  , 8, "ORE_ASPECT_OLD"),
    MobDescription(222, 1  , 3, "ORE"),
    MobDescription(226, 1  , 7, "ORE"),*/
    MobDescription(51 , 2  , 6, "ORE"), // health: 1475
    MobDescription(66 , 2  , 6, "ORE"), // health: 1475
    MobDescription(91 , 2  , 3, "ORE_MISTS_NON_LETHAL"), // health: 1203
    MobDescription(92 , 2  , 4, "ORE_MISTS_NON_LETHAL"), // health: 1203
    MobDescription(93 , 2  , 5, "ORE_MISTS_NON_LETHAL"), // health: 1367
    MobDescription(139, 2  , 3, "ORE_MISTS"), // health: 1444
    MobDescription(140, 2  , 4, "ORE_MISTS"), // health: 1444
    MobDescription(141, 2  , 5, "ORE_MISTS"), // health: 1640
    MobDescription(199, 2  , 6, "ORE_ASPECT_COLOSSUS"),
    MobDescription(246, 1  , 3, "ORE"),
    MobDescription(247, 1  , 5, "ORE"),
    MobDescription(248, 1  , 5, "ORE"), // health: 1367

    MobDescription(228, 2, 6, "TREASURE_AVALON"), // health: 139

    MobDescription(231, 2, 4, "TREASURE_FOREST"), // health: 132
    MobDescription(232, 2, 4, "TREASURE_PLAIN"), // health: 126
    MobDescription(233, 2, 4, "TREASURE_SWAMP"), // health: 144
    MobDescription(234, 2, 4, "TREASURE_MOUNTAIN"),
    MobDescription(235, 2, 4, "TREASURE_STEPPE"),
    MobDescription(236, 2, 5, "TREASURE_FOREST"), // health: 129 // health: 135 // от зачарки зависит редкость
    MobDescription(237, 2, 5, "TREASURE_PLAIN"), // health: 135
    MobDescription(238, 2, 5, "TREASURE_SWAMP"), // health: 155
    MobDescription(239, 2, 5, "TREASURE_MOUNTAIN"),
    MobDescription(240, 2, 5, "TREASURE_STEPPE"),
    MobDescription(241, 2, 6, "TREASURE_FOREST"), // health: 146
    MobDescription(242, 2, 6, "TREASURE_PLAIN"),
    MobDescription(243, 2, 6, "TREASURE_SWAMP"), // health: 146
    MobDescription(244, 2, 6, "TREASURE_MOUNTAIN"), // health: 146
    MobDescription(245, 2, 6, "TREASURE_STEPPE"),


    
    /*MobDescription(62 , 3  , 5, "AVALONIAN_TREASURE_UNCOMMON"),
    MobDescription(63 , 3  , 6, "AVALONIAN_TREASURE_COMMON"),
    MobDescription(64 , 3  , 6, "AVALONIAN_TREASURE_UNCOMMON"),
    MobDescription(69 , 3  , 8, "AVALONIAN_TREASURE_RARE"),*/
    
};

inline bool isMobKnown(uint8_t category, uint8_t mobTypeID) {
    auto it = std::find_if(mobDescriptions.begin(), mobDescriptions.end(),
        [category, mobTypeID](const MobDescription& mob) {
            return (mob._category == category && mob._typeID == mobTypeID);
        });
    return it != mobDescriptions.end(); // Return true if found
}
inline const MobDescription& getMobDescription(uint8_t category, uint8_t mobTypeID) {
    static const MobDescription invalidMob = MobDescription(0, 0, 0, "MOB_NOT_FOUND");

    auto it = std::find_if(mobDescriptions.begin(), mobDescriptions.end(),
        [category, mobTypeID](const MobDescription& mob) {
            return (mob._category == category && mob._typeID == mobTypeID);
        });

    if (it != mobDescriptions.end()) {
        return *it;
    }
    else {
        return invalidMob;
    }
}

#endif