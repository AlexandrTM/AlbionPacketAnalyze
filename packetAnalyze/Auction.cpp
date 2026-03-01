#include "pch.h"
#include "Auction.h"

//size_t counter = 0;
constexpr uint64_t sixHours = static_cast<uint64_t>(3600 * 6);
constexpr uint64_t years1970offset = static_cast<uint64_t>(62135596800);
uint16_t numOfColumns = 4;
uint16_t numOfRows = 113;
static uint32_t previousCommandID = 0;
std::string previousAuctionOrdersString = "";

//std::unordered_set<uint64_t> seenOrderIds;

void Auction::findAuctionAverageValues(NetworkCommand& command, std::string& itemData, std::string dataSeparator)
{
	if (previousCommandID != command.getCommandID()) {

		//counter += 1;
		//std::cout << counter << "\n";
		DataLayout dataLayout;
		dataLayout.findDataLayout(command);
		//dataLayout.printInfo(command);
		//command.printCommandInOneString();

		//std::cout << net::read_uint32(command, dataLayout.findFragment(255)._offset) << "\n";

		std::ofstream auctionAverageValues;
		auctionAverageValues.open("auctionAverageValues.csv", std::ofstream::app);

		if (auctionAverageValues.is_open()) {

			DataFragment& soldAmountFragment = dataLayout.findFragment(0);
			DataFragment& soldVolumeFragment = dataLayout.findFragment(1);
			DataFragment& dateFragment = dataLayout.findFragment(2);

			uint16_t numOfEntries = soldAmountFragment._numOfEntries;

			std::vector<std::vector<std::string>> auctionData(numOfEntries, 
				std::vector<std::string>(numOfColumns));

			uint8_t soldAmountDataTypeSize = soldAmountFragment._dataType._size;
			uint8_t soldVolumeDataTypeSize = soldVolumeFragment._dataType._size;

			uint32_t soldAmount = 0;
			uint64_t soldVolume = 0;
			double_t averagePrice = 0;
			uint64_t date = 0;

			for (size_t i = 0; i < numOfEntries; i++) {
				if (soldAmountDataTypeSize == 4) 
					{ soldAmount = net::read_uint32(command, soldAmountFragment._offset + i * 4); }
				else if (soldAmountDataTypeSize == 2) 
					{ soldAmount = net::read_uint16(command, soldAmountFragment._offset + i * 2); }
				else if (soldAmountDataTypeSize == 1) 
					{ soldAmount = net::read_uint8 (command, soldAmountFragment._offset + i); }
				else if (soldAmountDataTypeSize == 8) 
					{ soldAmount = net::read_uint64(command, soldAmountFragment._offset + i * 8); }

				if (soldVolumeDataTypeSize == 8) 
					{ soldVolume = net::read_uint64(command, soldVolumeFragment._offset + i * 8) / 1e+4; }
				else if (soldVolumeDataTypeSize == 4) 
					{ soldVolume = net::read_uint32(command, soldVolumeFragment._offset + i * 4) / 1e+4; }
				averagePrice = soldAmount > 0 ? (double_t)soldVolume / soldAmount : 0;
				date = net::read_uint64(command, dateFragment._offset + i * 8) / 1e+7;

				auctionData[i][0] = std::numberToString(soldAmount);
				auctionData[i][1] = std::numberToString(soldVolume);
				auctionData[i][2] = std::numberToString(averagePrice);
				auctionData[i][3] = std::numberToString(date);
			}
			//std::cout << auctionData[auctionData.size() - 1][3] << "\n";

			size_t idOfSortingRow = 3;
			std::sort(auctionData.begin(), auctionData.end(),
				[idOfSortingRow](const std::vector<std::string>& a, const std::vector<std::string>& b) {
					return std::stoull(a.at(idOfSortingRow)) < std::stoull(b.at(idOfSortingRow));
				});

			//std::cout << "auctionData.size(): " << auctionData.size() << "\n";
			addEmptyEntries(auctionData);

			std::sort(auctionData.begin(), auctionData.end(),
				[idOfSortingRow](const std::vector<std::string>& a, const std::vector<std::string>& b) {
					return std::stoull(a.at(idOfSortingRow)) < std::stoull(b.at(idOfSortingRow));
				});

			//for (size_t i = 0; i < 4; i++) {
			//	for (size_t j = 0; j < auctionData.size(); j++) {
			//		auctionAverageValues << auctionData[j][i] << dataSeparator;
			//	}
			//	auctionAverageValues << i << " " << itemData << "\n";
			//	/*if (i == 3) {
			//		auctionAverageValues << itemData;
			//	}*/
			//}
			for (size_t i = 0; i < 2; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i] << dataSeparator;
				}
				auctionAverageValues << i << " " << itemData << "\n";
				/*if (i == 1) {
					auctionAverageValues << itemData;
				}*/
			}
			auctionAverageValues.close();
		}
		else {
			std::cout << "auctionAverageValues.csv is not opened" << "\n";
		}
	}
	previousCommandID = command.getCommandID();
}

