#ifndef AUCTION_H
#define AUCTION_H

#include "Location.h"

namespace Auction
{
	void findAuctionAverageValues(NetworkCommand& command, std::string& itemData, std::string dataSeparator);
	void GetItemData(
		NetworkCommand& command,
		Location& currentLocation,
		std::string& itemData,
		bool printInfo
	);
	void processAuctionOrders(
		const std::string& auctionSellOrdersString, bool isSellOrders, 
		Location& currentLocation, bool isFilterEnabled
	);
	void auctionOrders(
		NetworkCommand& command, bool isSellOrders, 
		Location& currentLocation, bool isFilterEnabled
	);
	void printAuctionOrders(
		bool isFilterEnabled,
		std::unordered_map
			<std::string, // player name
			std::unordered_map<std::string, std::pair<size_t, size_t>>> // item id | amount | total silver
		auctionOrdersData
	);

	void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H