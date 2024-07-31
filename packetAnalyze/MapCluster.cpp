#include "pch.h"

void MapCluster::findClusterData(NetworkCommand& command)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);
	dataLayout.printInfo(command, true);

	DataFragment elementCoordsFragment = dataLayout.findFragment(18);
	// 35 some id
	DataFragment feeFragment = dataLayout.findFragment(35);
	DataFragment ownerFragment = dataLayout.findFragment(25);

	uint16_t numOfEntries = net::read_uint16(command, elementCoordsFragment._offset - 6);
	uint16_t numOfSubEntries = elementCoordsFragment._numOfEntries;

	ptrdiff_t float32_offset = elementCoordsFragment._offset;

	for (size_t i = 0; i < numOfEntries; i++) {
		std::cout << net::read_uint16(command, feeFragment._offset + i * feeFragment._dataType._size) << " \n";
	}
	/*for (size_t i = 0; i < numOfEntries; i++) {
		for (size_t j = 0; j < numOfSubEntries; j++) {
			std::cout << net::read_float32(command, float32_offset) << " ";
			float32_offset += 4;
		}
		float32_offset += 3;
		std::cout << "\n";
	}*/
}