void Auction::addEmptyEntries(std::vector<std::vector<std::string>>& auctionData) 
{
	auto now = std::chrono::system_clock::now();
	uint64_t nowEpochSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() + years1970offset;
	uint64_t currentAlignedTime = nowEpochSeconds - (nowEpochSeconds % sixHours);

	uint64_t firstAlignedTime = currentAlignedTime;
	uint64_t lastAlignedTime = currentAlignedTime;
	if (auctionData.size() > 0) {
		firstAlignedTime = std::stoull(auctionData[0][3]);
		lastAlignedTime = std::stoull(auctionData[auctionData.size() - 1][3]);
	}

	/*std::cout << 
		  "firstAlignedTime:   " << firstAlignedTime << 
		"\nlastAlignedTime:    " << lastAlignedTime <<
		"\ncurrentAlignedTime: " << currentAlignedTime << "\n";
	std::cout << std::stoull(auctionData[auctionData.size() - 1][1]) << "\n";*/

	uint64_t datePrevious = 0;
	size_t initialSize = auctionData.size();
	for (size_t i = 0; i < initialSize; i++) {

		uint64_t date = std::stoull(auctionData[i][3]);
		if (i > 0) {
			size_t dateDifference = (date - datePrevious) / sixHours;
			if (dateDifference > 1) {
				for (size_t j = 1; j < dateDifference; j++) {
					auctionData.push_back(
						{ "0", "0", "0", std::numberToString(datePrevious + j * sixHours) });
				}
			}
		}
		datePrevious = date;
	}

	if (lastAlignedTime < currentAlignedTime) {
		for (uint64_t t = lastAlignedTime + sixHours; t <= currentAlignedTime; t += sixHours) {
			auctionData.push_back({ "0", "0", "0", std::numberToString(t) });
		}
	}

	size_t valuesToAdd = numOfRows - auctionData.size();
	for (size_t i = 0; i < valuesToAdd; i++) {
		auctionData.push_back({ "0", "0", "0", std::numberToString(firstAlignedTime - (i + 1) * sixHours) });
	}
}

void Auction::GetItemData(
	NetworkCommand& command,
	Location& currentLocation,
	std::string& itemData,
	bool printInfo
)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);

	DataFragment& cityFragment = dataLayout.findFragment(0);
	DataFragment& itemIdFragment = dataLayout.findFragment(1);
	DataFragment& qualityFragment = dataLayout.findFragment(2);
	DataFragment& dataScaleFragment = dataLayout.findFragment(3);

	uint64_t cityId = net::read_integer(command, cityFragment);
	uint64_t itemId = net::read_integer(command, itemIdFragment);
	uint64_t qualityId = net::read_integer(command, qualityFragment);
	uint64_t dataScaleId = net::read_integer(command, dataScaleFragment);

	std::ofstream auctionAverageValues;

	itemData =
		"currentLocation: " + currentLocation._id + " " +
		"itemId: " + std::to_string(itemId)				  + " " +
		"qualityId: " + std::to_string(qualityId)		  + " " +
		"dataScaleId: " + std::to_string(dataScaleId);

	if (printInfo) {
		//std::cout << itemData << "\n";
		std::cout << std::to_string(itemId) << "\n";
	}
	//dataLayout.printInfo(command);
	// command.printCommandInOneString();
}

