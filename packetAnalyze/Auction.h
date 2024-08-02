#ifndef AUCTION_H
#define AUCTION_H

struct Auction
{
	static void findProductName(NetworkCommand& command, DataLayout &dataLayout);
	static void findAuctionAverageValues(NetworkCommand& command, std::string dataSeparator);

	static void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H