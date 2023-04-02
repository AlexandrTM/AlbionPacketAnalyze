#include "pch.h"
#include "Auction.h"

uint64_t sixHours = static_cast<uint64_t>(3600 * 6);
uint16_t numOfRows = 4;

NetworkCommand commandBuffer;
void Auction::FindAuctionAverageValues(NetworkCommand& command)
{
	if (commandBuffer != command) {

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

				uint8_t soldAmountDataTypeSize = soldAmountFragment._dataType._dataTypeSize;
				if (soldAmountDataTypeSize == 4) {
					soldAmount = net::read_int32(command, soldAmountFragment._offset + i * 4);
				}
				else if (soldAmountDataTypeSize == 2) {
					soldAmount = net::read_int16(command, soldAmountFragment._offset + i * 2);
				}
				else if (soldAmountDataTypeSize == 1) {
					soldAmount = net::read_int8(command, soldAmountFragment._offset + i);
				}
				else if (soldAmountDataTypeSize == 8) {
					soldAmount = net::read_int64(command, soldAmountFragment._offset + i * 8);
				}
				soldVolume = net::read_int64(command, soldVolumeFragment._offset + i * 8) / 1e+4;
				averagePrice = (double_t)soldVolume / soldAmount;
				date = net::read_int64(command, dateFragment._offset + i * 8) / 1e+7;

				auctionData[i].push_back(std::numberToString(soldAmount));
				auctionData[i].push_back(std::numberToString(averagePrice));
				auctionData[i].push_back(std::numberToString(soldVolume));
				auctionData[i].push_back(std::numberToString(date));
			}

			size_t idx = 3;
			std::sort(auctionData.begin(), auctionData.end(),
				[idx](const std::vector<std::string>& a, const std::vector<std::string>& b) {
					return std::stoull(a.at(idx)) < std::stoull(b.at(idx));
				});

			uint64_t datePrevious = 0;
			size_t addedValues = 0;
			for (size_t i = 0; i < auctionData.size() - addedValues; i++) {
				std::cout << auctionData.size() << "\n";

				uint64_t date = std::stoull(auctionData[i + addedValues][3]);
				if (i > 0) {
					size_t dateDifference = (date - datePrevious) / sixHours;
					//std::cout << dateDifference << "\n";
					if (dateDifference > 1) {
						//std::cout << date << "\n";
						for (size_t j = 1; j < dateDifference; j++) {
							auctionData.insert(auctionData.begin() + i + j - 1,
								{ "0", "0", "0", std::numberToString(datePrevious + j * sixHours) });
							addedValues += 1;
							//std::cout << datePrevious + j * sixHours << "\n";
						}
					}
				}

				//std::cout << date << " " << datePrevious << "\n";
				//std::cout << auctionData.size() << "\n";
				datePrevious = date;
			}


			for (size_t i = 0; i < numOfRows; i++) {
				for (size_t j = 0; j < auctionData.size(); j++) {
					auctionAverageValues << auctionData[j][i] << ",";
				}
				auctionAverageValues << "\n";
			}

			auctionAverageValues.close();
		}
		else {
			std::cout << "auctionAverageValues not opened" << "\n";
		}
	}

	//command.printCommandInOneString();
	commandBuffer = command;
}
