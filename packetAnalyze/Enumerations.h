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
    AuctionGetFinishedAuctions = 83,
    AuctionGetFinishedAuctionsCount = 84,
    AuctionGetMyOpenAuctions = 88,
    AuctionGetItemAverageStats = 89,

    GetCharacterEquipment = 136,
    GetClusterMapInfo = 190,
    GoldMarketGetAverageInfo = 242,
    RealEstateGetAuctionData = 256,

};
enum eventCode : uint16_t
{
    None = 0,
    Leave = 1, // player leave 
    PlayerMove = 3,
    Teleport = 4,
    HealthUpdate = 6,
    ActiveSpellEffectsUpdate = 11,
    ChannelingEnded = 24,
    NewCharacter = 29,
    NewSimpleItem = 32,
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
    Mount = 192,
    MountCancel = 193,
    BuyJourney = 194,
    NewRandomDungeonExit = 307,
    FishingFinished = 352,
    NewFishingZoneObject = 355,

};

enum harvestableUniqueType : uint8_t
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

enum class HarvestableType : int8_t
{
    UNDEFINED = -1,
    WOOD = 0,
    ROCK = 1,
    FIBER = 2,
    HIDE = 3,
    ORE = 4
};

struct HarvestableRange
{
    uint8_t start;
    uint8_t end;
};

// Two sets of ranges: Standard and Roads variants
const std::array<std::pair<HarvestableRange, HarvestableRange>, 5> staticHarvestableRanges = { {
    {{0, 6},   {33, 37}}, // WOOD & WOOD_ROADS
    {{7, 13},  {38, 42}}, // ROCK & ROCK_ROADS
    {{14, 19}, {43, 47}}, // FIBER & FIBER_ROADS
    {{20, 26}, {48, 52}}, // HIDE & HIDE_ROADS
    {{27, 32}, {53, 57}}  // ORE & ORE_ROADS
} };

const std::vector<std::string> harvestableKeywords = {
    "WOOD_", "_ROCK", "_FIBER", "_ORE", "_ENT", "DRYAD"
};

const std::vector<std::pair<std::string, HarvestableType>> harvestableMappings = {
        {harvestableKeywords[0], HarvestableType::WOOD},
        {harvestableKeywords[1], HarvestableType::ROCK},
        {harvestableKeywords[2], HarvestableType::FIBER},
        {harvestableKeywords[3], HarvestableType::ORE},
        {harvestableKeywords[4], HarvestableType::WOOD},
        {harvestableKeywords[5], HarvestableType::FIBER}
};

const std::vector<std::string> harvestableSpecialNames = { 
    "ELEMENTAL", "VETERAN", "ELITE", "GUARDIAN", "ROCKGIANT", "OREGIANT"};

const std::unordered_map<std::string, std::string> biomeMap = {
    {"SW", "swamp"},
    {"FR", "forest"},
    {"HL", "highlands"},
    {"ST", "steppe"},
    {"MN", "mountains"}
};

#endif