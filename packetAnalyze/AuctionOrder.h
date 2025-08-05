#pragma once

struct AuctionOrder
{
	uint64_t id = 0;
	std::string location = "";
	std::string playerName;
	std::string itemTypeId;
	double unitPrice = 0;
	size_t amount = 0;
	size_t totalSilver = 0;
	AuctionOrderType type = AuctionOrderType::UNDEFINED;

	std::string toString() const {
		std::ostringstream oss;
		oss << std::left <<
			std::setw(20) << playerName  << " " <<
			std::setw(8)  << 
				(type == AuctionOrderType::BUY ? "buy" : 
				type == AuctionOrderType::SELL ? "sell" : 
				type == AuctionOrderType::FINISHED ? "finished" : 
				"undefined") << " " <<
			std::setw(30) << itemTypeId  << " " << 
			std::fixed << std::setprecision(0) <<
			std::setw(10) << unitPrice   << " " <<
			std::setw(7)  << amount      << " " <<
			std::setw(12) << totalSilver;
		return oss.str();
	}
};

