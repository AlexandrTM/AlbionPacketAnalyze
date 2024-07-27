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
uint16_t numOfRows = 4;
NetworkCommand commandBuffer;
void Auction::findAuctionAverageValues(NetworkCommand& command)
{
	if (commandBuffer != command) {

		//counter += 1;
		//std::cout << counter << "\n";
		DataLayout dataLayout;
		dataLayout.findDataLayout(command);

		std::ofstream auctionAverageValues;

		auctionAverageValues.open("auctionAverageValues.txt", std::ofstream::app);
		if (auctionAverageValues.is_open()) {

			DataFragment soldAmountFragment = dataLayout.findFragment(0);
			DataFragment soldVolumeFragment = dataLayout.findFragment(1);
			DataFragment dateFragment = dataLayout.findFragment(2);

			uint16_t numOfEntries = soldAmountFragment._numOfEntries;

			std::vector<std::vector<std::string>> auctionData(numOfEntries);

			for (size_t i = 0; i < numOfEntries; i++) {

				uint32_t soldAmount = 0;
				uint64_t soldVolume = 0;
				double_t averagePrice = 0;
				uint64_t date = 0;

				uint8_t soldAmountDataTypeSize = soldAmountFragment._dataType._size;
				if (soldAmountDataTypeSize == 4) {
					soldAmount = net::read_uint32(command, soldAmountFragment._offset + i * 4);
				}
				else if (soldAmountDataTypeSize == 2) {
					soldAmount = net::read_uint16(command, soldAmountFragment._offset + i * 2);
				}
				else if (soldAmountDataTypeSize == 1) {
					soldAmount = net::read_uint8(command, soldAmountFragment._offset + i);
				}
				else if (soldAmountDataTypeSize == 8) {
					soldAmount = net::read_uint64(command, soldAmountFragment._offset + i * 8);
				}
				soldVolume = net::read_uint64(command, soldVolumeFragment._offset + i * 8) / 1e+4;
				averagePrice = (double_t)soldVolume / soldAmount;
				date = net::read_uint64(command, dateFragment._offset + i * 8) / 1e+7;

				auctionData[i].push_back(std::numberToString(soldAmount));
				auctionData[i].push_back(std::numberToString(averagePrice));
				auctionData[i].push_back(std::numberToString(soldVolume));
				auctionData[i].push_back(std::numberToString(date));
			}
			//std::cout << auctionData[auctionData.size() - 1][3] << "\n";

			size_t IDOfSortingRow = 3;
			std::sort(auctionData.begin(), auctionData.end(),
				[IDOfSortingRow](const std::vector<std::string>& a, const std::vector<std::string>& b) {
					return std::stoull(a.at(IDOfSortingRow)) < std::stoull(b.at(IDOfSortingRow));
				});

			addEmptyEntries(auctionData);

			std::sort(auctionData.begin(), auctionData.end(),
				[IDOfSortingRow](const std::vector<std::string>& a, const std::vector<std::string>& b) {
					return std::stoull(a.at(IDOfSortingRow)) < std::stoull(b.at(IDOfSortingRow));
				});

			for (size_t i = 0; i < numOfRows; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i] << ",";
				}
				auctionAverageValues << "\n";
			}
			auctionAverageValues.close();
		}
		else {
			std::cout << "auctionAverageValues is not opened" << "\n";
		}
	}

	//command.printCommandInOneString();
	commandBuffer = command;
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

	size_t valuesToAdd = 113 - auctionData.size();
	uint64_t dateFirst = sixHours * 113;
	if (auctionData.size() > 0) {
		dateFirst = std::stoull(auctionData[0][3]);
	}
	for (size_t i = 0; i < valuesToAdd; i++) {
		auctionData.push_back({ "0", "0", "0", std::numberToString(dateFirst - (i + 1) * sixHours) });
	}
}