void Auction::findAuctionOrders(
	NetworkCommand& command,
	Location& currentLocation, bool isFilterEnabled
)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);
	//dataLayout.printInfo(command);

	std::vector<std::reference_wrapper<DataFragment>> auctionOrdersFragments = dataLayout.findFragments(0);

	std::string auctionOrdersString{};
	for (const auto& auctionOrderFragmentReference : auctionOrdersFragments) {
		const DataFragment& auctionOrderFragment = auctionOrderFragmentReference.get();

		size_t from = auctionOrderFragment._offset;
		size_t to = from + auctionOrderFragment._numOfEntries;

		auctionOrdersString += std::string(command.begin() + from, command.begin() + to);
		//std::cout << auctionOrdersString << "\n";
	}

	if (previousAuctionOrdersString != auctionOrdersString) {
		//std::cout << auctionOrdersString << "\n";
		processAuctionOrders(auctionOrdersString, currentLocation, isFilterEnabled);
	}
	previousAuctionOrdersString = auctionOrdersString;
}

struct PlayerAuctionData
{
	int64_t totalSilver = 0;
	int64_t sellSilver = 0;
	int64_t buySilver = 0;

	std::vector<AuctionOrder> orders;

	void addOrder(const AuctionOrder& newOrder) {
		// Find existing order with same ID
		auto it = std::find_if(orders.begin(), orders.end(),
			[&newOrder](const AuctionOrder& o) {
				return o.id == newOrder.id;
			});

		if (it != orders.end()) {
			// If amount differs, replace the order
			if (it->amount != newOrder.amount) {
				//std::cout << newOrder.toString() << "\n";
				// Subtract old order
				totalSilver -= it->totalSilver;
				if (it->type == AuctionOrderType::BUY)
					buySilver -= it->totalSilver;
				else if (it->type == AuctionOrderType::SELL)
					sellSilver -= it->totalSilver;

				// Replace old order with new one
				*it = newOrder;
				totalSilver += newOrder.totalSilver;
				if (newOrder.type == AuctionOrderType::BUY)
					buySilver += newOrder.totalSilver;
				else if (newOrder.type == AuctionOrderType::SELL)
					sellSilver += newOrder.totalSilver;
			}
			else {
				// If amount is same, do nothing
				return;
			}
		}
		else {
			// Order is new, add it
			orders.push_back(newOrder);
			totalSilver += newOrder.totalSilver;
			if (newOrder.type == AuctionOrderType::BUY)
				buySilver += newOrder.totalSilver;
			else if (newOrder.type == AuctionOrderType::SELL)
				sellSilver += newOrder.totalSilver;
		}
	}

	std::string totalsToString() const {
		std::ostringstream oss;
		oss <<
			std::right <<
			std::setw(18) << totalSilver <<
			std::setw(18) << sellSilver <<
			std::setw(18) << buySilver << "\n";
		return oss.str();
	}

	std::string ordersToString() const {
		std::vector<AuctionOrder> sortedOrders = orders;
		std::sort(sortedOrders.begin(), sortedOrders.end(),
			[](const AuctionOrder& a, const AuctionOrder& b) {
				return a.totalSilver > b.totalSilver;
			}
		);

		std::ostringstream oss;
		size_t count = std::min<size_t>(
			std::min<size_t>(
				20, std::min<size_t>(20 + std::floor((totalSilver - 20'000'000) / 10'000'000), 50)
			), sortedOrders.size()
		);
		for (size_t i = 0; i < count; ++i) {
			oss << " - " << sortedOrders[i].toString() << "\n";
		}
		return oss.str();
	}
};

std::unordered_map<std::string, PlayerAuctionData> playerAuctionMap;

