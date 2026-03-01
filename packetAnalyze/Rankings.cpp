#include "pch.h"
#include "Rankings.h"

static uint32_t previousCommandID = 0;

void Rankings::getRankings(NetworkCommand& command)
{
    if (previousCommandID == command.getCommandID()) return;
    previousCommandID = command.getCommandID();

    DataLayout dataLayout;
    dataLayout.findDataLayout(command);
    //dataLayout.printInfo(command);

    //std::vector<std::reference_wrapper<DataFragment>> rankingsGuildNames = dataLayout.findFragments(17);
    std::vector<std::reference_wrapper<DataFragment>> rankingsNames = dataLayout.findFragments(11);
    int32_t rankingType = net::read_uint32(command, dataLayout.findFragment(0)._offset);
    int32_t rankingDateType = net::read_uint32(command, dataLayout.findFragment(2)._offset);
    // 0 - all time, 1 - this week, 2 - last week
    
	/*
    RANKING_CATEGORY_ARENA_WINS
    RANKING_CATEGORY_CRYSTAL_ARENA_RANKING_POINTS
    RANKING_CATEGORY_FACTION_CLUSTER_CAPTURED
    RANKING_CATEGORY_FACTION_OUTPOST_CAPTURED
    RANKING_CATEGORY_FACTION_POINTS_COLLECTED
    RANKING_CATEGORY_FACTION_PVP_KILLS
    RANKING_CATEGORY_FACTION_PVP_KNOCKOUTS
    RANKING_CATEGORY_FACTION_STANDING
    RANKING_CATEGORY_FAME
    RANKING_CATEGORY_GATHER_FIBER
    RANKING_CATEGORY_GATHER_FISHING
    RANKING_CATEGORY_GATHER_HIDE
    RANKING_CATEGORY_GATHER_ORE
    RANKING_CATEGORY_GATHER_STONE
    RANKING_CATEGORY_GATHER_WOOD
    RANKING_CATEGORY_INFAMY_CORRUPTED_DUNGEONS
    RANKING_CATEGORY_INFAMY_HELLGATES_10V10
    RANKING_CATEGORY_INFAMY_HELLGATES_2V2
    RANKING_CATEGORY_INFAMY_HELLGATES_5V5
    RANKING_CATEGORY_JOURNAL
    RANKING_CATEGORY_KILL_VALUE
    RANKING_CATEGORY_PEAK_INFAMY
    RANKING_CATEGORY_PVE_GROUP
    RANKING_CATEGORY_PVE_HCE
    RANKING_CATEGORY_PVE_HELL_DUNGEON
    RANKING_CATEGORY_PVE_MISTS
    RANKING_CATEGORY_PVE_OPENWORLD
    RANKING_CATEGORY_PVE_RAID
    RANKING_CATEGORY_PVE_ROADS
    RANKING_CATEGORY_PVE_SOLO
    RANKING_CATEGORY_PVE_STATIC
    RANKING_CATEGORY_PVE_VALUE
    RANKING_CATEGORY_PVP_CORRUPTED_DUNGEON
    RANKING_CATEGORY_PVP_HELL_DUNGEON
    RANKING_CATEGORY_PVP_HELLGATE
    RANKING_CATEGORY_PVP_MISTS
    RANKING_CATEGORY_PVP_OPENWORLD
    RANKING_CATEGORY_PVP_ROADS
    RANKING_CATEGORY_RESOURCE_VALUE
    RANKING_CATEGORY_SILVER_COLLECTED
    RANKING_CATEGORY_TERRITORIES_OWNED
    RANKING_CATEGORY_TRACKING
    */

    std::cout << "ranking type: " << rankingType << "\n";
    switch (rankingDateType) {
        case 0: std::cout << "all time"; break;
        case 1: std::cout << "this week"; break;
        case 2: std::cout << "last week"; break;
        default: std::cout << "unknown (" << rankingDateType << ")"; break;
    }
    std::cout << "\n*player name*\n";
    for (const auto& rankingsName : rankingsNames) {
        const DataFragment& nameFragment = rankingsName.get();

        size_t from = nameFragment._offset;
        size_t to = from + nameFragment._numOfEntries;

        std::string name(command.begin() + from, command.begin() + to);
        std::cout<< name << "\n";
    }
    std::cout << "\n";
}
