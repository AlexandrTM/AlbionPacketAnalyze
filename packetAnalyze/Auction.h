#ifndef AUCTION_H
#define AUCTION_H

struct Auction
{
	static void findAuctionAverageValues(NetworkCommand& command, std::string& itemData, std::string dataSeparator);
	static void GetItemData(
		NetworkCommand& command,
		Location& currentLocation,
		std::string& itemData,
		bool printInfo
	);
	static void processAuctionOrders(const std::string& auctionSellOrdersString, bool isSellOrders);
	static void auctionOrders(NetworkCommand& command, bool isSellOrders);

	static void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H