void Auction::processAuctionOrders(
	const std::string& auctionSellOrdersString,
	Location& currentLocation, bool isFilterEnabled
) {
	std::vector<AuctionOrder> auctionOrders;

	size_t pos = 0;
	while (pos < auctionSellOrdersString.size()) {
		size_t start = auctionSellOrdersString.find('{', pos);
		if (start == std::string::npos) break;
		size_t end = auctionSellOrdersString.find('}', start);
		if (end == std::string::npos) break;

		std::string jsonStr = auctionSellOrdersString.substr(start, end - start + 1);
		//std::cout << jsonStr << "\n";
		pos = end + 1;

		try {
			nlohmann::json auctionOrder = nlohmann::json::parse(jsonStr);
			AuctionOrder order;

			//std::cout << auctionOrder << "\n";

			std::string orderTypeString = auctionOrder["AuctionType"];
			if (auctionOrder["IsFinished"] == true) {
				order.type = AuctionOrderType::FINISHED;
			}
			else if (orderTypeString == "offer") {
				order.type = AuctionOrderType::SELL;
			}
			else if (orderTypeString == "request") {
				order.type = AuctionOrderType::BUY;
			}
			else {
				order.type = AuctionOrderType::UNDEFINED;
			}
			order.id = auctionOrder["Id"];
			order.location = entityList._currentLocation._id;
			if (order.type == AuctionOrderType::SELL) {
				order.playerName = auctionOrder["SellerName"];
			}
			else if (order.type == AuctionOrderType::BUY) {
				order.playerName = auctionOrder["BuyerName"];
			}
			else if (order.type == AuctionOrderType::FINISHED) {
				order.playerName = auctionOrder["SellerName"];
			}
			order.timePlaced = 
				net::parse_utc_time_string(auctionOrder["Expires"]) - 
				std::chrono::days(order.type == AuctionOrderType::FINISHED ? 60 : 30);
			//std::cout << order.timePlaced << "\n";

			order.itemTypeId = auctionOrder["ItemTypeId"];
			order.amount = auctionOrder["Amount"];
			order.totalSilver = static_cast<double>(auctionOrder["TotalPriceSilver"]) / 1e4;
			order.unitPrice = 
				order.amount > 0 ? static_cast<double>(order.totalSilver) / order.amount : 0.0;

			auctionOrders.push_back(order);

			// add new order or replace existing one
			playerAuctionMap[order.playerName].addOrder(order);
		}
		catch (const std::exception& e) {
			std::cerr << "JSON Parsing Error: " << e.what() << "\n";
		}
	}

	printAuctionOrders(isFilterEnabled, auctionOrders);
}

void Auction::printAuctionOrders(bool isFilterEnabled, std::vector<AuctionOrder> auctionOrders)
{
	/*if (orderType) {
		std::cout << "sell orders\n";
	}
	else {
		std::cout << "buy orders\n";
	}*/

	size_t totalAmount = 0;
	size_t totalSilver = 0;

	for (const auto& order : auctionOrders) {
		totalAmount += order.amount;
		totalSilver += order.totalSilver;
	}

	// Sort descending by amount
	std::vector<AuctionOrder> sortedOrders = auctionOrders;
	/*std::sort(sortedOrders.begin(), sortedOrders.end(), 
		[](const AuctionOrder& a, const AuctionOrder& b) {
			return a.amount > b.amount;
		}
	);*/

	// Compute threshold
	size_t thresholdAmount = static_cast<size_t>(1.0 * totalAmount);
	size_t accumulatedAmount = 0;

	std::ofstream auctionOrdersFile;
	auctionOrdersFile.open("auction orders.txt", std::ofstream::app);

	if (auctionOrdersFile.is_open()) {
		std::cout << "total amount: " << totalAmount << " total silver: " << totalSilver << "\n";
		std::cout << std::left <<
			std::setw(20) << "playerName"  << " " <<
			std::setw(8)  << "type"		   << " " <<
			std::setw(30) << "itemTypeId"  << " " <<
			std::setw(10) << "price"       << " " <<
			std::setw(7)  << "amount"      << " " <<
			std::setw(12) << "totalSilver"
			<< "\n";

		// Print the sorted result
		for (const auto& order : sortedOrders) {
			if (accumulatedAmount >= thresholdAmount) {
				break; // Stop once we have reached threshold
			}
			accumulatedAmount += order.amount;

			if (!isFilterEnabled || (order.amount >= 30 || order.totalSilver >= 5e5)) {
				std::cout << order.toString() << "\n";
			}
		}
		std::cout << "\n";
		auctionOrdersFile.close();
	}
	else {
		std::cout << "auction orders.txt is not opened" << "\n";
	}
}

