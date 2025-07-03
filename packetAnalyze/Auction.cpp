#include "pch.h"
#include "Auction.h"

//size_t counter = 0;
constexpr uint64_t sixHours = static_cast<uint64_t>(3600 * 6);
constexpr uint64_t years1970offset = static_cast<uint64_t>(62135596800);
uint16_t numOfColumns = 4;
uint16_t numOfRows = 113;
uint32_t previousCommandID = 0;
std::string previousAuctionOrdersString = "";

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

			for (size_t i = 0; i < 4; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i] << dataSeparator;
				}
				/*if (i == 3) {
					auctionAverageValues << itemData;
				}*/
				auctionAverageValues << i << " " << itemData << "\n";
			}
			//for (size_t i = 0; i < 2; i++) {
			//	for (size_t j = 0; j < auctionData.size(); j++) {
			//		auctionAverageValues << auctionData[j][i] << dataSeparator;
			//	}
			//	/*if (i == 1) {
			//		auctionAverageValues << itemData;
			//	}*/
			//	auctionAverageValues << i << " " << itemData << "\n";
			//}
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
		"currentLocation: " + currentLocation._locationID + " " +
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

void Auction::auctionOrders(
	NetworkCommand& command, bool isSellOrders, 
	Location& currentLocation, bool isFilterEnabled
)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);
	//dataLayout.printInfo(command);

	std::vector<std::reference_wrapper<DataFragment>> auctionOrdersData = dataLayout.findFragments(0);

	std::string auctionOrdersString{};
	for (const auto& auctionOrderData : auctionOrdersData) {
		const DataFragment& auctionOrderFragment = auctionOrderData.get();

		size_t from = auctionOrderFragment._offset;
		size_t to = from + auctionOrderFragment._numOfEntries;

		auctionOrdersString += std::string(command.begin() + from, command.begin() + to);
	}

	if (previousAuctionOrdersString != auctionOrdersString) {
		//std::cout << auctionOrdersString << "\n";
		processAuctionOrders(auctionOrdersString, isSellOrders, currentLocation, isFilterEnabled);
	}
	previousAuctionOrdersString = auctionOrdersString;
}

void Auction::processAuctionOrders(
	const std::string& auctionSellOrdersString, bool isSellOrders,
	Location& currentLocation, bool isFilterEnabled
) {
	std::unordered_map<std::string, std::unordered_map<std::string, std::pair<size_t, size_t>>> sellerData;

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
			nlohmann::json order = nlohmann::json::parse(jsonStr);
			std::string playerName = isSellOrders ? order["SellerName"] : order["BuyerName"];
			std::string itemTypeId = order["ItemTypeId"];
			size_t amount = order["Amount"];
			size_t totalPriceSilver = static_cast<double>(order["TotalPriceSilver"]) / 1e4;

			sellerData[playerName][itemTypeId].first += amount;
			sellerData[playerName][itemTypeId].second += totalPriceSilver;
		}
		catch (const std::exception& e) {
			std::cerr << "JSON Parsing Error: " << e.what() << "\n";
		}
	}

	size_t totalAmount = 0;
	size_t totalSilver = 0;
	std::vector<std::tuple<size_t, size_t, std::string, std::string>> sortedData;
	for (const auto& player : sellerData) {
		for (const auto& item : player.second) {
			size_t amount = item.second.first;
			size_t totalSilverPrice = item.second.second;
			sortedData.emplace_back(amount, totalSilverPrice, player.first, item.first);

			totalAmount += amount;
			totalSilver += totalSilverPrice;
		}
	}

	// Sort by amount in descending order
	std::sort(sortedData.rbegin(), sortedData.rend());

	// Compute threshold
	size_t thresholdAmount = static_cast<size_t>(1.0 * totalAmount);
	size_t accumulatedAmount = 0;

	// Print the sorted result
	if (isSellOrders) {
		std::cout << "sell orders\n";
	}
	else {
		std::cout << "buy orders\n";
	}

	std::ofstream auctionOrders;
	auctionOrders.open("auction orders.txt", std::ofstream::app);

	if (auctionOrders.is_open()) {
		std::cout << "total amount: " << totalAmount << "\n";
		for (const auto& [amount, totalSilverPrice, playerName, itemTypeId] : sortedData) {
			if (accumulatedAmount >= thresholdAmount) {
				break; // Stop once we have reached threshold
			}
			accumulatedAmount += amount;

			if (!isFilterEnabled || (amount >= 100 || totalSilverPrice >= 4e6)) {
				std::cout <<
					std::setw(16) << std::left << playerName << " "
					<< itemTypeId << " "
					<< std::setw(5) << amount << " "
					/*<< std::setw(10) << totalSilverPrice << " "*/
					<< totalSilverPrice / amount << " "
					<< /*std::string(magic_enum::enum_name(
						static_cast<locationId>(
							static_cast<uint32_t>(std::stoul(currentLocation._locationID)))))
					<< */"\n";
				/*auctionOrders <<
					std::setw(16) << std::left << playerName << " "
					<< itemTypeId << " "
					<< std::setw(5) << amount << " "
					<< totalSilverPrice / amount
					<< "\n";*/
			}
		}
		std::cout << "\n";
		auctionOrders.close();
	}
	else {
		std::cout << "auction orders.txt is not opened" << "\n";
	}
}
