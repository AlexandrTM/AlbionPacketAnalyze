#ifndef AUCTION_H
#define AUCTION_H

struct Auction
{
	static void findAuctionAverageValues(NetworkCommand& command);

	static void addEmptyEntries(std::vector<std::vector<std::string>>& auctionData);
};

#endif // !AUCTION_H