void Auction::printPlayersAuctionData() {
	int64_t totalSilver = 0;
	int64_t totalBuySilver = 0;
	int64_t totalSellSilver = 0;

	size_t countUnder5m = 0;
	size_t count5mTo20m = 0;
	size_t count20mPlus = 0;

	for (const auto& [name, playerAuctionData] : playerAuctionMap) {
		totalBuySilver += playerAuctionData.buySilver;
		totalSellSilver += playerAuctionData.sellSilver;

		const int64_t ts = playerAuctionData.totalSilver;
		if (ts < 5'000'000)
			++countUnder5m;
		else if (ts < 20'000'000)
			++count5mTo20m;
		else
			++count20mPlus;
	}

	totalSilver = totalBuySilver + totalSellSilver;

	if (totalSilver == 0) return;

	// Create vector of pairs for sorting
	std::vector<std::pair<std::string, PlayerAuctionData>> sortedPlayersAuctionData(
		playerAuctionMap.begin(), playerAuctionMap.end()
	);

	// Sort by totalSilver descending
	std::sort(sortedPlayersAuctionData.begin(), sortedPlayersAuctionData.end(),
		[](const auto& a, const auto& b) {
			return a.second.totalSilver > b.second.totalSilver;
		}
	);

	// Print and save
	//std::string timestamp = net::sanitize_filename(net::get_utc_time());
	//std::string filename = "players_auction_data_" + timestamp + ".txt";
	std::string filename = "players_auction_data.txt";
	std::ofstream out(filename);

	// Print grand totals
	/*std::cout << 
		"\nTotal Buy Silver:  " << std::setw(18) << totalBuySilver << 
		"\nTotal Sell Silver: " << std::setw(18) << totalSellSilver << 
		"\nTotat players:     " << std::setw(18) << totals.size() << "\n\n";*/

	out << 
		"Total Silver:      " << std::setw(18) << totalSilver << "\n" <<
		"Total Buy Silver:  " << std::setw(18) << totalBuySilver << "\n" <<
		"Total Sell Silver: " << std::setw(18) << totalSellSilver << "\n\n" <<

		"Totat players:     " << std::setw(18) << playerAuctionMap.size() << "\n"
		"Players < 5m:      " << std::setw(18) << countUnder5m << "\n" <<
		"Players [5m–20m):  " << std::setw(18) << count5mTo20m << "\n" <<
		"Players >= 20m:    " << std::setw(18) << count20mPlus << "\n\n";

	/*std::cout << 
		std::left << 
		std::setw(24) << "Player" << 
		std::right <<
		std::setw(18) << "Total Silver" <<
		std::setw(18) << "Sell Silver" <<
		std::setw(18) << "Buy Silver" << "\n";*/

	out << 
		std::left << 
		std::setw(24) << "Player" <<
		std::right <<
		std::setw(18) << "Total Silver" <<
		std::setw(18) << "Sell Silver" <<
		std::setw(18) << "Buy Silver" << "\n";

	for (const auto& [name, playerAuctionData] : sortedPlayersAuctionData) {
		//std::cout << entry.toString();

		if (playerAuctionData.totalSilver >= 5'000'000) {
			out <<
				std::left <<
				std::setw(24) << name << 
				playerAuctionData.totalsToString();
			if (playerAuctionData.totalSilver >= 20'000'000) {
				size_t totalAmount = 0;
				for (const auto& order : playerAuctionData.orders) {
					totalAmount += order.amount;
				}

				out << 
					"total orders: " << 
					std::left << std::setw(8) << playerAuctionData.orders.size() <<
					" total amount: " << 
					std::setw(8) << totalAmount <<
					"\n" << 
					playerAuctionData.ordersToString();
			}
		}
	}

	out.close();
	std::cout << "\nSaved to " << filename << "\n";
}
