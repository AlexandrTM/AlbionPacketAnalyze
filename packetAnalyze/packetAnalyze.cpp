#include "pch.h"


using namespace Tins;

void printPacket(RawPDU pdu) {
    for (size_t i = 0; i < pdu.payload_size(); i++)
    {
        if ((int)pdu.payload()[i] < 16)
            std::cout << 0;
        std::cout << std::hex << (int)pdu.payload()[i];
        if (i % 4 == 3)
            std::cout << " ";
        if (i % 16 == 15 and i != pdu.payload_size())
            std::cout << "\n";
    }
}
void printPacket(RawPDU pdu, size_t regionStart, size_t regionEnd) {
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        if ((int)pdu.payload()[i] < 16)
            std::cout << 0;
        std::cout << std::hex << (int)pdu.payload()[i];
        if (i % 4 == 3)
            std::cout << " ";
        if (i % 16 == 15 and i != pdu.payload_size())
            std::cout << "\n";
    }
}
void readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, std::stringstream &ss)
{
    for (size_t i = 16; i < 20; i++)
        //for (size_t i = 0; i < pdu.rfind_pdu<RawPDU>().payload_size(); i++)
    {
        uint8_t x = pdu.payload()[i];
        if (x < 16)
            ss << 0;
        ss << std::hex << (int)x;
    }
}

bool callback(const PDU& pdu) {
    const IP& ip = pdu.rfind_pdu<IP>();// Find the IP layer
    const UDP& udp = pdu.rfind_pdu<UDP>();// Find the UDP layer
    IPv4Range albionIPRange = IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    RawPDU rawPDU = pdu.rfind_pdu<RawPDU>();
    if (albionIPRange.contains(ip.src_addr()) and udp.sport() == 5056)
    { 
        std::stringstream ss;
        if (rawPDU.payload_size() > 23)
        {
            // 0-4 num of commnads in packet, 
            // 4-8 time, not 20-22, eventcandidate 36-40
            // 8-12 location id
            // 12-16 only 4 types exists 07000000 06000100
            // 16-20 event code ?
            // one byte is 2
            readPacket(rawPDU, 16, 20, ss);

            if (ss.str() != "0000001c")
            {
                printPacket(rawPDU, 12, 16);
                //std::cout << stoi(ss.str());
                //std::cout << std::dec << "\n" << "Packet size: " 
                //    << rawPDU.payload_size() << "\n";
            }
            
        }
    }
    
    return true;
}

int main() {
    // Get the default interface (where the default gateway route is)
    NetworkInterface iface = NetworkInterface::default_interface();
    SnifferConfiguration albionConfig;
    albionConfig.set_filter("ip dst 192.168.1.71");
    //albionConfig.set_filter("");
    Sniffer(iface.name(), albionConfig).sniff_loop(callback);
   
}
