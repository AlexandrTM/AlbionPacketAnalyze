#ifndef AUCTION_H
#define AUCTION_H

#include "Location.h"
#include "AuctionOrder.h"

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
		bool isSellOrders, bool isFilterEnabled, std::vector<AuctionOrder> auctionOrders
	);
	void printPlayersAuctionData();

	void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H