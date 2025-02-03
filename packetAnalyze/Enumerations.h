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
    None = 0,
    Leave = 1, // player leave 
    PlayerMove = 3,
    HealthUpdate = 6,
    ActiveSpellEffectsUpdate = 11,
    NewCharacter = 29,
    NewSimpleHarvestableObjectList = 39,
    NewHarvestableObject = 40,
    NewSilverObject = 44,
    NewBuilding = 45,
    HarvestableChangeState = 46,
    MobChangeState = 47,
    CraftBuildingInfo = 49,
    HarvestStart = 59,
    HarvestFinished = 61,
    CraftItemFinished = 71,
    EquipmentChanged = 85,
    NewMob = 123,
    NewRandomDungeonExit = 307,

};

enum resourceType : uint8_t
{
    WOOD = 0,
    WOOD_DYNAMIC = 1,
    WOOD_GIANTTREE = 2,
    WOOD_CRITTER = 3,
    WOOD_GUARDIAN = 4,
    WOOD_MINIGUARDIAN = 5,
    WOOD_TREASURE = 6,
    ROCK = 7,
    ROCK_DYNAMIC = 8,
    ROCK_CRITTER = 9,
    ROCK_CRITTER_RANDOM_DUNGEON = 10,
    ROCK_GUARDIAN = 11,
    ROCK_MINIGUARDIAN = 12,
    ROCK_TREASURE = 13,
    FIBER = 14,
    FIBER_DYNAMIC = 15,
    FIBER_CRITTER = 16,
    FIBER_GUARDIAN = 17,
    FIBER_MINIGUARDIAN = 18,
    FIBER_TREASURE = 19,
    HIDE = 20,
    HIDE_DYNAMIC = 21,
    HIDE_FOREST_02 = 22,
    HIDE_CRITTER = 23,
    HIDE_GUARDIAN = 24,
    HIDE_MINIGUARDIAN = 25,
    HIDE_TREASURE = 26,
    ORE = 27,
    ORE_DYNAMIC = 28,
    ORE_CRITTER = 29,
    ORE_GUARDIAN = 30,
    ORE_MINIGUARDIAN = 31,
    ORE_TREASURE = 32,
    WOOD_CRITTER_ROADS = 33,
    WOOD_CRITTER_ROADS_VETERAN = 34,
    WOOD_CRITTER_ROADS_ELITE = 35,
    WOOD_GUARDIAN_ROADS = 36,
    WOOD_MINIGUARDIAN_ROADS = 37,
    ROCK_CRITTER_ROADS = 38,
    ROCK_CRITTER_ROADS_VETERAN = 39,
    ROCK_CRITTER_ROADS_ELITE = 40,
    ROCK_GUARDIAN_ROADS = 41,
    ROCK_MINIGUARDIAN_ROADS = 42,
    FIBER_CRITTER_ROADS = 43,
    FIBER_CRITTER_ROADS_VETERAN = 44,
    FIBER_CRITTER_ROADS_ELITE = 45,
    FIBER_GUARDIAN_ROADS = 46,
    FIBER_MINIGUARDIAN_ROADS = 47,
    HIDE_CRITTER_ROADS = 48,
    HIDE_CRITTER_ROADS_VETERAN = 49,
    HIDE_CRITTER_ROADS_ELITE = 50,
    HIDE_GUARDIAN_ROADS = 51,
    HIDE_MINIGUARDIAN_ROADS = 52,
    ORE_CRITTER_ROADS = 53,
    ORE_CRITTER_ROADS_VETERAN = 54,
    ORE_CRITTER_ROADS_ELITE = 55,
    ORE_GUARDIAN_ROADS = 56,
    ORE_MINIGUARDIAN_ROADS = 57,
    OTHER = 58,
};

struct ResourceRange
{
    uint8_t start;
    uint8_t end;
};

// Two sets of ranges: Standard and Roads variants
const std::array<std::pair<ResourceRange, ResourceRange>, 5> resourceRanges = { {
    {{0, 6},   {33, 37}}, // WOOD & WOOD_ROADS
    {{7, 13},  {38, 42}}, // ROCK & ROCK_ROADS
    {{14, 19}, {43, 47}}, // FIBER & FIBER_ROADS
    {{20, 26}, {48, 52}}, // HIDE & HIDE_ROADS
    {{27, 32}, {53, 57}}  // ORE & ORE_ROADS
} };

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