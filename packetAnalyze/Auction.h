#ifndef AUCTION_H
#define AUCTION_H

#include "Location.h"
#include "AuctionOrder.h"
#include "entity_list_instance.h"

namespace Auction
{
	void findAuctionAverageValues(
		NetworkCommand& command, std::string& itemData, std::string dataSeparator
	);
	void GetItemData(
		NetworkCommand& command,
		Location& currentLocation,
		std::string& itemData,
		bool printInfo
	);
	void processAuctionOrders(
		const std::string& auctionSellOrdersString,
		Location& currentLocation, bool isFilterEnabled
	);
	void findAuctionOrders(
		NetworkCommand& command,
		Location& currentLocation, bool isFilterEnabled
	);
	void printAuctionOrders(bool isFilterEnabled, std::vector<AuctionOrder> auctionOrders);
	void printPlayersAuctionData();

	void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H