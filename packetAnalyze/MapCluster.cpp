#include "pch.h"

void MapCluster::findClusterData(NetworkCommand& command)
{
	DataLayout dataLayout;
	dataLayout.findDataLayout(command);
	//dataLayout.printInfo(command, true);
	//std::cout << "dataLayout.size(): " << dataLayout.size() << "\n";

	// more than 80 entries
	// 18 (4 floats) 25 (owner) 17 (plot type)
	// 20 (int64) 22 (int64 some tax) 23 (int32) 24 (int64) 33 (int32) 34 (int32) 
	// 
	// not interesting
	// 16 19 21 (tax fo associates) 26 27 28 (tax for users) 29 30 31 32
	// 
	// 80 entries
	// 35 (int 16) 36 (4 floats) 37 (2 floats) 38 (uint8) 39 (int64) 
	// 40 (list of 10 int8) 41 (list of 10 int8)

	DataFragment& coordsFragment = dataLayout.findFragment(18);
	std::vector<std::reference_wrapper<DataFragment>> ownerFragments = dataLayout.findFragments(25);

	DataFragment& idFragment = dataLayout.findFragment(35);

	DataFragment& someFragment0 = dataLayout.findFragment(20);
	DataFragment& someFragment1 = dataLayout.findFragment(22);
	DataFragment& someFragment2 = dataLayout.findFragment(23);
	DataFragment& someFragment3 = dataLayout.findFragment(24);
	DataFragment& someFragment4 = dataLayout.findFragment(33);
	DataFragment& someFragment5 = dataLayout.findFragment(34);

	uint16_t numOfAllEntries = net::read_uint16(command, coordsFragment._offset - 6);
	uint16_t numOfEntries = idFragment._numOfEntries;

	uint16_t numOfSubEntries = coordsFragment._numOfEntries;
	ptrdiff_t coordsOffset = coordsFragment._offset;

	std::unordered_map<std::string, size_t> ownerCounts;
	for (const auto& fragment : ownerFragments) {
		const DataFragment& ownerFragment = fragment.get();

		size_t from = ownerFragment._offset;
		size_t to = from + ownerFragment._numOfEntries;

		std::string ownerName(command.begin() + from, command.begin() + to);
		ownerCounts[ownerName]++;
	}

	std::vector<std::pair<std::string, size_t>> sortedOwners(ownerCounts.begin(), ownerCounts.end());
	std::sort(sortedOwners.begin(), sortedOwners.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;
		});

	std::cout << "----------------------\n";
	for (const auto& entry : sortedOwners) {
		std::cout << std::setw(16) << entry.first << " - " << entry.second << "\n";
	}

	/*for (size_t i = 0; i < numOfEntries; i++) {
		std::cout << net::read_uint16(command, idFragment._offset + i * idFragment._dataType._size) << " ";
		std::cout << (unsigned)net::read_uint8 (command, someFragment3._offset + i * someFragment3._dataType._size) << " ";
		std::cout << net::read_uint64(command, someFragment4._offset + i * someFragment4._dataType._size) << " \n";
	}*/
	for (size_t i = 0; i < numOfAllEntries; i++) {
		/*command.printCommandInOneString(
			ownerFragments[i].get()._offset, 
			ownerFragments[i].get()._offset + ownerFragments[i].get()._numOfEntries, 
			false, true
		);*/
		/*std::cout << std::setw(12) << net::read_uint64(command, someFragment0._offset + i * someFragment0._dataType._size) << " ";
		std::cout << std::setw(12) << net::read_uint64(command, someFragment1._offset + i * someFragment1._dataType._size) << " ";
		std::cout << std::setw(8)  << net::read_uint32(command, someFragment2._offset + i * someFragment2._dataType._size) << " ";
		std::cout << std::setw(12) << net::read_uint64(command, someFragment3._offset + i * someFragment3._dataType._size) << " ";
		std::cout << std::setw(8)  << net::read_uint32(command, someFragment4._offset + i * someFragment4._dataType._size) << " ";
		std::cout << std::setw(8)  << net::read_uint32(command, someFragment5._offset + i * someFragment5._dataType._size) << "\n";*/
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
