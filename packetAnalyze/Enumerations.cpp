#include "pch.h"

void NetworkPacketInfo::to_string() const
{
	std::cout << std::left <<
		"Source IP: "        << std::setw(15) << src_ip.to_string() << " "
		"Source Port: "      << std::setw(5)  << src_port << " "
		"Destination IP: "   << std::setw(15) << dst_ip.to_string() << " "
		"Destination Port: " << std::setw(5)  << dst_port << "\n";
}
