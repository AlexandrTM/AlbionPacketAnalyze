#include "pch.h"
#include "Auction.h"

void Auction::findProductName(NetworkCommand& command, DataLayout& dataLayout)
{
	std::ofstream auctionAverageValues;

	auctionAverageValues.open("auctionAverageValues.txt", std::ofstream::app);
	if (auctionAverageValues.is_open()) {
		auctionAverageValues.close();
	}
	else {
		std::cout << "auctionAverageValues is not opened" << "\n";
	}
}

//size_t counter = 0;
uint64_t sixHours = static_cast<uint64_t>(3600 * 6);
uint16_t numOfColumns = 4;
uint16_t numOfRows = 113;
uint32_t previousCommandID = 0;

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
				auctionData[i][1] = std::numberToString(averagePrice);
				auctionData[i][2] = std::numberToString(soldVolume);
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

			/*for (size_t i = 0; i < 4; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i] << dataSeparator;
				}
				if (i == 3) {
					auctionAverageValues << itemData;
				}
				auctionAverageValues << "\n";
			}*/
			for (size_t i = 0; i < 2; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i == 1 ? 2 : i] << dataSeparator;
				}
				if (i == 1) {
					auctionAverageValues << itemData;
				}
				auctionAverageValues << "\n";
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

	size_t valuesToAdd = numOfRows - auctionData.size();
	uint64_t dateFirst = sixHours * numOfRows;
	if (auctionData.size() > 0) {
		dateFirst = std::stoull(auctionData[0][3]);
	}
	for (size_t i = 0; i < valuesToAdd; i++) {
		auctionData.push_back({ "0", "0", "0", std::numberToString(dateFirst - (i + 1) * sixHours) });
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