#include "pch.h"
#include "Auction.h"

void Auction::FindAuctionAverageValues(NetworkCommand& command)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);

	std::ofstream auctionAverageValues;

	auctionAverageValues.open("auctionAverageValues.txt");
	if (auctionAverageValues.is_open()) {
		ptrdiff_t soldAmountOffset = dataLayout.findFragment(0)._offset;
		ptrdiff_t soldVolumeOffset = dataLayout.findFragment(1)._offset;
		ptrdiff_t dateOffset = dataLayout.findFragment(2)._offset;

		uint16_t numOfEntries = dataLayout.findFragment(0)._numOfEntries;

		for (size_t i = 0; i < numOfEntries; i++) {
			uint32_t soldAmount = net::read_int32(command, soldAmountOffset + i * 4);
			uint64_t soldVolume = net::read_int64(command, soldVolumeOffset + i * 8) / 10000;
			double_t averagePrice = (double_t)soldVolume / soldAmount;
			uint64_t date = net::read_int64(command, dateOffset + i * 8);

			auctionAverageValues << soldAmount   << "," <<
									soldVolume   << "," <<
									averagePrice << "," <<
									date		 << "," << "\n";
			/*std::cout << soldAmountOffset + i * 4 << " " <<
						 soldVolumeOffset + i * 8 << " " <<
						 dateOffset       + i * 8 << "\n";*/
		}

		auctionAverageValues.close();
		//dataLayout.printInfo();
		command.printCommandInOneString();
	}
	else {
		std::cout << "auctionAverageValues not opened" << "\n";
	}
}
