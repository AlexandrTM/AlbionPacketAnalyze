#include "pch.h"
#include "Auction.h"

void Auction::FindAuctionAverageValues(NetworkCommand& command)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);

	std::ofstream auctionAverageValues;

	auctionAverageValues.open("auctionAverageValues.txt"/*, std::ofstream::app*/);
	if (auctionAverageValues.is_open()) {
		DataFragment soldAmountFragment = dataLayout.findFragment(0);
		DataFragment soldVolumeFragment = dataLayout.findFragment(1);
		DataFragment dateFragment = dataLayout.findFragment(2);

		uint16_t numOfEntries = soldAmountFragment._numOfEntries;

		std::vector<std::string> ss;

		for (size_t i = 0; i < numOfEntries; i++) {

			uint32_t soldAmount = 0;
			uint64_t soldVolume = 0;
			double_t averagePrice = 0;
			uint64_t date = 0;

			if (soldAmountFragment._dataType._dataTypeSize == 4) {
				soldAmount = net::read_int32(command, soldAmountFragment._offset + i * 4);
			}
			else if (soldAmountFragment._dataType._dataTypeSize == 2) {
				soldAmount = net::read_int16(command, soldAmountFragment._offset + i * 2);
			}
			soldVolume = net::read_int64(command, soldVolumeFragment._offset + i * 8) / 1e+4;
			averagePrice = (double_t)soldVolume / soldAmount;
			date = net::read_int64(command, dateFragment._offset + i * 8) / 1e+7;

			ss.push_back(std::numberToString(soldAmount));
			ss.push_back(std::numberToString(soldVolume));
			ss.push_back(std::numberToString(averagePrice));
			ss.push_back(std::numberToString(date));
		}
		
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < numOfEntries; j++) {
				auctionAverageValues << ss[j * 4 + i] << ",";
			}
			auctionAverageValues << "\n";
		}

		auctionAverageValues.close();
		//dataLayout.printInfo();
		//command.printCommandInOneString();
	}
	else {
		std::cout << "auctionAverageValues not opened" << "\n";
	}
}
